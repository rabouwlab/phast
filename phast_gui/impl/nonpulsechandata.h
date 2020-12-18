/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef NONPULSECHANDATA_H
#define NONPULSECHANDATA_H

#include "base/base_chandata.h"

class PulseChanData;

class NonPulseChanData : public Base_ChanData
{
private:
    PulseChanData* pulse_chan;

public:
    NonPulseChanData(const NonPulseChanData&) = delete;
    NonPulseChanData& operator=(const NonPulseChanData&) = delete;
    NonPulseChanData(chaninfo info, PulseChanData* pulse_channel, bool offline);

    virtual void ProcessEvent(timestamp macrotime);
    virtual void ProcessEvent(timestamp macrotime, timestamp microtime);
};

#endif // NONPULSECHANDATA_H
