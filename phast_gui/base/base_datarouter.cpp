/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "base_datarouter.h"

#include <algorithm>
#include <chrono>
#include <thread>
#include <iostream>

#include "../support/lockfreequeue.h"

#include "../support/detectionupdate.h"
#include "../interfaces/itimetaggercommunicator.h"

Base_DataRouter::~Base_DataRouter()
{
    if (this->is_running) {
        this->is_running = false;
        this->running_thread.join();
    }    
}

Base_DataRouter::Base_DataRouter(ITimeTaggerCommunicator *tt_comm,
                                 LockFreeQueue<raw_photon_data> *storage,
                                 double poll_interval) :
    tt_comm(tt_comm),
    storage(storage),
    poll_interval(poll_interval),
    is_running(false),
    running_thread()
{
}

void Base_DataRouter::loop()
{
    while(this->is_running) {
        std::this_thread::sleep_for(this->poll_interval);

        raw_photon_data data;
        data.channels = new std::vector<uint8_t>();
        data.timestamps = new std::vector<int64_t>();

        this->tt_comm->ReceiveData(data.timestamps, data.channels);

        // Check for data loss
        bool dataloss = this->tt_comm->DataLossSinceLastCall();

        if (dataloss) {
            std::cout << "WARNING: time tagger reports data loss. Your data may be skewed by artefacts." << std::endl;
            std::cout << "This may be due to time-tagger saturation, e.g. in cases of high count rates, and" << std::endl;
            std::cout << "especially when the time-tagger has to process high rep-rate sync signals (>=10MHz)" << std::endl;

            std::cerr << "WARNING: time tagger reports data loss. Your data may be skewed by artefacts." << std::endl;
            std::cerr << "This may be due to time-tagger saturation, e.g. in cases of high count rates, and" << std::endl;
            std::cerr << "especially when the time-tagger has to process high rep-rate sync signals (>=10MHz)" << std::endl;
        }
        if (data.timestamps->size() != data.channels->size()) {
            std::cout << "Retrieved data is malformed :-(" << std::endl;
            continue; // Data is malformed
        }

        this->storage->Produce(data);
    }
}

bool Base_DataRouter::StartThread()
{
    if (this->is_running) {
        return false;
    }

    this->running_thread = std::thread(&Base_DataRouter::loop, this);
    this->is_running = true;

    return true;;
}

bool Base_DataRouter::StopThread()
{
    if (!this->is_running)
        return false;

    this->is_running = false;
    this->running_thread.join();

    return true;
}

double Base_DataRouter::GetPollInterval() const
{
    return this->poll_interval.count();
}

void Base_DataRouter::SetPollInterval(double value)
{
    if (value <= 0)
        return;

    this->poll_interval = std::chrono::duration<double, std::milli>(value);
}
