/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef BASE_CHANDATA_H
#define BASE_CHANDATA_H

#include "../interfaces/ichandata.h"
#include "../support/chaninfo.h"
#include "../support/typedefs.h"
#include "../support/global.h"

class IChanDataProvider;

class TT_EXPORT Base_ChanData : public IChanData
{
private:
    bool offline;

protected:
    chaninfo info;
    data_holder micro_data;
    data_holder macro_data;

    timestamp last_m_time;
    uint64_t n_events_detected;

    bool new_purge_round_started;

    uint64_t n_removed_in_last_purge;
    int64_t purge_up_to_this_index;
    uint64_t size_before_last_purge;
    std::map<IListener*,index> listeners_purge_requests;

    uint64_t sync_div_counter;
    bool ShouldIgnoreThisEvent();

public:
    explicit Base_ChanData(const chaninfo& info, bool offline);

    virtual bool HavePulseChan() const override;
    virtual bool IsPulseChan() const override;
    virtual chan_id CorrespondingPulseChan() const override;

    virtual bool HaveMicrotimes() const override;
    virtual chan_id ID() const override;
    virtual void ProcessEvent(timestamp macrotime) override = 0;
    virtual timestamp last_macro_time() const override;
    virtual uint64_t n_detected_events() const override;

    virtual data_type GetMicroStart() override;
    virtual data_type GetMicroEnd() override;
    virtual data_type GetMicroNewDataStart() override;

    virtual data_type GetMacroStart() override;
    virtual data_type GetMacroEnd() override;
    virtual data_type GetMacroNewDataStart() override;

    virtual void HandleListenerPurgeRequest(IListener* listener, data_type macro_not_to_be_purged_it) override;
    virtual void purge() override;
};

#endif // BASE_CHANDATA_H
