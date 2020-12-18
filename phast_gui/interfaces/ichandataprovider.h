/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef ICHANDATAPROVIDER_H
#define ICHANDATAPROVIDER_H

#include "typedefs.h"

class ChannelData;

class IChanDataProvider
{
public:
    virtual ~IChanDataProvider() {}

    virtual ChannelData* GetChanData(chan_id ID) = 0;
};
#endif // ICHANDATAPROVIDER_H
