#include "Arduino.h"
#include "Wire.h"
#include "si5351.h"
#include "quadrature.h"
#include <driverlib.h>
#include "DynamicCommandParser.h" // https://github.com/mdjarv/DynamicCommandParser

extern "C"{
#include "adc14vna.h"
};
#include <stdio.h>
#include <math.h>

#define BUFFER_LENGTH 256
#define MAX_NUMBER_FREQ 1000


Si5351 si5351;
DynamicCommandParser dcp('^', '$', ',');  //https://github.com/mdjarv/DynamicCommandParser

volatile uint16_t ref[SAMPLE_LENGTH];
volatile uint16_t meas[SAMPLE_LENGTH];
extern volatile bool doneADC;
volatile bool sendMeasurement = false;
volatile int numberFrequenciestoMeasure, frequencyIndex;
volatile float refSumRe, measSumRe, refSumIm, measSumIm;

float shift[SAMPLE_LENGTH];  // Make this constant sometime.

int simpleDownConverter(void);
void sweepFreqMeas(char **values, int valueCount);
void voltageMeasurement(char **values, int valueCount); // For testing (sending individual voltages.
void setOscillator(unsigned long long freq);
void sendSampleRate(char **values, int valueCount);

void setup()
{
	adc14_main(); // Initialize ADC14 for multichannel conversion at 500 kHz.
    si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
    // For debugging 1/4/2018
    si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA);
    // Initialize the data parser using the start, end and delimiting character
    Serial.begin(115200);
    // For frequency sweep: "^SWEEP,Fmin,Fmax,NFreq$"
    dcp.addParser("SWEEP", sweepFreqMeas);
    // Returns single frequency measurements as a function of time:  "^TIME,Freq$"
    dcp.addParser("TIME", voltageMeasurement);
    // Returns the sample rate:  "^SAMPLERATE,Fs$"
    dcp.addParser("SAMPLERATE", sendSampleRate);

    /* This is no window: and partially optimized for RAM use:  You could improve it by
     * a factor of two, but using a quarter of a cycle.  With no window, we need to make
     * sure the sampling interval is an integer number of cycles. */
/*    for(int n=0;n<SAMPLES_IN_ONE_CYCLE;n++)
    {
        shift[n] = cos(OMEGA_IF*n/SAMPLE_FREQUENCY);
    }*/
    for(int n=0;n<SAMPLE_LENGTH;n++) // Initialize shift, should make constant later.
     {

         /* Pick one:  This is with a Hanning window; uses more memory, and isn't as good
          * for a perfect sample length and rate which we control.*/
         shift[n] = cos(OMEGA_IF*n/SAMPLE_FREQUENCY)\
                 *0.5*(1-cos(2*PI*n/(SAMPLE_LENGTH-1))); // Hanning window
     }
    setOscillator(10000000);
    Serial.println("Done with setup.");
}

void loop()
{
    while(Serial.available() > 0)
    {
        dcp.appendChar(Serial.read());
    }
}

int simpleDownConverter(void)    // Do DSP here.
{
    refSumRe = 0.0;
    refSumIm = 0.0;
    measSumRe = 0.0;
    measSumRe = 0.0;
    for(int n=0;n<SAMPLE_LENGTH;n++)
    {
        /* This needs to be changed if you use a Hanning window */
        //refSumRe += shift[n%SAMPLES_IN_ONE_CYCLE]*ref[n];
        //refSumIm -= shift[(n+(int)(SAMPLES_IN_ONE_CYCLE/4+0.5))%SAMPLES_IN_ONE_CYCLE]*ref[n];
        //measSumRe += shift[n%SAMPLES_IN_ONE_CYCLE]*meas[n];
        //measSumIm -= shift[(n+(int)(SAMPLES_IN_ONE_CYCLE/4+0.5))%SAMPLES_IN_ONE_CYCLE]*meas[n];
    }
    return(1);  // Later fix this to report errors if there are any.
}

void sweepFreqMeas(char **values, int valueCount) // Might change function type to return errors.
{
    int i;
    unsigned long long fMin, fMax, deltaFreq, freq[MAX_NUMBER_FREQ];
    if(valueCount != 4)
    {
        Serial.println("In sweepFreqMeas, number of arguments is not correct.");
        return;  // Something is wrong if you end up here.
    }
    fMin = atoi(values[1]);
    fMax = atoi(values[2]);
    numberFrequenciestoMeasure = atoi(values[3]);

    deltaFreq = (fMax-fMin)/numberFrequenciestoMeasure;
    /* The idea is that we will get the first frequency's data, and then
     * send it out the serial port to gnu octave while we are getting the
     * next frequency's data.  We use the multithreaded Energia stuff to do
     * this sending.  The MultiTaskSerial.ino does the sending.  We compute the data
     * after we have collected the SAMPLE_LENGTH of it at one frequency.
     * Then we go on to the next frequency.
     *
     * At this time, we are waiting until the sending is finished, before
     * going on to the next frequency.  However, we should be able to determine
     * which operation takes the longest, and do both at the same time, to make
     * measurements quicker.  It would also speed things up to send binary data,
     * but this is harder to debug, so for now, we want to use ASCII.
     */
    for(i=0;i<numberFrequenciestoMeasure;i++)
    {
        freq[i]=fMin+i*deltaFreq;
        setOscillator(freq[i]);
        ADC14_enableConversion();
        while(!doneADC)
        {
            /* Wait until it is done converting everything at
             * this frequency.  Eventually we want to do concurrent processing.
             * For now we will just let the ADC interrupt this loop
             * and finish up its job.
             */
        }
        simpleDownConverter();
        while(sendMeasurement)
        {
            /* Wait until the last measurement is sent.  (MultiTaskSerial sets it false.)
             * Eventually we will want to do concurrent processing, but for now this is
             * safer.
             */
        }
        frequencyIndex = i;
        sendMeasurement = true;
    }
    for(i=0;i<numberFrequenciestoMeasure;i++)
    {
        Serial.print(refSumRe);
        Serial.println(", ");
    }
    for(i=0;i<numberFrequenciestoMeasure;i++)
    {
        Serial.print(refSumIm);
        Serial.println(", ");
    }
    for(i=0;i<numberFrequenciestoMeasure;i++)
    {
        Serial.print(measSumRe);
        Serial.println(", ");
    }
    for(i=0;i<numberFrequenciestoMeasure;i++)
    {
        Serial.print(measSumIm);
        Serial.println(", ");
    }
    // After we are done sending all the data, we should probably send a termination character or string.
    return;
}

void voltageMeasurement(char **values, int valueCount) // Might want to return error number.
{
    int j;
    unsigned long long freq;
    if (valueCount != 2)
    {
        Serial.println(
                "In voltageMeasurement, number of arguments is not correct.");
        return;  // Something is wrong if you end up here.
    }
    freq = atoi(values[1]);
    setOscillator(freq);
    //ADC14_enableConversion();
    startConversion();
    while(!doneADC)
    {}
    {
        for (j = 0; j < SAMPLE_LENGTH; j++)
        {
            Serial.print(ref[j]);
            Serial.print(", ");
            //Serial.flush(); // Waits until completion of transmitted data.
        }
        Serial.print('\n');
        //Serial.println();
//        Serial.flush();
        //delay(500);
        for (j = 0; j < SAMPLE_LENGTH; j++)
        {
            Serial.print(meas[j]);
            Serial.print(", ");
            //Serial.flush(); // Waits until completion of transmitted data.
        }
        Serial.print('\n');
        //Serial.println();
//        Serial.flush();
    }
}

void setOscillator (unsigned long long freq) // freq in Hz
{
    si5351.set_freq(freq*100ULL, SI5351_CLK0);
    si5351.set_freq(freq*100ULL+100ULL*F_IF, SI5351_CLK1);
    si5351.set_freq(freq*100ULL+100ULL*F_IF, SI5351_CLK2);
    delay(1000); // Wait for oscillator and steady state.  Do we need 1 second?
}

void sendSampleRate (char **values, int valueCount)
{
    int Fs = SAMPLE_FREQUENCY;
    int N = SAMPLE_LENGTH;
    Serial.println(Fs);
    Serial.println(N);
    Serial.println(F_IF);
}


