/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef BASE_SETTINGSCONTAINER_H
#define BASE_SETTINGSCONTAINER_H

#include "../interfaces/isettingscontainer.h"

#include <map>
#include <stdint.h>

#include "../support/chaninfo.h"
#include "../support/global.h"

class TT_EXPORT Base_SettingsContainer : public ISettingsContainer
{
    friend class MainWindow;

private:
    std::map<uint64_t, chaninfo> chan_infos;
    //expinfo experiment_info;

public:
    Base_SettingsContainer();

    explicit Base_SettingsContainer(const std::map<uint64_t, chaninfo> &chan_infos);

    virtual const std::map<uint64_t, chaninfo>& GetChannelInfos() const override;

    //virtual expinfo GetExperimentInfo() const override;
};

#endif // BASE_SETTINGSCONTAINER_H
