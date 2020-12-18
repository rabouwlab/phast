/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "base_settingscontainer.h"

Base_SettingsContainer::Base_SettingsContainer() :
    chan_infos()
{
}

Base_SettingsContainer::Base_SettingsContainer(const std::map<uint64_t, chaninfo>& chan_infos) :
    chan_infos(chan_infos)
{
}

const std::map<uint64_t, chaninfo>& Base_SettingsContainer::GetChannelInfos() const
{
    return this->chan_infos;
}
