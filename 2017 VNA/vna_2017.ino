#include "Arduino.h"
#include "Wire.h"
#include "si5351.h"
#include "quadrature.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "DynamicCommandParser.h"

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
void processCommand(char **values, int valueCount);

void setup()
{
    int n;

    adc14_main();
    si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
    dcp.addParser("MEASURE", processCommand);
    for(n=0;n<SAMPLE_LENGTH;n++)
    {
        shift[n] = cos(OMEGA_IF*n/SAMPLE_FREQUENCY)\
                *0.5*(1-cos(2*PI*n/(SAMPLE_LENGTH-1))); // Hanning window
    }

    // Initialize the data parser using the start, end and delimiting character
    DynamicCommandParser dcp('^', '$', ',');

    // Query a status update and wait a bit to let the Si5351 populate the
    // status flags correctly.
    si5351.update_status();
    ADC14_enableConversion(); // Start conversions for testing.
    delay(500);
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

void processCommand(char **values, int valueCount)
{
    int i;
    unsigned long long fMin, fMax, deltaFreq, freq[MAX_NUMBER_FREQ];
    if(valueCount != 4)
    {
        return;  // Something is wrong if you end up here.
    }
    fMin = atoi(values[0])*100ULL;
    fMax = atoi(values[1])*100ULL;
    numberFrequenciestoMeasure = atoi(values[2]);

    deltaFreq = (fMax-fMin)/numberFrequenciestoMeasure;
    /* The idea is that we will get the first frequency's data, and then
     * send it out the serial port to gnu octave while we are getting the
     * next frequency's data.  We use the multithreaded Energia stuff to do
     * this.  The MultiTaskSerial.ino does the sending.
     */
    for(i=0;i<numberFrequenciestoMeasure;i++)
    {
        freq[i]=(fMin+i*deltaFreq);
        si5351.set_freq(freq[i], SI5351_CLK0);
        si5351.set_freq(freq[i]+100ULL*F_IF, SI5351_CLK1);
        si5351.set_freq(freq[i]+100ULL*F_IF, SI5351_CLK2);
        //ADC14_enableConversion();
        delay(1000);
        startSampling();
        while(!doneADC)
        {
            //sleep(10); // Wait until it is done converting everything.
            // Eventually we want to do concurrent processing.
            // For now we will just let the ADC interrupt this loop
            // and finish up its job.
        }
        simpleDownConverter();
        while(sendMeasurement)
        {
            // Wait until the last measurement is sent.  (MultiTaskSerial sets it false.)
        }
        frequencyIndex = i;
        sendMeasurement = true;
    }
    return;
}
