/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef ITEMPDATASTORAGE_H
#define ITEMPDATASTORAGE_H

#include <map>
#include <stdint.h>
#include <vector>

#include "../support/photon_event.h"
#include "../support/lockfreequeue.h"
#include "../support/raw_photon_data.h"

class IListener;

class ITempDataStorage
{
public:
    virtual ~ITempDataStorage() {}

    virtual void AddListener(IListener*) = 0;
    virtual void RemoveListener(IListener*) = 0;

    virtual double GetPollInterval() const = 0;
    virtual void SetPollInterval(double value) = 0;
    virtual bool StartThread() = 0;
    virtual bool StopThread() = 0;
};

#endif // ITEMPDATASTORAGE_H
