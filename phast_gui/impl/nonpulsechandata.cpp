/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "nonpulsechandata.h"

#include "pulsechandata.h"

#include <limits.h>
#include <stdint.h>

NonPulseChanData::NonPulseChanData(chaninfo info, PulseChanData *pulse_channel, bool offline) :
    Base_ChanData(info, offline),
    pulse_chan(pulse_channel)
{
}

void NonPulseChanData::ProcessEvent(timestamp macrotime)
{
    if (this->ShouldIgnoreThisEvent())
        return;

    if (this->pulse_chan != nullptr) {
        // Get the micro time
        timestamp microtime = 0;

        bool success = this->pulse_chan->CalcMicrotime(macrotime, &microtime);

        if (!success) {
            // We could not calculate a valid micro time.
            // We cannot do nothing, so we save a bogus time.
            this->micro_data.push_back(INT64_MIN);
        } else {
            this->micro_data.push_back(microtime);
        }
    }

    // Save the event.
    this->last_m_time = macrotime;

    this->macro_data.push_back(macrotime);
    this->n_events_detected++;
}

void NonPulseChanData::ProcessEvent(timestamp macrotime, timestamp microtime)
{
    if (this->ShouldIgnoreThisEvent())
        return;

    if (this->HaveMicrotimes()) {
        this->micro_data.push_back(microtime);
    }

    // Save the event.
    this->last_m_time = macrotime;

    this->macro_data.push_back(macrotime);
    this->n_events_detected++;
}
