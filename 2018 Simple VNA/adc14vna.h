/*
 * adc14.h
 *
 *  Created on: May 2, 2017
 *      Author: frohro
 */

#ifndef ADC14VNA_H_
#define ADC14VNA_H_
#include <ti/sysbios/family/arm/m3/Hwi.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "driverlib/dma.h"
#include "driverlib/adc14.h"

#define SMCLK_FREQUENCY     12000000

#define SAMPLE_FREQUENCY    200000	 // This is limited by the SMCLK speed. We have 2 conversions  so 2*23/12Mhz = 260ksps
									 // To get more speed just re adjust the clock speeds with these lines in adc14main()
									 // However bewarned, this messes up the SPI in the si5351 library. So something in
									 // there needs changed too.
									 //
									 // /*
									 //	 * Setting up clocks
									 //	 * MCLK = MCLK = 24MHz
									 //	 * SMCLK = MCLK/2 = 24Mhz
									 //	 * ACLK = REFO = 32Khz
									 //	 */
									 //	MAP_CS_setDCOFrequency(48000000);
									 //	MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
									 //	MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_2);
									 //	MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);


#define SAMPLE_LENGTH       4096 // Cannot be lower than 1024 but needs to be a multiple of 1024 right now.

#define F_IF 155
#define OMEGA_IF F_IF*TWO_PI


int adc14_main(void);
void startConversion(void);
void ADC14_IRQHandler(void);
void DMA_INT1_IRQHandler(void);
void DMA_INT2_IRQHandler(void);

#endif /* ADC14VNA_H_ */
