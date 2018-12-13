/*
 * SignalProcessing.h
 *
 *  Created on: Dec 6, 2018
 *      Author: JD
 */

#ifndef SIGNALPROCESSING_H_
#define SIGNALPROCESSING_H_
#include <cmath>
#include "constants.h"
#include <cstdint>

constexpr COMPUTATION_TYPE hanning_window(int n, int total);

constexpr COMPUTATION_TYPE real_filter(int n, int total);

constexpr COMPUTATION_TYPE imag_filter(int n, int total);

template<int N>
struct Table
{
    constexpr Table() : values()
    {
        for (auto i = 0; i < N; ++i)
        {
            values[i][0] = real_filter(i, N);
            values[i][1] = imag_filter(i, N);
        }
    }
    COMPUTATION_TYPE values[N][2];
};

extern const Table<SAMPLE_LENGTH> lut;

void compute_block(computation &c, int index, uint16_t * block, size_t length);

#endif /* SIGNALPROCESSING_H_ */
