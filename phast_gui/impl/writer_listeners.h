/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef WRITER_LISTENERS_H
#define WRITER_LISTENERS_H

#include "../interfaces/ilistener.h"
#include "../support/small_tcspc_io.h"
#include "../interfaces/ifilewriter.h"

#include <mutex>

class writer_listeners : public IListener
{
private:
    IFileWriter* writer;
    chan_id channel;
    bool has_written_anything;
    bool only_write_once;
    bool done;

    bool in_event;
    std::mutex in_event_mutex;

    void set_in_event(bool value);

public:
    writer_listeners(const writer_listeners&) = delete;
    operator=(const writer_listeners&) = delete;

    ~writer_listeners();
    writer_listeners(chan_id channel, IFileWriter* writer, bool only_write_once);

    virtual const chan_data_map NewEvent(ITempDataStorage* sender, DetectionUpdate event) override;

    bool IsDone() const;

    uint64_t NumPhotonsWritten() const;
    uint64_t NumOverflows() const;
    uint64_t NumBytesWritten() const;
    bool IsInEvent();
};

#endif // WRITER_LISTENERS_H
