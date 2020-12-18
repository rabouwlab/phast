/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef SMALL_TCSPC_IO_H
#define SMALL_TCSPC_IO_H

#include <stdio.h>

#include <QString>

#include "../support/typedefs.h"
#include "../support/photon_event.h"
#include "../interfaces/ifilewriter.h"

class small_tcspc_io : public IFileWriter
{
private:
    QString outputfile;

    FILE *f;

    timestamp last_macro_timestamp;
    uint64_t n_overflows;
    uint64_t overflow_value;
    uint64_t overflow_value_micro;

    uint64_t n_photons_written;
    uint64_t n_bytes_written;

private:
    bool write_overflow_event(uint64_t n_overflows);
    bool write_photon_event_(timestamp macrotime, timestamp microtime);

public:
    small_tcspc_io(const small_tcspc_io&) = delete;
    small_tcspc_io& operator=(const small_tcspc_io&) = delete;

    ~small_tcspc_io();
    explicit small_tcspc_io(QString outputfile);

    virtual bool write_photon_event(int64_t macrotime, int64_t microtime) override;
    virtual uint64_t NumPhotonsWritten() const override;
    virtual uint64_t NumOverflows() const override;
    virtual uint64_t NumBytesWritten() const override;
    virtual bool WritesMicroTimesExplicitely() const override;
};

#endif // SMALL_TCSPC_IO_H
