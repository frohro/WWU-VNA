#include "Energia.h"

#line 1 "/home/frohro/Projects/2017_VNA/2018 Simple VNA/vna_2017.ino"
#include "Arduino.h"
#include "Wire.h"
#include "si5351.h"
#include "quadrature.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "DynamicCommandParser.h" 

void setup();
void loop();
void setOscillator (unsigned long long freq);
void sendSampleRate (char **values, int valueCount);
void setupSerial();
void loopSerial();

#line 8
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

volatile uint16_t ref[SAMPLE_LENGTH];
volatile uint16_t meas[SAMPLE_LENGTH];
extern volatile bool doneADC;
volatile bool sendMeasurement = false;
volatile int numberFrequenciestoMeasure, frequencyIndex;
volatile float  refSum, measSum;

float shift[SAMPLE_LENGTH];  

int simpleDownConverter(void);
void sweepFreqMeas(char **values, int valueCount);
void voltageMeasurement(char **values, int valueCount); 
void setOscillator(unsigned long long freq);
void sendSampleRate(char **values, int valueCount);

void setup()
{
    adc14_main(); 
    si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
    
    si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA);
    setOscillator(200000000ULL);
    
    
    dcp.addParser("SWEEP", sweepFreqMeas);
    
    dcp.addParser("TIME", voltageMeasurement);
    
    dcp.addParser("SAMPLERATE", sendSampleRate);
    for(int n=0;n<SAMPLE_LENGTH;n++) 
    {
        shift[n] = cos(OMEGA_IF*n/SAMPLE_FREQUENCY)\
                *0.5*(1-cos(2*PI*n/(SAMPLE_LENGTH-1))); 
    }
}

void loop()
{
    while(Serial.available() > 0)
    {
        dcp.appendChar(Serial.read());
    }
}

int simpleDownConverter(void)    
{
    int j;
    float refSum, measSum;
    refSum = 0.0;
    measSum = 0.0;
    for(j=0;j<SAMPLE_LENGTH;j++)
    {
        refSum = ref[j]*shift[j]+refSum;
        measSum = meas[j]*shift[j]+measSum;
    }
    return(1);  
}

void sweepFreqMeas(char **values, int valueCount) 
{
    int i;
    unsigned long long fMin, fMax, deltaFreq, freq[MAX_NUMBER_FREQ];
    if(valueCount != 4)
    {
        Serial.println("In sweepFreqMeas, number of arguments is not correct.");
        return;  
    }
    fMin = atoi(values[1])*100ULL;
    fMax = atoi(values[2])*100ULL;
    numberFrequenciestoMeasure = atoi(values[3]);

    deltaFreq = (fMax-fMin)/numberFrequenciestoMeasure;
    












    for(i=0;i<numberFrequenciestoMeasure;i++)
    {
        freq[i]=fMin+i*deltaFreq;
        setOscillator(freq[i]);
        ADC14_enableConversion();
        while(!doneADC)
        {
            




        }
        simpleDownConverter();
        while(sendMeasurement)
        {
            



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
    if (valueCount != 2)
    {
        Serial.println(
                "In voltageMeasurement, number of arguments is not correct.");
        return;  
    }
    freq = atoi(values[1]);
    setOscillator(freq);
    ADC14_enableConversion();
    while(!doneADC)
    {}
    {
        for (j = 0; j < SAMPLE_LENGTH; j++)
        {
            Serial.print(ref[j]);
            Serial.print(", ");
            
        }
        Serial.print('\n');
        
        Serial.flush();
        delay(500);
        for (j = 0; j < SAMPLE_LENGTH; j++)
        {
            Serial.print(meas[j]);
            Serial.print(", ");
            
        }
        Serial.print('\n');
        
        Serial.flush();
    }
    Serial.print('Done sending both ref and meas.');
}

void setOscillator (unsigned long long freq)
{
    si5351.set_freq(freq, SI5351_CLK0);
    si5351.set_freq(freq+100ULL*F_IF, SI5351_CLK1);
    si5351.set_freq(freq+100ULL*F_IF, SI5351_CLK2);
    delay(1000); 
}

void sendSampleRate (char **values, int valueCount)
{
    int Fs = SAMPLE_FREQUENCY;
    int N = SAMPLE_LENGTH;
    Serial.println(Fs);
    Serial.println(N);
}



#line 1 "/home/frohro/Projects/2017_VNA/2018 Simple VNA/MultiTaskSerial.ino"







volatile extern bool sendMeasurement;
volatile extern int numberFrequenciestoMeasure, frequencyIndex;
volatile extern float  refSum, measSum;

void setupSerial()
{
  Serial.begin(115200);
}

void loopSerial()
{
   static int indexSending = -1;
   if(sendMeasurement&&(indexSending < frequencyIndex))
   {
       indexSending = frequencyIndex;
       Serial.print(refSum);
       Serial.print(", ");
       Serial.println(measSum);
       Serial.flush();
       sendMeasurement = false;
   }
}



