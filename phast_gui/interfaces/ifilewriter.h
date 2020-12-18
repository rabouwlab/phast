/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef IFILEWRITER_H
#define IFILEWRITER_H

#include <stdint.h>

class IFileWriter
{
public:
    virtual ~IFileWriter() {}

    virtual bool write_photon_event(int64_t macrotime, int64_t microtime) = 0;

    virtual uint64_t NumPhotonsWritten() const = 0;
    virtual uint64_t NumOverflows() const = 0;
    virtual uint64_t NumBytesWritten() const = 0;

    virtual bool WritesMicroTimesExplicitely() const = 0;
};

#endif // IFILEWRITER_H
