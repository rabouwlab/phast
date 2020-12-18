/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef CHANINFO_H
#define CHANINFO_H

#include <stdint.h>

struct chaninfo
{
public:
    uint64_t ID;
    bool channel_has_microtime;
    bool is_pulses_channel;
    bool has_pulses_channel;
    uint64_t num_dependent_chans;
    uint64_t corresponding_pulses_channel;
    int64_t micro_display_delaytime;
    uint64_t sync_divider;
    uint64_t additional_sync_divider;

    bool operator==(const chaninfo &other) const;

    bool operator!=(const chaninfo &other) const;

    chaninfo();

    chaninfo& operator= (const chaninfo &other);
};

#endif // CHANINFO_H
