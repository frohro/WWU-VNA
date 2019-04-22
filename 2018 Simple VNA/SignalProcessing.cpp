#include "SignalProcessing.h"

/*
 * SignalProcessing.cpp
 *
 *  Created on: Dec 6, 2018
 *      Author: JD
 */

constexpr COMPUTATION_TYPE hanning_window(int n, int total) {
    return std::sin(PI * n / (total - 1)) * sin(PI * n / (total - 1));
}

constexpr COMPUTATION_TYPE real_filter(int n, int total) {
   return std::cos(TWO_PI * n * BIN_INDEX / total) * hanning_window(n, total);
}


constexpr COMPUTATION_TYPE imag_filter(int n, int total) {
    return -(std::sin(TWO_PI * n * BIN_INDEX / total) * hanning_window(n, total));
}

void compute_block(computation &c, int index, uint16_t * block, size_t length) {
    for (auto i = 0; i < length; ++i) {
        c.real += static_cast<COMPUTATION_TYPE>(lut.values[i + index * DMA_Block][0] * static_cast<COMPUTATION_TYPE>(block[i]));
        c.imag += static_cast<COMPUTATION_TYPE>(lut.values[i + index * DMA_Block][1] * static_cast<COMPUTATION_TYPE>(block[i]));
    }
}

// Not sure if this does anything, but what I'm hoping to do here is store the lut to flash
// Which is being done currently...
#pragma DATA_SECTION(".lut")
#pragma RETAIN
const Table<SAMPLE_LENGTH> lut = Table<SAMPLE_LENGTH>();
