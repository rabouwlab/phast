/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef ILISTENER_H
#define ILISTENER_H

#include <stdint.h>
#include <map>

#include "../support/typedefs.h"

class ITempDataStorage;
class DetectionUpdate;

class IListener
{
public:
    virtual ~IListener() {}

    // Should return time from which deletion is not allowed yet.
    //  (all times smaller than this time may be removed)
    virtual const chan_data_map NewEvent(ITempDataStorage* sender, DetectionUpdate event) = 0;
};

#endif // ILISTENER_H
