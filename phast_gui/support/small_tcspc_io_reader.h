/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef SMALL_TCSPC_IO_READER_H
#define SMALL_TCSPC_IO_READER_H

#include <QString>
#include <stdio.h>
#include <vector>

#include "../support/photon_event.h"

class small_tcspc_io_reader
{
private:
    enum event_type
    {
        PHOTON = 0,
        PHOTON_OVERFLOW = 1,
    };

    struct event
    {
        event_type type;
        timestamp macrotime;
        timestamp microtime;
        uint64_t n_overflows;

        event(timestamp macrotime, timestamp microtime) :
            type(PHOTON),
            macrotime(macrotime),
            microtime(microtime),
            n_overflows(0)
        {
        }

        explicit event(uint64_t n_overflows) :
            type(PHOTON_OVERFLOW),
            macrotime(0),
            microtime(0),
            n_overflows(n_overflows)
        {
        }
    };

private:
    FILE* f;

    std::vector<photon_event>* data_out;

    uint64_t n_overflows;
    chan_id channel;
    bool done_reading;

    void read_data();
    bool read_event(event* data);

public:
    small_tcspc_io_reader(const small_tcspc_io_reader&) = delete;
    small_tcspc_io_reader& operator=(const small_tcspc_io_reader&) = delete;
    ~small_tcspc_io_reader();
    small_tcspc_io_reader(QString filename, chan_id channel, std::vector<photon_event>* data_out);
};

#endif // SMALL_TCSPC_IO_READER_H
