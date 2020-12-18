/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "pulsechandata.h"

#include <math.h>

PulseChanData::PulseChanData(chaninfo info, bool offline) :
    Base_ChanData(info, offline),
    first_pulse(-1),
    latest_pulse(-1),
    last_dt(-1),
    have_first_pulse(false),
    have_latest_pulse(false),
    have_last_dt(false),
    ci(info)
{
}

void PulseChanData::ProcessEvent(timestamp macrotime)
{
    if (this->ShouldIgnoreThisEvent())
        return;

    // If we did not have the very first pulse yet,
    // we assign it here
    if (!this->have_first_pulse) {
        this->first_pulse = macrotime;
        this->have_first_pulse = true;
    }

    // If this is not the very first pulse,
    // then we can start determining delta times
    if (this->have_latest_pulse) {
        this->last_dt = macrotime - this->latest_pulse;
        this->have_last_dt = true;
    }

    // Now set the latest pulse to equal the current one:
    this->latest_pulse = macrotime;
    this->have_latest_pulse = true;

    // Save the event. Maybe not store the whole event?
    this->last_m_time = macrotime;

    this->macro_data.push_back(macrotime);

    this->n_events_detected++;
}

void PulseChanData::ProcessEvent(timestamp macrotime, timestamp microtime)
{
    this->ProcessEvent(macrotime);
}

bool PulseChanData::CalcMicrotime(timestamp macrotime, timestamp *result)
{
    if (!this->have_last_dt) {
        return false;
    }

    if (result == nullptr) {
        return false;
    }

    // We calculate the micro time as follows:
    //      - we determine the time difference between the last pulse, and the target time event
    //         for a sync divider of one, this would directly give us the micro time.
    //      - we then correct for the sync divider
    //         to do so, we take into account the last time between two pulses (last_dt).
    //         we divide this value by the sync divider, to obtain the 'average' time between
    //         laser pulses. We use here the time difference between the last two pulses, because
    //         this may not be constant throughout the experiment.
    //
    //         Finally, we take the modulus of the delta time with the thus-obtained pulse period.
    //         This gives the micro time, corrected for the sync divider and possible variations
    //         in the pulse period.
    timestamp dt = macrotime - this->latest_pulse;
    double div = (double)this->last_dt / (double)(this->ci.sync_divider * this->ci.additional_sync_divider);
    double rem = fmod((double)dt, div) ;

    *result = (uint64_t) rem;

    return true;
}
