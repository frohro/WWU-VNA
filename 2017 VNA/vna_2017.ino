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
#define F_IF 500
#define OMEGA_IF F_IF*2*PI

Si5351 si5351;
DynamicCommandParser dcp('^', '$', ',');

volatile uint16_t refRe[SAMPLE_LENGTH];
volatile uint16_t refIm[SAMPLE_LENGTH];
volatile uint16_t measRe[SAMPLE_LENGTH];
volatile uint16_t measIm[SAMPLE_LENGTH];
extern volatile bool doneADC;
volatile bool sendMeasurement = false;
volatile int numberFrequenciestoMeasure, frequencyIndex;
volatile float  refSum, measSum;


float shift[SAMPLE_LENGTH];  // Make this constant sometime.

int simpleDownConverter(void);
void sweepFreqMeas(char **values, int valueCount);
void voltageMeasurement(char **values, int valueCount);

void setup()
{
    int n;

    adc14_main(); // Initialize ADC14 for multichannel conversion at 8 kHz.
    si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);

    // Initialize the data parser using the start, end and delimiting character
    DynamicCommandParser dcp('^', '$', ',');
    // For frequency sweep: "^SWEEP,Fmin,Fmax,NFreq$"
    dcp.addParser("SWEEP", sweepFreqMeas);
    // Returns single freqency measurements as a function of time:  "^TIME,Freq$"
    dcp.addParser("TIME", voltageMeasurement);

    for(n=0;n<SAMPLE_LENGTH;n++) // Initialize shift, should make constant later.
    {
        shift[n] = cos(OMEGA_IF*n/SAMPLE_FREQUENCY)\
                *0.5*(1-cos(2*PI*n/(SAMPLE_LENGTH-1))); // Hanning window
    }
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
    int j;
    float refReSum, refImSum, measReSum, measImSum;
    refReSum = 0;
    refImSum = 0;
    measReSum = 0;
    measImSum = 0;
    for(j=0;j<SAMPLE_LENGTH;j++)
    {
        refReSum = refRe[j]*shift[j]+refReSum;
        refImSum = refIm[j]*shift[j]+refImSum;
        measReSum = measRe[j]*shift[j]+measReSum;
        measImSum = measIm[j]*shift[j]+measImSum;
    }
    refSum = refReSum + refImSum;  // Not I/Q mixer average.
    measSum = measReSum + measImSum;
    return(1);  // Later fix this to report errors if there are any.
}

void sweepFreqMeas(char **values, int valueCount)
{
    int i;
    unsigned long long fMin, fMax, deltaFreq, freq[MAX_NUMBER_FREQ];
    if(valueCount != 4)
    {
        Serial.println("In sweepFreqMeas, number of arguments is not correct.");
        return;  // Something is wrong if you end up here.
    }
    fMin = atoi(values[1])*100ULL;
    fMax = atoi(values[2])*100ULL;
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
        freq[i]=(fMin+i*deltaFreq);
        si5351.set_freq(freq[i], SI5351_CLK0);
        si5351.set_freq(freq[i]+100ULL*F_IF, SI5351_CLK1);
        si5351.set_freq(freq[i]+100ULL*F_IF, SI5351_CLK2);
        delay(1000); // Do we need this long?
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
    return;
}

void voltageMeasurement(char **values, int valueCount)
{
    int j;
    unsigned long long freq;
    if(valueCount != 2)
    {
        Serial.println("In voltageMeasurement, number of arguments is not correct.");
        return;  // Something is wrong if you end up here.
    }
    freq = atoi(values[1]);
    si5351.set_freq(freq, SI5351_CLK0);
    si5351.set_freq(freq+100ULL*F_IF, SI5351_CLK1);
    si5351.set_freq(freq+100ULL*F_IF, SI5351_CLK2);
    delay(1000);
    for(j=0;j<SAMPLE_LENGTH;j++)
    {
        Serial.print(refRe[j]);
        Serial.print(", ");
        Serial.print(refIm[j]);
        Serial.print(", ");
        Serial.print(measRe[j]);
        Serial.print(", ");
        Serial.println(measIm[j]);
        Serial.flush(); // Waits until completion of transmitted data.
    }
}
