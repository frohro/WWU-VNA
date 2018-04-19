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

#define SAMPLE_FREQUENCY    7956
#define SAMPLE_LENGTH       3840/2  // 8192 is too big.
#define F_IF 500
#define OMEGA_IF F_IF*TWO_PI
#define SAMPLES_IN_ONE_CYCLE 384 //(int)((float)SAMPLE_FREQUENCY/(float)F_IF)
/* Make sure these above are such that SAMPLE_FREQUENCY/OMEGA_IF/4 is an integer
 * so that there are an integer number of points in a quarter cycles of the IF frequency.
 */


int adc14_main(void);;

#endif /* ADC14VNA_H_ */
