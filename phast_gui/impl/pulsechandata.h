/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef PULSECHANDATA_H
#define PULSECHANDATA_H

#include "base/base_chandata.h"

class PulseChanData : public Base_ChanData
{
private:
    timestamp first_pulse;
    timestamp latest_pulse;
    timestamp last_dt;

    bool have_first_pulse;
    bool have_latest_pulse;
    bool have_last_dt;

    chaninfo ci;

public:
    explicit PulseChanData(chaninfo info, bool offline);

    virtual void ProcessEvent(timestamp macrotime);
    virtual void ProcessEvent(timestamp macrotime, timestamp microtime);

    virtual bool CalcMicrotime(timestamp macrotime, timestamp* result);
};

#endif // PULSECHANDATA_H
