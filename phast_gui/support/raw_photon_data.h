/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef RAW_PHOTON_DATA_H
#define RAW_PHOTON_DATA_H

#include <vector>
#include <stdint.h>

struct raw_photon_data
{
public:
    std::vector<int64_t>* timestamps;
    std::vector<uint8_t>* channels;
};

#endif // RAW_PHOTON_DATA_H
