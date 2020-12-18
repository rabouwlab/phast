/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef ITIMETAGGERCOMMUNICATOR_H
#define ITIMETAGGERCOMMUNICATOR_H

#include <map>
#include <vector>
#include <string>
#include "../support/photon_event.h"

struct chan_trigger_settings;

class ITimeTaggerCommunicator
{
public:
    virtual ~ITimeTaggerCommunicator() {}

    virtual uint64_t ReceiveData(std::vector<int64_t>* timestamps,
                             std::vector<uint8_t>* channel_IDs) = 0;

    virtual uint64_t GetSyncDivider(chan_id channel) = 0;

    virtual bool ConnectedToDevice() = 0;
    virtual const std::string& DeviceDescriptor() const = 0;
    virtual bool TryToConnect(int64_t device_ID) = 0; // -1 for any
    virtual bool DataLossSinceLastCall() = 0;
    virtual double TimeUnit() const = 0;
    virtual bool AnyDeviceAvailable() const = 0;
    virtual bool Disconnect() = 0;
    virtual uint64_t GetNumDevicesConnected() = 0;
    virtual bool IsRealDevice() const = 0;
};
#endif // ITIMETAGGERCOMMUNICATOR_H
