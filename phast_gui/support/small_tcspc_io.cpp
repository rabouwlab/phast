/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "small_tcspc_io.h"

#include <math.h>

small_tcspc_io::~small_tcspc_io()
{
    if (f != nullptr) {
        fclose(f);
    }
}

small_tcspc_io::small_tcspc_io(QString outputfile) :
    outputfile(outputfile),
    f(nullptr),
    last_macro_timestamp(0),
    n_overflows(0),
    overflow_value((uint64_t)1 << 28),
    overflow_value_micro((uint64_t)1 << 34),
    n_photons_written(0),
    n_bytes_written(0)
{
    this->f = fopen(outputfile.toStdString().c_str(),"wb");
}

bool small_tcspc_io::write_photon_event_(timestamp macrotime, timestamp microtime)
{
    uint64_t bytes = 0;

    uint64_t micro_mask = ((uint64_t)1 << 34) - 1;
    bytes += (microtime & micro_mask) << 2;

    uint64_t macro_mask = ((uint64_t)1 << 28) - 1;
    bytes += (macrotime & macro_mask) << (34 + 2);

    size_t n_written = fwrite(&bytes, 8, 1, this->f);

    if (n_written == 1) {
        this->n_bytes_written += 8;
        return true;
    }

    return false;
}

bool small_tcspc_io::write_overflow_event(uint64_t n_overflows)
{
    uint64_t bytes = 0;

    bytes += 1; // type = 1;

    uint64_t overflow_mask = ((uint64_t)1 << 62) - 1;
    bytes += (n_overflows & overflow_mask) << 2;

    size_t n_written = fwrite(&bytes, 8, 1, this->f);

    if (n_written == 1) {
        this->n_bytes_written += 8;
        return true;
    }

    return false;
}


bool small_tcspc_io::write_photon_event(int64_t macrotime, int64_t microtime)
{
    if (macrotime < this->last_macro_timestamp) {
        return false;
    }

    if (microtime >= this->overflow_value_micro) {
        return false;
    }

    this->last_macro_timestamp = macrotime;

    bool success = false;

    timestamp temp_macro_t = macrotime - this->n_overflows * this->overflow_value;

    if (temp_macro_t >= this->overflow_value) {
        // We should write an overflow event
        uint64_t num_overflows = trunc((double)temp_macro_t / (double)this->overflow_value);
        success = this->write_overflow_event(num_overflows);

        if (!success) {
            return false;
        }

        this->n_overflows += num_overflows;
        temp_macro_t -= num_overflows * this->overflow_value;
    }


    success = this->write_photon_event_(temp_macro_t, microtime);

    if (!success) {
        return false;
    }

    n_photons_written++;

    return true;
}

uint64_t small_tcspc_io::NumPhotonsWritten() const
{
    return this->n_photons_written;
}

uint64_t small_tcspc_io::NumOverflows() const
{
    return this->n_overflows;
}

uint64_t small_tcspc_io::NumBytesWritten() const
{
    return this->n_bytes_written;
}

bool small_tcspc_io::WritesMicroTimesExplicitely() const
{
    return true;
}
