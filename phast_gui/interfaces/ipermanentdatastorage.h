/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef IPERMANENTDATASTORAGE_H
#define IPERMANENTDATASTORAGE_H

#include <map>
#include <vector>

#include "photon_event.h"
#include "dat_type.h"

class IPermanentDataStorage
{
public:
    virtual ~IPermanentDataStorage() {}

    virtual void GetData(chan_dat_map* data_start, chan_dat_map* data_end) = 0;
    // virtual const std::map<uint64_t, std::vector<photon_event> >* GetData() const = 0;
};

#endif // IPERMANENTDATASTORAGE_H
