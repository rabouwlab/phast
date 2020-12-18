/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef small_tcspc_io2_H
#define small_tcspc_io2_H

#include <stdio.h>

#include <QString>

#include "../support/typedefs.h"
#include "../support/photon_event.h"
#include "../interfaces/ifilewriter.h"

class small_tcspc_io2 : public IFileWriter
{
private:
    QString outputfile;

    FILE *f;

    timestamp last_macro_timestamp;
    uint64_t n_overflows;
    uint64_t overflow_value;

    uint64_t n_photons_written;
    uint64_t n_bytes_written;

    static constexpr const char* MAGIC = "SSTT2";

private:
    bool write_overflow_event(uint64_t n_overflows);
    bool write_photon_event_(timestamp macrotime);

    void write_header();


public:
    small_tcspc_io2(const small_tcspc_io2&) = delete;
    small_tcspc_io2& operator=(const small_tcspc_io2&) = delete;

    ~small_tcspc_io2();
    explicit small_tcspc_io2(QString outputfile);

    virtual bool write_photon_event(int64_t macrotime, int64_t microtime) override;
    virtual uint64_t NumPhotonsWritten() const override;
    virtual uint64_t NumOverflows() const override;
    virtual uint64_t NumBytesWritten() const override;
    virtual bool WritesMicroTimesExplicitely() const override;
};

#endif // small_tcspc_io2_H
