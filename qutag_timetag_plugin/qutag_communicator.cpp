/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "qutag_communicator.h"
#include "qutag_inc/tdcbase.h"
#include "qutag_inc/tdcmultidev.h"

#include <phast_gui/support/photon_event.h>

#include "chan_trigger_settings.h"

#include <map>
#include <iostream>

#define MAX_BUF_SIZE 1000000
#define EXPOSURE_TIME 100       // in ms

qutag_communicator::~qutag_communicator()
{
    TDC_deInit();
}

bool qutag_communicator::IsRealDevice() const
{
    return true;
}

void qutag_communicator::update_channels_enabled()
{
    int32_t channels_mask = 0;

    for (chan_id id : this->enabled_channels) {
        channels_mask &= id;
    }

    TDC_enableChannels(channels_mask);
}

void qutag_communicator::EnableChannel(chan_id channel_id)
{
    for (chan_id id : this->enabled_channels) {
        if (id == channel_id)
            return;
    }

    this->enabled_channels.push_back(channel_id);
    this->update_channels_enabled();
}

void qutag_communicator::DisableChannel(chan_id channel_id)
{
    bool found = false;
    uint64_t index = 0;

    for (uint64_t i = 0; i < this->enabled_channels.size(); i++) {
        if (this->enabled_channels.at(i) == channel_id) {
            found = true;
            index = i;
            break;
        }
    }

    if (!found)
        return;

    this->enabled_channels.erase(this->enabled_channels.begin() + index);
    this->update_channels_enabled();
}

void qutag_communicator::SetChannelEnabled(chan_id channel_id, bool enabled)
{
    return (enabled) ? this->EnableChannel(channel_id) : this->DisableChannel(channel_id);
}

bool qutag_communicator::AnyDeviceAvailable() const
{
    if (this->have_device)
        return true;

    // We pass -1 to detect any device
    int rc = 0;

    TDC_init(-1);

    if (rc == TDC_Ok) {
        TDC_deInit();

        return true;
    }

    return false;
}

bool qutag_communicator::TryToConnect(int64_t device_ID)
{
    if (this->have_device)
        return true;

    // We pass -1 to detect any device
    int rc = TDC_connect(device_ID);

    if (rc != TDC_Ok) {
        printf("Could not connect to device. Error code: %i\n", rc);
        this->have_device = false;
        return false;
    } else {
        this->have_device = true;
    }

    this->device_type = TDC_getDevType();

    switch (this->device_type) {
    case DEVTYPE_QUTAG:
            this->device_descriptor = "qutag";
            break;
        case DEVTYPE_NONE:
        default:
            this->device_descriptor = "none";
            this->have_device = false;
            TDC_deInit();
            return false;
            break;
    }

    TDC_setTimestampBufferSize(this->timestamp_buffer_size);

    TDC_setExposureTime(EXPOSURE_TIME);

    // By default we enable ALL channels
    rc = TDC_enableChannels( 0xFF );

    // We can also set a 'sync divider'. This tells
    // the timetagger box to skip every n-th event in
    // channel 0 (ALWAYS channel zero, cannot be done
    // on other channels). This can be useful when using
    // high laser pulse frequencies.
    //  For now, we disable this.
    rc = TDC_configureSyncDivider(1, false);

    // Get the time unit
    TDC_getTimebase(&this->time_unit_seconds);

    return true;
}

qutag_communicator::qutag_communicator(uint64_t buffer_size) :
    have_device(false),
    device_type(DEVTYPE_NONE),
    enabled_channels(),
    time_unit_seconds(0.0),
    timestamp_buffer_size(buffer_size),
    reset_buffer_after_event_retrieval(true),
    device_descriptor("none")
{
    if (buffer_size > MAX_BUF_SIZE)
        this->timestamp_buffer_size = MAX_BUF_SIZE;

    unsigned int temp = 0;
    TDC_discover(&temp);
}

uint64_t qutag_communicator::ReceiveData(std::vector<int64_t>* timestamps,
                                     std::vector<uint8_t>* chan_IDs)
{
    timestamps->resize(this->timestamp_buffer_size);
    chan_IDs->resize(this->timestamp_buffer_size);

    uint64_t num_valid_events = 0;

    TDC_getLastTimestamps(this->reset_buffer_after_event_retrieval,
                          (int64_t*)&(*timestamps)[0],
                          (Int8*)&(*chan_IDs)[0],
                          (Int32*)&num_valid_events);

    timestamps->resize(num_valid_events);
    chan_IDs->resize(num_valid_events);

    return num_valid_events;
}

uint64_t qutag_communicator::UpdateSyncDivider(uint64_t value)
{
    Int32 divider = value;

    TDC_configureSyncDivider(divider, false);

    return this->GetSyncDivider(0);
}

uint64_t qutag_communicator::GetSyncDivider(chan_id channel_id)
{
    if (channel_id != 0)
        return 1;

    Int32 divider = 1;
    Bln32 reconstruct = false;

    TDC_getSyncDivider(&divider, &reconstruct);

    return (uint64_t)divider;
}

chan_trigger_settings qutag_communicator::UpdateSignalConditioning(uint64_t chan_ID, chan_trigger_settings new_values)
{
    Bln32 edge = (new_values.edge == chan_trigger_settings::RISING) ? 1 : 0;
    // Bln32 term = (new_values.terminate_in_signal_path) ? 1 : 0; // NOTE: the QuTAG device does not seem to support termination on/off
    double threshold = new_values.voltage_threshold;
    TDC_SignalCond cond = (new_values.signal_conditioning_enabled) ? SCOND_MISC : SCOND_LVTTL;
    Int32 channel = (Int32)chan_ID;

    TDC_configureSignalConditioning(channel, cond, edge, threshold);

    return this->GetSignalConditioning(chan_ID);
}

chan_trigger_settings qutag_communicator::GetSignalConditioning(uint64_t chan_ID)
{
    Int32 chan = (Int32)chan_ID;
    Bln32 on = 0;
    Bln32 edge = 1;
    //Bln32 term = 0;
    double threshold = 0;

    int error_val = TDC_getSignalConditioning(chan, &edge, &threshold);

    std::cout << "qutag getSignalConditioning error value: " << error_val << std::endl;

    chan_trigger_settings ret;
    ret.ID = chan_ID;
    ret.edge = (edge == 0) ? chan_trigger_settings::FALLING : chan_trigger_settings::RISING;
    ret.signal_conditioning_enabled = on;
    // ret.terminate_in_signal_path = term;
    ret.voltage_threshold = threshold;

    ret.delay_time = 0;

    if (chan_ID != 0) {
        ret.sync_divider = 1;
    } else {
        ret.sync_divider = this->GetSyncDivider(0);

    }
    return ret;
}

bool qutag_communicator::ConnectedToDevice()
{
    return this->have_device;
}

const std::string& qutag_communicator::DeviceDescriptor() const
{
    return this->device_descriptor;
}

bool qutag_communicator::DataLossSinceLastCall()
{
    Bln32 ret = 1;
    Bln32 ret2 = 1;
    TDC_getDataLost(&ret);
    TDC_getDataLost(&ret2);

    if (ret == 0 && ret2 == 0)
        return false;

    return true;
}

double qutag_communicator::TimeUnit() const
{
    double result = 0;

    TDC_getTimebase(&result);

    return result;
}

bool qutag_communicator::Disconnect()
{
    TDC_deInit();
    this->have_device = false;

    return true;
}

uint64_t qutag_communicator::GetNumDevicesConnected()
{
    if (this->have_device)
        return 0;

    uint64_t ret = 0;
    int error_code = TDC_discover((unsigned int*)&ret);

    return ret;
}
