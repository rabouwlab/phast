/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef PHOTON_EVENT_H
#define PHOTON_EVENT_H

#include <stdint.h>

#include "../support/typedefs.h"

struct photon_event
{
public:
    chan_id channel_number;
    timestamp macrotime;
    timestamp microtime;

    friend bool operator<(const photon_event& l, const photon_event& r)
    {
        return l.macrotime < r.macrotime; // keep the same order
    }

    friend bool operator>(const photon_event& l, const photon_event& r)
    {
        return l.macrotime > r.macrotime; // keep the same order
    }

    friend bool operator<=(const photon_event& l, const photon_event& r)
    {
        return l.macrotime <= r.macrotime; // keep the same order
    }

    friend bool operator>=(const photon_event& l, const photon_event& r)
    {
        return l.macrotime >= r.macrotime; // keep the same order
    }
};


#endif // PHOTON_EVENT_H
