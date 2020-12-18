/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "small_tcspc_io_reader.h"
#include "../support/bitpacker.h"

#include <iostream>

small_tcspc_io_reader::~small_tcspc_io_reader()
{
    if (this->f != nullptr) {
        fclose(this->f);
    }
}
small_tcspc_io_reader::small_tcspc_io_reader(QString filename, chan_id channel, std::vector<photon_event> *data_out) :
    f(nullptr),
    data_out(data_out),
    n_overflows(0),
    channel(channel),
    done_reading(false)
{
    std::cout << filename.toStdString() << std::endl;

    f = fopen(filename.toStdString().c_str(), "rb");

    this->read_data();
}

bool small_tcspc_io_reader::read_event(event *data)
{
    uint64_t bytes = 0;

    size_t n_read = fread(&bytes, 8, 1, this->f);

    if (n_read != 1) {
        return false;
    }

    uint64_t event_type_mask = ((uint64_t)1 << 2) - 1;
    uint64_t n_overflows_mask = ((uint64_t)1 << 62) - 1;
    uint64_t micro_mask = ((uint64_t)1 << 34) - 1;
    uint64_t macro_mask = ((uint64_t)1 << 28) - 1;
    uint64_t type_of_event = bytes & event_type_mask;


    if (type_of_event == 1) { // overflow
        *data = event((bytes >> 2) & n_overflows_mask);
        return true;
    } else if (type_of_event == 0) { // photon
        *data = event((bytes >> (2 + 34)) & macro_mask,
                      (bytes >> 2) & micro_mask);
        return true;
    }

    return false;
}

void small_tcspc_io_reader::read_data()
{
    if (this->done_reading)
        return;

    bool read_success = true;

    while (read_success) {
        event new_data(0);

        read_success = this->read_event(&new_data);

        if (!read_success)
            break;

        // Handle overflows
        if (new_data.type == PHOTON_OVERFLOW) {
            this->n_overflows += new_data.n_overflows;
            continue;
        } else if (new_data.type == PHOTON) {
            photon_event e;
            e.channel_number = this->channel;
            e.microtime = new_data.microtime;
            e.macrotime = this->n_overflows * ((uint64_t)1 << 28) + new_data.macrotime;

            this->data_out->push_back(e);
            continue;
        } else {
            break;
        }
    }

    this->done_reading = true;
}
