/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef BITPACKER_H
#define BITPACKER_H

#include <stdint.h>
#include <math.h>
#include <stdio.h>

class bitpacker
{
public:
    template <class T, class U>
    static bool cpy_n_bits(T input,
                               uint64_t in_offset_start,    // inclusive
                               uint64_t in_offset_end,      // exclusive
                               uint64_t out_offset_start,   // exclusive
                               U* destination)
    {

        if (destination == NULL) {
            return false;
        }

        uint64_t counter = 0;

        for (uint64_t i = in_offset_start; i < in_offset_end; i++) {
            (*destination) += (input >> i & 1) << (out_offset_start + counter);
            counter++;
        }

        return true;
    }

    static uint64_t n_bits_required(double value)
    {
        if (value == 0)
            return 1;

        if (value < 0) {
            double temp_val = -value;
            return floor(log(temp_val) / log(2)) + 1 + 1; // One extra sign bit
        }

        return floor(log(value) / log(2)) + 1;
    }

    static uint64_t write_n_bytes(FILE* f, uint8_t value, uint64_t num)
    {
        uint64_t n = 0;

        for (uint64_t i = 0; i < num; i++) {
            n += fwrite(&value, 1, 1, f);
        }

        return n;
    }
};

#endif // BITPACKER_H
