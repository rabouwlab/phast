/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef BASE_TEMPDATASTORAGE_H
#define BASE_TEMPDATASTORAGE_H

#include <thread>
#include <mutex>

#include "../interfaces/itempdatastorage.h"
#include "../support/lockfreequeue.h"
#include "../interfaces/ilistener.h"
#include "../support/raw_photon_data.h"
#include "../support/chaninfo.h"
#include "../interfaces/ichandata.h"
#include "../support/global.h"

class ISettingsContainer;

class TT_EXPORT Base_TempDataStorage : public ITempDataStorage
{
private:
    bool offline;
    std::map<chan_id,IChanData*> datas;
    std::vector<IListener*> listeners;
    bool always_keep_data;
    LockFreeQueue<raw_photon_data>* queue;
    LockFreeQueue<std::vector<photon_event>*>* queue_nonraw;
    std::chrono::duration<double, std::milli> poll_interval;
    bool is_running;
    std::mutex listeners_mutex;
    std::thread running_thread;

    void retrieve_data();
    void purge();
    void loop();
    void call_listeners();
    void handle_new_raw_data(const raw_photon_data& data);
    void handle_new_photon_event(photon_event &e);
    void prepare_data_storage(const std::map<chan_id,chaninfo>& channels);

    void retrieve_clean_data();

public:
    Base_TempDataStorage(const Base_TempDataStorage&) = delete;
    Base_TempDataStorage& operator=(const Base_TempDataStorage&) = delete;

    ~Base_TempDataStorage();

    explicit Base_TempDataStorage(LockFreeQueue<raw_photon_data>* queue);

    Base_TempDataStorage(const std::map<chan_id, chaninfo> &channels,
                         bool always_keep_data,
                         bool offline,
                         double poll_interval_milliseconds,
                         LockFreeQueue<raw_photon_data>* queue);

    Base_TempDataStorage(const std::map<chan_id, chaninfo> &channels,
                         bool always_keep_data,
                         bool offline,
                         double poll_interval_milliseconds,
                         LockFreeQueue<std::vector<photon_event>*>* queue_nonraw);

    virtual double GetPollInterval() const override;
    virtual void SetPollInterval(double value) override;
    virtual bool StartThread() override;
    virtual bool StopThread() override;
    virtual void AddListener(IListener* value) override;
    virtual void RemoveListener(IListener* value) override;
};

#endif // BASE_TEMPDATASTORAGE_H
