/*
 * adc14.h
 *
 *  Created on: May 2, 2017
 *      Author: frohro
 */

#ifndef ADC14VNA_H_
#define ADC14VNA_H_
#include <ti/sysbios/family/arm/m3/Hwi.h>

#define SMCLK_FREQUENCY     24000000

#define SAMPLE_FREQUENCY    8000
#define SAMPLE_LENGTH       128

int adc14_main(void);
void startSampling(void);

#endif /* ADC14VNA_H_ */
