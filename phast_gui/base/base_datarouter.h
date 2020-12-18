/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef DATARECEIVER_H
#define DATARECEIVER_H

#include "../interfaces/idatarouter.h"
#include "../interfaces/ilistener.h"
#include "../support/photon_event.h"

#include <stdint.h>
#include <vector>
#include <thread>
#include <map>

#include "../support/raw_photon_data.h"

#include "../support/lockfreequeue.h"
#include "../support/global.h"

#define MAX_NUM_CHANNELS 32

class ITimeTaggerCommunicator;

class TT_EXPORT Base_DataRouter : public IDataRouter
{
private:
    ITimeTaggerCommunicator* tt_comm;
    LockFreeQueue<raw_photon_data>* storage;

    std::chrono::duration<double, std::milli> poll_interval;

    bool is_running;

    void loop();

    std::thread running_thread;

public:
    Base_DataRouter(const Base_DataRouter&) = delete;
    Base_DataRouter& operator=(const Base_DataRouter&) = delete;

    ~Base_DataRouter();

    Base_DataRouter(ITimeTaggerCommunicator* tt_comm,
                    LockFreeQueue<raw_photon_data> *storage,
                 double poll_interval);

    virtual bool StartThread() override;
    virtual bool StopThread() override;

    virtual double GetPollInterval() const override;
    virtual void SetPollInterval(double value) override;
};

#endif // DATARECEIVER_H
