/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "writer_listeners.h"

#include "../support/detectionupdate.h"

writer_listeners::~writer_listeners()
{
    if (this->writer != nullptr)
        delete this->writer;
}

writer_listeners::writer_listeners(chan_id channel, IFileWriter *writer, bool only_write_once) :
    writer(writer),
    channel(channel),
    has_written_anything(false),
    only_write_once(only_write_once),
    done(false),
    in_event(false),
    in_event_mutex()
{
}

void writer_listeners::set_in_event(bool value)
{
    std::lock_guard<std::mutex> l(this->in_event_mutex);
    this->in_event = value;
}

bool writer_listeners::IsInEvent()
{
    std::lock_guard<std::mutex> l(this->in_event_mutex);
    return this->in_event;
}

const chan_data_map writer_listeners::NewEvent(ITempDataStorage *sender, DetectionUpdate event)
{
    this->set_in_event(true);

    chan_data_map ret;

    data_type write_start;
    data_type write_start_micro;
    uint64_t n_photons = 0;
    bool have_micro = false;

    for (auto pair : event.updates) {
        chan_id id = pair.first;
        chan_det_update upd = pair.second;

        ret[id] = upd.macro_end;

        if (id == this->channel) {
            write_start = (this->has_written_anything) ? upd.new_macro_start : upd.macro_start;
            write_start_micro = (this->has_written_anything) ? upd.new_micro_start : upd.micro_start;
            n_photons = (this->has_written_anything) ? upd.n_new_macro : upd.n_macro;

            if (upd.n_micro == 0) {
                have_micro = false;
            } else {
                have_micro = true;
            }
        }
    }

    if (this->has_written_anything && this->only_write_once) {
        this->set_in_event(false);
        return ret;
    }

    if (have_micro) {
        for (uint64_t i = 0; i < n_photons; i++) {
            int64_t macrotime = *(write_start + i);
            int64_t microtime = *(write_start_micro + i);
            this->writer->write_photon_event(macrotime, microtime);

            this->has_written_anything = true;
        }
    } else {
        for (uint64_t i = 0; i < n_photons; i++) {
            int64_t macrotime = *(write_start + i);
            int64_t microtime = 0;
            this->writer->write_photon_event(macrotime, microtime);

            this->has_written_anything = true;
        }
    }


    if (this->only_write_once) {
        this->done = true;
    }

    this->set_in_event(false);
    return ret;
}

bool writer_listeners::IsDone() const
{
    return this->done;
}

uint64_t writer_listeners::NumBytesWritten() const
{
    return this->writer->NumBytesWritten();
}

uint64_t writer_listeners::NumOverflows() const
{
    return this->writer->NumOverflows();
}

uint64_t writer_listeners::NumPhotonsWritten() const
{
    return this->writer->NumPhotonsWritten();
}
