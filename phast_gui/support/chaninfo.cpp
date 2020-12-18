/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "chaninfo.h"


bool chaninfo::operator==(const chaninfo &other) const {
    if (other.ID == this->ID &&
        other.channel_has_microtime == this->channel_has_microtime &&
        other.is_pulses_channel == this->is_pulses_channel &&
        other.has_pulses_channel == this->has_pulses_channel &&
            other.corresponding_pulses_channel == this->corresponding_pulses_channel &&
            other.sync_divider == this->sync_divider &&
            other.additional_sync_divider == this->additional_sync_divider) {
        return true;
    }

    return false;
}

bool chaninfo::operator!=(const chaninfo &other) const {
    return !(*this == other);
}

/*
chaninfo::chaninfo(const chaninfo& other) :
    ID(other.ID),
    channel_has_microtime(other.channel_has_microtime),
    is_pulses_channel(other.is_pulses_channel),
    has_pulses_channel(other.has_pulses_channel),
    num_dependent_chans(other.num_dependent_chans),
    corresponding_pulses_channel(other.corresponding_pulses_channel),
    micro_display_delaytime(other.micro_display_delaytime),
    sync_divider(other.sync_divider),
    additional_sync_divider(other.additional_sync_divider)
{
}
*/

chaninfo::chaninfo() :
    ID(0),
    channel_has_microtime(false),
    is_pulses_channel(false),
    has_pulses_channel(false),
    num_dependent_chans(0),
    corresponding_pulses_channel(0),
    micro_display_delaytime(0),
    sync_divider(1),
    additional_sync_divider(1)
{
}

/*
chaninfo::chaninfo(uint64_t ID) :
    ID(ID),
    channel_has_microtime(false),
    is_pulses_channel(false),
    has_pulses_channel(false),
    num_dependent_chans(0),
    corresponding_pulses_channel(0),
    micro_display_delaytime(0),
    sync_divider(1),
    additional_sync_divider(1)
{
}
*/
chaninfo& chaninfo::operator= (const chaninfo &other)
{
    this->ID = other.ID;
    this->is_pulses_channel = other.is_pulses_channel;
    this->channel_has_microtime = other.channel_has_microtime;
    this->has_pulses_channel = other.has_pulses_channel;
    this->num_dependent_chans = other.num_dependent_chans;
    this->corresponding_pulses_channel = other.corresponding_pulses_channel;
    this->micro_display_delaytime = other.micro_display_delaytime;
    this->sync_divider = other.sync_divider;
    this->additional_sync_divider = other.additional_sync_divider;

    return *this;
}
