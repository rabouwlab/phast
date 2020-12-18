/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef QUTAU_TIMETAG_PLUGIN_H
#define QUTAU_TIMETAG_PLUGIN_H

#include "qutau_timetag_plugin_global.h"

#include <phast_gui/interfaces/itimetagplugin.h>
#include <phast_gui/interfaces/itimetaggercommunicator.h>

#include <qobject.h>

#include "qutau_communicator.h"
#include "chan_trigger_settings.h"

class QUTAU_TIMETAG_PLUGINSHARED_EXPORT Qutau_timetag_plugin : public QObject,
                                                               public ITimetagPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "qutau_itimetagplugin" )
    Q_INTERFACES(ITimetagPlugin)

private:
    qutau_communicator* comm;
    std::map<chan_id, chan_trigger_settings> chan_settings;

    void update_chan_settings_map(const std::vector<chan_id>& active_channels);

public:
    Qutau_timetag_plugin();
    virtual ~Qutau_timetag_plugin();

    virtual QString TimeTagPluginName() const override;
    virtual QString TimeTagPluginDescriptor() const override;
    virtual QString TimeTagMenuDescriptor() const override;
    virtual bool TimeTagHasSettingsMenu() const override;

    virtual ITimeTaggerCommunicator* GetComm() const override;
    virtual void DisableChan(chan_id ID) override;
    virtual void ShowTriggerSettingsUI(QWidget* parent, const std::vector<chan_id>& chan_IDs) override;
    virtual void ShowInitDialog(QWidget* parent) override;
};

#endif // QUTAU_TIMETAG_PLUGIN_H
