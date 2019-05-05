#include "Arduino.h"
#include "Wire.h"
#include "si5351.h"
#include <driverlib.h>
#include "constants.h"
#include "DynamicCommandParser.h" // https://github.com/mdjarv/DynamicCommandParser
#include "adc14vna.h"
#include "SignalProcessing.h"
#include <stdio.h>
#include <cmath>

#define MAX_NUMBER_FREQ 1000


Si5351 si5351;
DynamicCommandParser dcp('^', '$', ',');  //https://github.com/mdjarv/DynamicCommandParser

volatile bool sendMeasurement = false;
volatile int numberFrequenciestoMeasure, frequencyIndex;
volatile float refSumRe, measSumRe, refSumIm, measSumIm;

//int simpleDownConverter(void);
void sweepFreqMeas(char **values, int valueCount);
void voltageMeasurement(char **values, int valueCount); // For testing (sending individual voltages.
void setOscillator(unsigned long long freq);
void sendSampleRate(char **values, int valueCount);
void computeMeasurement(char **values, int valueCount);
void computeFundamental(void);

void setup()
{
    Serial.begin(115200);
	adc14_main(); // Initialize ADC14 for multi-channel conversion.
    si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
    delay(5);
    si5351.output_enable(SI5351_CLK1, 0); // Disable this clock LO_Q (not used).
    delay(5);
    // For debugging 1/4/2018
    si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA);
    delay(5);
    si5351.drive_strength(SI5351_CLK2, SI5351_DRIVE_8MA);
    delay(5);
   /* si5351.set_ms_source(SI5351_CLK0, SI5351_PLLA);
    delay(50);
    si5351.set_ms_source(SI5351_CLK2, SI5351_PLLB);
    delay(50);*/
    // Initialize the data parser using the start, end and delimiting character
    Serial.begin(115200);
    // For frequency sweep: "^SWEEP,Fmin,Fmax,NFreq$"
    dcp.addParser("SWEEP", sweepFreqMeas);

    // Returns single frequency measurements as a function of time:  "^TIME,Freq$"
    dcp.addParser("TIME", voltageMeasurement);

    // Returns the sample rate:  "^SAMPLERATE,1=TIME/0=COMPUTE$"
    dcp.addParser("SAMPLERATE", sendSampleRate);

    // Returns the dft around 155 HZ with a bin of about 10 HZ:  "^COMPUTE,Fs$"
    dcp.addParser("COMPUTE", computeMeasurement);

    dcp.addParser("TIMECOMPUTE", timeCompute);

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

/*
int simpleDownConverter(void)    // Do DSP here.
{
    refSumRe = 0.0;
    refSumIm = 0.0;
    measSumRe = 0.0;
    measSumRe = 0.0;
    for(int n=0;n<SAMPLE_LENGTH;n++)
    {
         This needs to be changed if you use a Hanning window
        //refSumRe += shift[n%SAMPLES_IN_ONE_CYCLE]*ref[n];
        //refSumIm -= shift[(n+(int)(SAMPLES_IN_ONE_CYCLE/4+0.5))%SAMPLES_IN_ONE_CYCLE]*ref[n];
        //measSumRe += shift[n%SAMPLES_IN_ONE_CYCLE]*meas[n];
        //measSumIm -= shift[(n+(int)(SAMPLES_IN_ONE_CYCLE/4+0.5))%SAMPLES_IN_ONE_CYCLE]*meas[n];
    }
    return(1);  // Later fix this to report errors if there are any.
}*/

void sweepFreqMeas(char **values, int valueCount)
{
    unsigned long long fMin, fMax, deltaFreq, freq[MAX_NUMBER_FREQ];
    int i;
    if(valueCount != 4)
    {
        Serial.println("In sweepFreqMeas, number of arguments is not correct.");
        return;  // Something is wrong if you end up here.
    }
    fMin = atoi(values[1]);
    fMax = atoi(values[2]);
    numberFrequenciestoMeasure = atoi(values[3]);
    deltaFreq = (fMax-fMin)/numberFrequenciestoMeasure;
    /*The idea is that we will get the first frequency's data, and then
    * send it out the serial port to Gnu Octave while we are getting the
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
       computeFundamental();
   }
}

void computeMeasurement(char **values, int valueCount)
{
    int j;
    unsigned long long freq;
    if (valueCount != 2)
    {
        Serial.println(
                "In computeMeasurement, number of arguments is not correct.");
        return;  // Something is wrong if you end up here.
    }
    TIME_MEASUREMENT = false;
    freq = atoi(values[1]);
    setOscillator(freq);

    computeFundamental();
}

void computeFundamental()
{
    // Measured computation
    computation m;
    // Real computation
    computation r;

    startConversion();

    while(!doneADC) {
        // Wait for next measurement
        while(!doneConv) { /* about 800k cycles doing nothing :(  Applying a filter takes about 50k-55k*/}
        auto index = sampleCount - 1;
        doneConv = false;
        compute_block(r, index, &ref[DMA_Block * (index % 2)], DMA_Block);
        compute_block(m, index, &meas[DMA_Block * (index % 2)], DMA_Block);
    }
    Serial.print(m.real);
    Serial.print(",");
    Serial.print(m.imag);
    Serial.print(",");
    Serial.print(r.real);
    Serial.print(",");
    Serial.println(r.imag);
    delay(3);  // Testing to see if this stops the hangs.  delay(500) seems to fix the problem.
    // What about shorter delays?
}

void voltageMeasurement(char **values, int valueCount) // Might want to return error number.
{
    TIME_MEASUREMENT = true;
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
    startConversion();
    while(!doneADC)
    {}
    {
        for (j = 0; j < TIME_SAMPLE_LENGTH; j++)
        {
            Serial.print(ref[j]);
            Serial.print(",");
        }
        Serial.print('\n');
        delay(2000);

        for (j = 0; j < TIME_SAMPLE_LENGTH; j++)
        {
            Serial.print(meas[j]);
            Serial.print(",");
        }
        Serial.print('\n');
        delay(2000);
    }
}

void timeCompute(char **values, int valueCount)
{
    TIME_MEASUREMENT = true;
    int j;
    unsigned long long freq;
    if (valueCount != 2)
    {
        Serial.println(
                "In timeCompute, number of arguments is not correct.");
        return;  // Something is wrong if you end up here.
    }
    freq = atoi(values[1]);
    setOscillator(freq);
    startConversion();
    while(!doneADC)
    {}
    // Measured computation
    computation m;
    // Real computation
    computation r;
    compute_block(r, 0, ref, TIME_SAMPLE_LENGTH);
    compute_block(m, 0, meas, TIME_SAMPLE_LENGTH);
    {
        for (j = 0; j < TIME_SAMPLE_LENGTH; j++)
        {
            Serial.print(ref[j]);
            Serial.print(",");
        }
        Serial.print('\n');
        for (j = 0; j < TIME_SAMPLE_LENGTH; j++)
        {
            Serial.print(meas[j]);
            Serial.print(",");
        }
        Serial.print('\n');
    }
    Serial.print(r.real);
    Serial.print(",");
    Serial.print(r.imag);
    Serial.print(",");
    Serial.print(m.real);
    Serial.print(",");
    Serial.print(m.imag);
    Serial.print("\n");
}

void setOscillator (unsigned long long freq) // freq in Hz
{

    si5351.set_freq(freq*100ULL, SI5351_CLK0);
    delay(5);
    si5351.set_freq(freq*100ULL-100ULL*F_IF, SI5351_CLK2); // LO_I
    delay(5); // Wait for oscillator and steady state.  Do we need 1 second?
 /*   si5351.update_status();
    delay(5);
    while(si5351.dev_status.SYS_INIT+si5351.dev_status.LOL_A+si5351.dev_status.LOL_B)
    { // Wait until PLLs lock.
        si5351.update_status();
        delay(50);
        Serial.print("SYS_INIT: ");
        Serial.print(si5351.dev_status.SYS_INIT);
        Serial.print("  LOL_A: ");
        Serial.print(si5351.dev_status.LOL_A);
        Serial.print("  LOL_B: ");
        Serial.print(si5351.dev_status.LOL_B);
        Serial.print("  LOS: ");
        Serial.print(si5351.dev_status.LOS);
    }*/
}

void sendSampleRate (char **values, int valueCount)
{
    if (valueCount != 2)
    {
        Serial.println(
                "In sendSampleRate, number of arguments is not correct.");
        return;  // Something is wrong if you end up here.
    }
    auto type = atoi(values[1]);

    Serial.println(SAMPLE_FREQUENCY);
    if (!type)
        Serial.println(SAMPLE_LENGTH);
    else
        Serial.println(TIME_SAMPLE_LENGTH);
    Serial.println(F_IF);
}


