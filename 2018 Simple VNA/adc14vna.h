/*
 * adc14.h
 *
 *  Created on: May 2, 2017
 *      Author: frohro
 */

#ifndef ADC14VNA_H_
#define ADC14VNA_H_
#include <ti/sysbios/family/arm/m3/Hwi.h>

#define SMCLK_FREQUENCY     12000000

#define SAMPLE_FREQUENCY    192000
#define SAMPLE_LENGTH       4096  // 8192 is too big.

int adc14_main(void);;

#endif /* ADC14VNA_H_ */
