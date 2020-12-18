/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef ICHANDATA_H
#define ICHANDATA_H

#include "../support/typedefs.h"
#include "../support/photon_event.h"
#include "../support/typedefs.h"

class IListener;

class IChanData
{
public:
    virtual ~IChanData() {}

    virtual chan_id ID() const = 0;

    virtual bool HavePulseChan() const = 0;
    virtual bool IsPulseChan() const = 0;
    virtual chan_id CorrespondingPulseChan() const = 0;

    virtual bool HaveMicrotimes() const = 0;
    virtual void ProcessEvent(timestamp macrotime) = 0;
    virtual void ProcessEvent(timestamp macrotime, timestamp microtime) = 0;
    virtual timestamp last_macro_time() const = 0;
    virtual uint64_t n_detected_events() const = 0;

    virtual data_type GetMicroStart() = 0;
    virtual data_type GetMicroEnd() = 0;
    virtual data_type GetMicroNewDataStart() = 0;

    virtual data_type GetMacroStart() = 0;
    virtual data_type GetMacroEnd() = 0;
    virtual data_type GetMacroNewDataStart() = 0;

    virtual void HandleListenerPurgeRequest(IListener* listener, data_type not_to_be_purged_it) = 0;
    virtual void purge() = 0;
};

#endif // ICHANDATA_H
