/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "base_tempdatastorage.h"

#include "../interfaces/isettingscontainer.h"
#include "../support/detectionupdate.h"
#include "impl/pulsechandata.h"
#include "impl/nonpulsechandata.h"

#include <algorithm>
#include <QDebug>
#include <cmath>
#include <iostream>
#include <mutex>
#include <QCoreApplication>

Base_TempDataStorage::~Base_TempDataStorage()
{
    this->StopThread();

    for (auto d : this->datas) {
        delete d.second;
    }
}

void Base_TempDataStorage::prepare_data_storage(const std::map<chan_id,chaninfo>& channels)
{
    // Prepare the data storage
    std::map<chan_id,PulseChanData*> pulses_data;

    for (auto pair : channels) {
        chaninfo ci = pair.second;

        if (!ci.is_pulses_channel) {
            continue;
        }

        PulseChanData* pcd = new PulseChanData(ci, this->offline);
        pulses_data[ci.ID] = pcd;
    }

    std::vector<NonPulseChanData*> non_pulses_data;

    for (auto pair : channels) {
        chaninfo ci = pair.second;

        if (ci.is_pulses_channel) {
            continue;
        }

        PulseChanData* pcd = nullptr;

        if (ci.has_pulses_channel && pulses_data.count(ci.corresponding_pulses_channel) != 0) {
            pcd = pulses_data.at(ci.corresponding_pulses_channel);
        }

        NonPulseChanData* npcd = new NonPulseChanData(ci, pcd, this->offline);
        non_pulses_data.push_back(npcd);
    }

    for (auto pair : pulses_data) {
        PulseChanData* pcd = pair.second;

        this->datas[pcd->ID()] = pcd;
    }

    for (NonPulseChanData* npcd : non_pulses_data) {
        this->datas[npcd->ID()] = npcd;
    }
}

Base_TempDataStorage::Base_TempDataStorage(LockFreeQueue<raw_photon_data> *queue) :
    offline(false),
    datas(),
    listeners(),
    always_keep_data(false),
    queue(queue),
    queue_nonraw(nullptr),
    poll_interval(10),
    is_running(false),
    listeners_mutex(),
    running_thread()
{
}

Base_TempDataStorage::Base_TempDataStorage(const std::map<chan_id,chaninfo>& channels,
                                           bool always_keep_data,
                                           bool offline,
                                           double poll_interval_milliseconds,
                                           LockFreeQueue<raw_photon_data> *queue) :
    offline(offline),
    datas(),
    listeners(),
    always_keep_data(always_keep_data),
    queue(queue),
    queue_nonraw(nullptr),
    poll_interval(poll_interval_milliseconds),
    is_running(false),
    listeners_mutex(),
    running_thread()
{
    this->prepare_data_storage(channels);
}

Base_TempDataStorage::Base_TempDataStorage(const std::map<chan_id,chaninfo>& channels,
                                           bool always_keep_data,
                                           bool offline,
                                           double poll_interval_milliseconds,
                                           LockFreeQueue<std::vector<photon_event>*> *queue_nonraw) :
    offline(offline),
    datas(),
    listeners(),
    always_keep_data(always_keep_data),
    queue(nullptr),
    queue_nonraw(queue_nonraw),
    poll_interval(poll_interval_milliseconds),
    is_running(false),
    listeners_mutex(),
    running_thread()
{
    this->prepare_data_storage(channels);
}

void Base_TempDataStorage::AddListener(IListener *value)
{
    std::lock_guard<std::mutex> l(this->listeners_mutex);

    if (std::find(this->listeners.begin(), this->listeners.end(), value) != this->listeners.end()) {
        // We already have this one.
        return;
    }

    this->listeners.push_back(value);
}

void Base_TempDataStorage::RemoveListener(IListener *value)
{
    std::lock_guard<std::mutex> l(this->listeners_mutex);

    this->listeners.erase(std::remove(this->listeners.begin(), this->listeners.end(), value), this->listeners.end());
}

void Base_TempDataStorage::loop() // Main loop
{
    while(this->is_running) {
        std::this_thread::sleep_for(this->poll_interval);

        if (this->queue != nullptr) {
            this->retrieve_data();
        } else if (this->queue_nonraw != nullptr) {
            this->retrieve_clean_data();
        }

        this->call_listeners();
        this->purge();
    }
}

struct temp_purge_request
{
public:
    IListener* listener;
    chan_data_map request;
};

void Base_TempDataStorage::call_listeners()
{
    std::lock_guard<std::mutex> lock(this->listeners_mutex);

    std::map<chan_id, chan_det_update> updates;

    // First get the iterators
    for (auto pair : this->datas) {
        IChanData* icd = pair.second;

        chan_det_update update(icd->GetMicroStart(),
                               icd->GetMicroEnd(),
                               icd->GetMicroNewDataStart(),
                               icd->GetMacroStart(),
                               icd->GetMacroEnd(),
                               icd->GetMacroNewDataStart());

        updates[icd->ID()] = update;
    }

    DetectionUpdate u(updates);

    std::vector<temp_purge_request> all_purge_requests;

    std::vector<std::thread> threads;
    std::mutex request_mutex;

    // Loop through all listeners
    for (IListener* l : this->listeners) {
        // Notify the listener
        threads.push_back(std::thread([&,l,this](){
            chan_data_map purge_requests = l->NewEvent(this, u);
            temp_purge_request tpr;
            tpr.listener = l;
            tpr.request = purge_requests;

            {
                std::lock_guard<std::mutex> l(request_mutex);
                all_purge_requests.push_back(tpr);
            }
        }));
    }

    for (auto& t : threads) {
        t.join();
    }

    for (temp_purge_request request : all_purge_requests) {
        // Notify the data holder of which data can be removed.
        for (auto pair : request.request) {
            chan_id ID = pair.first;
            data_type it = pair.second;

            this->datas.at(ID)->HandleListenerPurgeRequest(request.listener, it);
        }
    }
}

void Base_TempDataStorage::handle_new_raw_data(const raw_photon_data &data)
{
    for (uint64_t i = 0; i < data.timestamps->size(); i++) {
        if (this->datas.count(data.channels->at(i)) == 0) {
            continue;
        }

        this->datas.at(data.channels->at(i))->ProcessEvent(data.timestamps->at(i));
    }
}

void Base_TempDataStorage::retrieve_data()
{
    if (this->queue == nullptr)
        return;

    raw_photon_data e;

    while (this->queue->Consume(e)) {
        this->handle_new_raw_data(e);

        delete e.channels;
        delete e.timestamps;
    }
}

void Base_TempDataStorage::retrieve_clean_data()
{
    if (this->queue_nonraw == nullptr)
        return;

    std::vector<photon_event>* e = nullptr;;

    while (this->queue_nonraw->Consume(e)) {

        for (auto it = e->begin(); it != e->end(); it++) {
            photon_event event = *it;

            if (this->datas.count(event.channel_number) == 0) {
                continue;
            }

            IChanData* d = this->datas.at(event.channel_number);

            if (d->HavePulseChan()) {
                d->ProcessEvent(event.macrotime);
            } else {
                d->ProcessEvent(event.macrotime, event.microtime);
            }
        }

        if (e != nullptr)
            delete e;
    }
}

void Base_TempDataStorage::purge()
{
    if (this->always_keep_data)
        return;

    for (auto pair : this->datas) {
        IChanData* icd = pair.second;
        icd->purge();
    }
}

bool Base_TempDataStorage::StartThread()
{
    if (this->is_running) {
        return false;
    }

    this->running_thread = std::thread(&Base_TempDataStorage::loop, this);
    this->is_running = true;

    return true;;
}

bool Base_TempDataStorage::StopThread()
{
    if (!this->is_running)
        return false;

    this->is_running = false;
    this->running_thread.join();

    return true;
}

double Base_TempDataStorage::GetPollInterval() const
{
    return this->poll_interval.count();
}

void Base_TempDataStorage::SetPollInterval(double value)
{
    if (value <= 0)
        return;

    this->poll_interval = std::chrono::duration<double, std::milli>(value);
}
