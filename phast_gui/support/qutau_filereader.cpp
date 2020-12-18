/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "qutau_filereader.h"

#include "chaninfo.h"

#include  <stdio.h>
#include <stdint.h>
#include <iostream>
#include "raw_photon_data.h"

/*
    read_quTau function

    Parameters:
        filepath                The filepath to the input file.
        time_unit_in_seconds    The time unit to use for the data, in seconds.
                                E.g. 1e-9 for a clock of 1 ns.
    Output:
        A timetagged_file struct, containing the processed file.

    Note:
        This function was created based on the documentation from:

        [1]: "quTAU quPSI Manual v4.0", qutools, 2016, pp 25
*/
bool quTAU_filereader::read_quTau(const char *filepath,
                                  bool trash_useless_pulses,
                                  std::map<uint64_t, chaninfo> *chaninfos,
                                  chan_dat_holder *datas,
                                  int64_t pulses_chan,
                                  double time_unit_in_seconds)
{
    if (chaninfos == nullptr || datas == nullptr) {
        return false;
    }

    FILE* f = fopen(filepath, "rb");

    // If f equals nullptr, we could not open the input file
    // TODO: implement proper error handling
    if (f == nullptr) {
        return false;
    }

    // We skip the first 40 bytes. These contain some information,
    // but I'm not sure what it means. The manual is not very clear
    // on this either.
    fseek(f, 40, 0);

    size_t n_items_read = 1;

    int64_t t_min = INT64_MAX;
    int64_t t_max = 0;

    photon_event prev_event;
    prev_event.macrotime = -2;

    uint64_t n_bytes_read = 39;
    uint64_t counter = 0;

    // Read until end of file
    while (n_items_read != 0) {
        counter++;

        int64_t timestamp = 0;
        int16_t channel = 0;

        // Note: we are assuming a filetype of 10 bytes / event.
        // TODO: implement a system that recognizes the filetype
        n_items_read = fread(&timestamp, 8, 1, f);
        n_items_read = fread(&channel, 2, 1, f);

        n_bytes_read += 10;

        if (timestamp == 0) {
            continue;
        }

        if (timestamp < t_min)
            t_min = timestamp;

        if (timestamp > t_max)
            t_max = timestamp;

        if (datas->count(channel) == 0) {
            chaninfo ci2;
            ci2.ID = channel;
            ci2.is_pulses_channel = (channel == pulses_chan) ? true : false;
            ci2.has_pulses_channel = (!ci2.is_pulses_channel && pulses_chan != -1) ? true : false;
            ci2.channel_has_microtime = ci2.has_pulses_channel;
            ci2.corresponding_pulses_channel = (ci2.has_pulses_channel) ? pulses_chan : 0;
            ci2.num_dependent_chans = 0;

            chaninfos->insert(std::make_pair(channel, ci2));
        }

        photon_event e;
        e.channel_number = channel;
        e.macrotime = timestamp;
        e.microtime = 0;

        if (trash_useless_pulses && e.channel_number == (chan_id)pulses_chan && prev_event.channel_number == e.channel_number) {
            // Do nothing
        } else {
            (*datas)[prev_event.channel_number].push_back(prev_event);
        }

        prev_event = e;

        if (counter % 500000 == 0) {
            emit signalProgress(n_bytes_read);
        }
    }

    (*datas)[prev_event.channel_number].push_back(prev_event);

    fclose(f);

    std::cout << "done reading file!" << std::endl;

    emit finished();

    return true;
}

/*
 * std::vector<int64_t>* timestamps;
    std::vector<uint8_t>* channels;
    */

bool quTAU_filereader::read_quTau(const char *filepath,
                                  bool trash_useless_pulses,
                                  std::map<uint64_t, chaninfo> *chaninfos,
                                  raw_photon_data* data,
                                  int64_t pulses_chan,
                                  double time_unit_in_seconds)
{
    if (chaninfos == nullptr || data == nullptr) {
        return false;
    }

    FILE* f = fopen(filepath, "rb");

    // If f equals nullptr, we could not open the input file
    // TODO: implement proper error handling
    if (f == nullptr) {
        return false;
    }

    data->channels = new std::vector<uint8_t>();
    data->timestamps = new std::vector<int64_t>();

    // We skip the first 40 bytes. These contain some information,
    // but I'm not sure what it means. The manual is not very clear
    // on this either.
    fseek(f, 40, 0);

    size_t n_items_read = 1;

    int64_t t_min = INT64_MAX;
    int64_t t_max = 0;

    photon_event prev_event;
    prev_event.macrotime = -2;

    uint64_t n_bytes_read = 39;
    uint64_t counter = 0;

    // Read until end of file
    while (n_items_read != 0) {
        counter++;

        int64_t timestamp = 0;
        int16_t channel = 0;

        // Note: we are assuming a filetype of 10 bytes / event.
        // TODO: implement a system that recognizes the filetype
        n_items_read = fread(&timestamp, 8, 1, f);
        n_items_read = fread(&channel, 2, 1, f);

        n_bytes_read += 10;

        if (timestamp == 0) {
            continue;
        }

        if (timestamp < t_min)
            t_min = timestamp;

        if (timestamp > t_max)
            t_max = timestamp;

        if (chaninfos->count(channel) == 0) {
            chaninfo ci2;

            ci2.ID = channel;
            ci2.is_pulses_channel = (channel == pulses_chan) ? true : false;
            ci2.has_pulses_channel = (!ci2.is_pulses_channel && pulses_chan != -1) ? true : false;
            ci2.channel_has_microtime = ci2.has_pulses_channel;
            ci2.corresponding_pulses_channel = (ci2.has_pulses_channel) ? pulses_chan : 0;
            ci2.num_dependent_chans = 0;

            chaninfos->insert(std::make_pair(channel, ci2));
        }

        photon_event e;
        e.channel_number = channel;
        e.macrotime = timestamp;
        e.microtime = 0;

        if (trash_useless_pulses && e.channel_number == pulses_chan && prev_event.channel_number == e.channel_number) {
            // Do nothing
        } else if (prev_event.channel_number != -2) {
            data->channels->push_back(prev_event.channel_number);
            data->timestamps->push_back(prev_event.macrotime);
        }

        prev_event = e;

        if (counter % 500000 == 0) {
            emit signalProgress(n_bytes_read);
        }
    }

    if (prev_event.channel_number != -2) {
        data->channels->push_back(prev_event.channel_number);
        data->timestamps->push_back(prev_event.macrotime);
    }

    fclose(f);

    std::cout << "done reading file!" << std::endl;

    emit finished();

    return true;
}
