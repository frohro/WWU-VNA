#include "Energia.h"

#line 1 "/home/frohro/workspace_v7/2017 VNA/vna_2017.ino"
#include "Arduino.h"
#include "Wire.h"
#include "si5351.h"
#include "quadrature.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "DynamicCommandParser.h"

void setup();
void loop();
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

volatile uint16_t refRe[SAMPLE_LENGTH];
volatile uint16_t refIm[SAMPLE_LENGTH];
volatile uint16_t measRe[SAMPLE_LENGTH];
volatile uint16_t measIm[SAMPLE_LENGTH];
extern volatile bool doneADC;
volatile bool sendMeasurement = false;
volatile int numberFrequenciestoMeasure, frequencyIndex;
volatile float  refSum, measSum;


float shift[SAMPLE_LENGTH];  

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
                *0.5*(1-cos(2*PI*n/(SAMPLE_LENGTH-1))); 
    }

    
    DynamicCommandParser dcp('^', '$', ',');

    
    
    si5351.update_status();
    ADC14_enableConversion(); 
    delay(500);
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
    refSum = refReSum + refImSum;  
    measSum = measReSum + measImSum;
    return(1);  
}

void processCommand(char **values, int valueCount)
{
    int i;
    unsigned long long fMin, fMax, deltaFreq, freq[MAX_NUMBER_FREQ];
    if(valueCount != 4)
    {
        return;  
    }
    fMin = atoi(values[0])*100ULL;
    fMax = atoi(values[1])*100ULL;
    numberFrequenciestoMeasure = atoi(values[2]);

    deltaFreq = (fMax-fMin)/numberFrequenciestoMeasure;
    




    for(i=0;i<numberFrequenciestoMeasure;i++)
    {
        freq[i]=(fMin+i*deltaFreq);
        si5351.set_freq(freq[i], SI5351_CLK0);
        si5351.set_freq(freq[i]+100ULL*F_IF, SI5351_CLK1);
        si5351.set_freq(freq[i]+100ULL*F_IF, SI5351_CLK2);
        
        delay(1000);
        startSampling();
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


#line 1 "/home/frohro/workspace_v7/2017 VNA/MultiTaskSerial.ino"
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
       Serial.print(refSum,0); 
       Serial.print(", ");
       Serial.println(measSum,0);
       Serial.flush();
       sendMeasurement = false;
   }




}



