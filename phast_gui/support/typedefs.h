/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stdint.h>
#include <vector>
#include <map>

typedef uint64_t chan_id;
typedef uint64_t opaque_ptr;
typedef int64_t timestamp;
typedef uint64_t index;

struct photon_event;

typedef std::vector<photon_event> dat_holder;

typedef dat_holder::iterator dat_type;

typedef std::map<chan_id, dat_type> chan_dat_map;

typedef std::map<chan_id, dat_holder> chan_dat_holder;

typedef std::vector<timestamp> data_holder;

typedef data_holder::iterator data_type;

typedef std::map<chan_id, data_type> chan_data_map;

typedef std::map<chan_id, data_holder> chan_data_holder;

#endif // TYPEDEFS_H
