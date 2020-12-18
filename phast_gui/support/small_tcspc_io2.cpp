/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "small_tcspc_io2.h"

#include <math.h>
#include <cassert>

#define N_BYTES_PER_EVENT   6
#define N_HEADER_BITS       2
#define N_BITS_PER_EVENT    (N_BYTES_PER_EVENT*8)
#define N_MACRO_BITS        (N_BITS_PER_EVENT-N_HEADER_BITS)
#define N_OVERFLOW_BITS     (N_BITS_PER_EVENT-N_HEADER_BITS)

small_tcspc_io2::~small_tcspc_io2()
{
    if (f != nullptr) {
        fclose(f);
    }
}


// The header consists of a total of 18 bytes.

small_tcspc_io2::small_tcspc_io2(QString outputfile) :
    outputfile(outputfile),
    f(nullptr),
    last_macro_timestamp(0),
    n_overflows(0),
    overflow_value((uint64_t)1 << N_MACRO_BITS),
    n_photons_written(0),
    n_bytes_written(0)
{
    this->f = fopen(outputfile.toStdString().c_str(),"wb");
    this->write_header();
}

void small_tcspc_io2::write_header()
{
    // First write the "magic" string, which is 6 bytes
    fwrite(this->MAGIC, 6, 1, this->f);

    // Now fill the rest of the header with 0xFF, to show
    // it is in use
    std::vector<uint8_t> all(12, 0xFF);

    fwrite(all.data(), 1, 12, this->f);

    this->n_bytes_written += 18;

    fflush(this->f); // Force writing data to disk
}

bool small_tcspc_io2::write_photon_event_(timestamp macrotime)
{
    uint64_t bytes = 0;

    uint64_t macro_mask = ((uint64_t)1 << N_MACRO_BITS) - 1;
    bytes += (macrotime & macro_mask) << N_HEADER_BITS;

    size_t n_written = fwrite(&bytes, N_BYTES_PER_EVENT, 1, this->f);
    fflush(this->f); // Force writing data to disk

    if (n_written == 1) {
        this->n_bytes_written += N_BYTES_PER_EVENT;
        return true;
    }

    return false;
}

bool small_tcspc_io2::write_overflow_event(uint64_t n_overflows)
{
    uint64_t bytes = 0;

    bytes += 1; // type = 1;

    uint64_t overflow_mask = ((uint64_t)1 << N_OVERFLOW_BITS) - 1;
    bytes += (n_overflows & overflow_mask) << N_HEADER_BITS;

    size_t n_written = fwrite(&bytes, N_BYTES_PER_EVENT, 1, this->f);
    fflush(this->f); // Force writing data to disk

    if (n_written == 1) {
        this->n_bytes_written += N_BYTES_PER_EVENT;
        return true;
    }

    return false;
}


bool small_tcspc_io2::write_photon_event(int64_t macrotime, int64_t)
{
    if (macrotime < this->last_macro_timestamp) {
        return false;
    }

    this->last_macro_timestamp = macrotime;

    bool success = false;

    timestamp temp_macro_t = macrotime - this->n_overflows * this->overflow_value;

    if (temp_macro_t >= this->overflow_value) {
        // We should write an overflow event
        uint64_t num_overflows = trunc((double)temp_macro_t / (double)this->overflow_value);

        assert(num_overflows > 0);

        success = this->write_overflow_event(num_overflows);

        if (!success) {
            return false;
        }

        this->n_overflows += num_overflows;
        temp_macro_t -= num_overflows * this->overflow_value;
    }

    success = this->write_photon_event_(temp_macro_t);

    if (!success) {
        return false;
    }

    n_photons_written++;

    return true;
}

uint64_t small_tcspc_io2::NumPhotonsWritten() const
{
    return this->n_photons_written;
}

uint64_t small_tcspc_io2::NumOverflows() const
{
    return this->n_overflows;
}

uint64_t small_tcspc_io2::NumBytesWritten() const
{
    return this->n_bytes_written;
}

bool small_tcspc_io2::WritesMicroTimesExplicitely() const
{
    return false;
}
