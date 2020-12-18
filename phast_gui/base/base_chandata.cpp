/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "base_chandata.h"

#include <iostream>

Base_ChanData::Base_ChanData(const chaninfo &info, bool offline) :
    offline(offline),
    info(info),
    micro_data(),
    macro_data(),
    last_m_time(-1),
    n_events_detected(0),
    new_purge_round_started(false),
    n_removed_in_last_purge(0),
    purge_up_to_this_index(0),
    size_before_last_purge(0),
    listeners_purge_requests(),
    sync_div_counter(0)
{
}

bool Base_ChanData::ShouldIgnoreThisEvent()
{
    // If we are opening a file, we should process everything.
    if (this->offline) {
        return false;
    }

    sync_div_counter++;

    if (sync_div_counter < this->info.additional_sync_divider) {
        return true;
    } else {
        sync_div_counter = 0;
        return false;
    }
}
chan_id Base_ChanData::ID() const
{
    return this->info.ID;
}

timestamp Base_ChanData::last_macro_time() const
{
    return this->last_m_time;
}

uint64_t Base_ChanData::n_detected_events() const
{
    return this->n_events_detected;
}

data_type Base_ChanData::GetMacroNewDataStart()
{
    uint64_t n_survived = this->size_before_last_purge - this->n_removed_in_last_purge;

    auto ret = this->macro_data.begin() + n_survived;

    return ret;
}

data_type Base_ChanData::GetMacroStart()
{
    return this->macro_data.begin();
}

data_type Base_ChanData::GetMacroEnd()
{
    return this->macro_data.end();
}

bool Base_ChanData::HavePulseChan() const
{
    return this->info.has_pulses_channel;
}

bool Base_ChanData::IsPulseChan() const
{
    return this->info.is_pulses_channel;
}

chan_id Base_ChanData::CorrespondingPulseChan() const
{
    return this->info.corresponding_pulses_channel;
}

bool Base_ChanData::HaveMicrotimes() const
{
    return this->info.channel_has_microtime;
}

data_type Base_ChanData::GetMicroNewDataStart()
{
    if (!this->HaveMicrotimes()) {
        return this->micro_data.end();
    }

    uint64_t n_survived = this->size_before_last_purge - this->n_removed_in_last_purge;

    auto ret = this->micro_data.begin() + n_survived;

    return ret;
}

data_type Base_ChanData::GetMicroStart()
{
    if (!this->HaveMicrotimes()) {
        return this->micro_data.end();
    }

    return this->micro_data.begin();
}

data_type Base_ChanData::GetMicroEnd()
{
    return this->micro_data.end();
}

void Base_ChanData::HandleListenerPurgeRequest(IListener *listener, data_type macro_not_to_be_purged_it)
{
    int64_t purge_request_index = std::distance(this->macro_data.begin(), macro_not_to_be_purged_it);

    if (!this->new_purge_round_started ||
            purge_request_index < this->purge_up_to_this_index) {

        if (purge_request_index > static_cast<int64_t>(this->macro_data.size())) {
            // This should not happen :-/
            this->purge_up_to_this_index = static_cast<int64_t>(this->macro_data.size());
        } else {
            this->purge_up_to_this_index = purge_request_index;
        }

        this->new_purge_round_started = true;
    }

    this->listeners_purge_requests[listener] = purge_request_index;
}

void Base_ChanData::purge()
{
    this->size_before_last_purge = this->macro_data.size();

    if (this->macro_data.size() == 0) {
        this->n_removed_in_last_purge = 0;
        this->new_purge_round_started = false;
        return;
    }

    uint64_t purge_index = 0;

    if (!this->new_purge_round_started) {
        // If we heard nothing, we are trashing all data
        purge_index = this->macro_data.size() - 1;
    } else {
        purge_index = this->purge_up_to_this_index;
    }

    if (purge_index > this->macro_data.size()) {
        purge_index = this->macro_data.size();
    }

    auto purge_it = this->macro_data.begin() + purge_index;

    this->n_removed_in_last_purge = std::distance(this->macro_data.begin(), purge_it);
    this->macro_data.erase(this->macro_data.begin(), purge_it);

    if (this->HaveMicrotimes()) {
        purge_it = this->micro_data.begin() + purge_index;
        this->micro_data.erase(this->micro_data.begin(), purge_it);
    }

    this->new_purge_round_started = false;
}
