/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef TIMETAG_DEVICE_H
#define TIMETAG_DEVICE_H

#include <stdint.h>
#include <string>

#include "../interfaces/itimetagplugin.h"

struct timetag_device
{
public:
    ITimetagPlugin* communicator;
    uint64_t device_ID;
    std::string device_name;
    bool is_real_device;
};

#endif // TIMETAG_DEVICE_H
