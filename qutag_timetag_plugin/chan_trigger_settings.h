/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef CHAN_TRIGGER_SETTINGS_H
#define CHAN_TRIGGER_SETTINGS_H

#include <stdint.h>

struct chan_trigger_settings
{
public:
    enum trigger_edge {
        FALLING = 0,
        RISING = 1,
    };

    uint64_t ID;
    bool signal_conditioning_enabled;
    trigger_edge edge;
    //bool terminate_in_signal_path;
    double voltage_threshold;
    uint64_t sync_divider;

    int64_t delay_time;

    chan_trigger_settings() :
        ID(0),
        signal_conditioning_enabled(true),
        edge(RISING),
        //terminate_in_signal_path(true),
        voltage_threshold(-0.4),
        sync_divider(1),
        delay_time(0)
    {
    }
};

#endif // CHAN_TRIGGER_SETTINGS_H
