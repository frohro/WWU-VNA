#include "Energia.h"

#line 1 "/home/frohro/workspace_v7/Testing Si5351a/Testing Si5351a.ino"
#include "si5351.h"
#include "Wire.h"
void setup();
void loop();

#line 3
extern "C"{
#include "adc14vna.h"
};
#include "driverlib.h"


volatile float ref[SAMPLE_LENGTH];
volatile float meas[SAMPLE_LENGTH];
volatile uint16_t ref_uint16_t[SAMPLE_LENGTH];
volatile uint16_t meas_uint16_t[SAMPLE_LENGTH];
volatile unsigned short ref_ushort[SAMPLE_LENGTH];
volatile unsigned short meas_ushort[SAMPLE_LENGTH];
extern volatile bool doneADC;

Si5351 si5351;

void setup() {
  
    si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0); 
    si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA);
    si5351.set_freq(200000000ULL, SI5351_CLK0);
    si5351.set_freq(200020000ULL, SI5351_CLK1);
    si5351.set_freq(200020000ULL, SI5351_CLK2);
    adc14_main(); 
    ADC14_enableConversion();
            while(1)
            {
                if(doneADC){
                        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
                        
                    doneADC = false;
                }

                




            }
}

void loop() {
  
  
}



