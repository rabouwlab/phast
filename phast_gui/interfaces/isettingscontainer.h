/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef ISETTINGSCONTAINER_H
#define ISETTINGSCONTAINER_H

#include "../support/chaninfo.h"

#include <map>

class ISettingsContainer
{
public:
    virtual ~ISettingsContainer() {}

    virtual const std::map<uint64_t, chaninfo>& GetChannelInfos() const = 0;

    //virtual const std::map<uint64_t, chan_trigger_settings>& GetChannelSettings() const = 0;

    //virtual expinfo GetExperimentInfo() const = 0;
};
#endif // ISETTINGSCONTAINER_H
