#include "constants.h"
/*
 * constants.cpp
 *
 *  Created on: Dec 6, 2018
 *      Author: JD
 */


volatile bool TIME_MEASUREMENT = false;
volatile std::uint16_t ref[TIME_SAMPLE_LENGTH];
volatile std::uint16_t meas[TIME_SAMPLE_LENGTH];


extern volatile bool doneADC = false;
extern volatile int sampleCount = 0;
extern volatile bool doneConv = false;
