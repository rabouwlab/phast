/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "qutau_timetag_plugin.h"

#include "qutau_communicator.h"
#include "triggersettings_ui.h"

#include <qwidget.h>
#include <algorithm>

Qutau_timetag_plugin::~Qutau_timetag_plugin()
{
    delete this->comm;
}

Qutau_timetag_plugin::Qutau_timetag_plugin() :
    comm(new qutau_communicator()),
    chan_settings()
{
}

QString Qutau_timetag_plugin::TimeTagPluginName() const
{
    return "qutau_timetag_plugin";
}

QString Qutau_timetag_plugin::TimeTagPluginDescriptor() const
{
    return "plugin to communicate with QuTools quTAU devices";
}

QString Qutau_timetag_plugin::TimeTagMenuDescriptor() const
{
    return "quTAU trigger settings";
}

ITimeTaggerCommunicator* Qutau_timetag_plugin::GetComm() const
{
    return this->comm;
}

bool Qutau_timetag_plugin::TimeTagHasSettingsMenu() const
{
    return true;
}

void Qutau_timetag_plugin::DisableChan(chan_id ID)
{
    this->comm->DisableChannel(ID);
}

void Qutau_timetag_plugin::ShowInitDialog(QWidget *parent)
{
    return;
}

void Qutau_timetag_plugin::update_chan_settings_map(const std::vector<chan_id>& active_channels)
{
    std::vector<chan_id> actives = active_channels;
    std::vector<chan_id> to_be_removed;

    // See if the stored channels are in the active list.
    for (auto pair : this->chan_settings) {
        chan_id id = pair.first;
        bool found = false;
        uint64_t found_index = 0;

        for (uint64_t i = 0; i < actives.size(); i++) {
            if (id == actives.at(i)) { // We found a hit
                found = true;
                found_index = 0;
                break;
            }
        }

        if (!found) { // Hit not found, so remove this channel.
            to_be_removed.push_back(id);
        } else { // Hit found, so we don't need to add this active channel.
            actives.erase(actives.begin() + found_index);
        }
    }

    for (chan_id rem : to_be_removed) { // Remove inactive channels
        for (auto it = this->chan_settings.begin(); it != this->chan_settings.end(); it++) {
            auto pair = *it;
            chan_id id = pair.first;

            if (id == rem) {
                this->comm->DisableChannel(rem);
                this->chan_settings.erase(it);
                break;
            }
        }
    }

    for (chan_id a : actives) { // Add active channels we did not have yet.
        chan_trigger_settings cts;
        cts.ID = a;

        this->chan_settings[a] = cts;
    }
}

void Qutau_timetag_plugin::ShowTriggerSettingsUI(QWidget *parent, const std::vector<chan_id>& chan_IDs)
{
    this->update_chan_settings_map(chan_IDs);

    triggersettings_ui ui(parent, this->comm, this->chan_settings);

    ui.exec();
}
