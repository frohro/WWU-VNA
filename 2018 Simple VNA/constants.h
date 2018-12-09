/*
 * constants.h
 *
 *  Created on: Dec 6, 2018
 *      Author: JD
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_
#include <cstdint>


#define round(x) (x < 0.0) ? static_cast<int>(x - 0.5) : static_cast<int>(x + 0.5)
#define COMPUTATION_TYPE float
#define PI 3.1415926535897932384626433832795
#define TWO_PI 6.283185307179586476925286766559

constexpr const int DMA_Block = 1024;

constexpr const int SAMPLE_LENGTH = 4 * DMA_Block;

constexpr const int TIME_SAMPLE_LENGTH = 4 * DMA_Block;

constexpr const int F_IF = 155;
constexpr const float OMEGA_IF = F_IF*TWO_PI;


const int SAMPLE_FREQUENCY = 53000;     // This is limited by the SMCLK speed. We have 2 conversions  so 2*23/12Mhz = 260ksps
                                        // To get more speed just re adjust the clock speeds with these lines in adc14main()
                                        // However bewarned, this messes up the SPI in the si5351 library. So something in
                                        // there needs changed too.
                                        //
                                        // /*
                                        //  * Setting up clocks
                                        //  * MCLK = MCLK = 24MHz
                                        //  * SMCLK = MCLK/2 = 24Mhz
                                        //  * ACLK = REFO = 32Khz
                                        //  */
                                        // MAP_CS_setDCOFrequency(48000000);
                                        // MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
                                        // MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_2);
                                        // MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);


// Bin size
constexpr const float df = static_cast<float>(SAMPLE_FREQUENCY) / static_cast<float>(SAMPLE_LENGTH);

// k = F_IF / df but it has to be an integer, and close to a number eg 2.1 rather than 2.5
const int BIN_INDEX = round(F_IF / df);


struct computation {
    COMPUTATION_TYPE real = 0.;
    COMPUTATION_TYPE imag = 0.;
};

// Globals

extern volatile bool TIME_MEASUREMENT;
extern volatile std::uint16_t ref[TIME_SAMPLE_LENGTH];
extern volatile std::uint16_t meas[TIME_SAMPLE_LENGTH];

extern volatile bool doneADC;
// Sample currently being taken
extern volatile int sampleCount;
extern volatile bool doneConv;


#endif /* CONSTANTS_H_ */
