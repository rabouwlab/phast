/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef FAKE_TIMETAG_DEVICE_PLUGIN_H
#define FAKE_TIMETAG_DEVICE_PLUGIN_H

#include "fake_timetag_device_plugin_global.h"

#include <phast_gui/interfaces/itimetagplugin.h>
#include <phast_gui/interfaces/itimetaggercommunicator.h>

#include <qobject.h>

#include "fakecommunicator.h"

class FAKE_TIMETAG_DEVICE_PLUGINSHARED_EXPORT fake_timetag_device_plugin : public QObject,
                                                                           public ITimetagPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fake_timetag_device_plugin" )
    Q_INTERFACES(ITimetagPlugin)

private:
    FakeCommunicator* comm;

public:
    fake_timetag_device_plugin();
    virtual ~fake_timetag_device_plugin();

    virtual QString TimeTagPluginName() const override;
    virtual QString TimeTagPluginDescriptor() const override;
    virtual QString TimeTagMenuDescriptor() const override;
    virtual bool TimeTagHasSettingsMenu() const override;

    virtual ITimeTaggerCommunicator* GetComm() const override;
    virtual void DisableChan(chan_id ID) override;
    virtual void ShowTriggerSettingsUI(QWidget* parent, const std::vector<chan_id>& chan_IDs) override;
    virtual void ShowInitDialog(QWidget* parent) override;
};

#endif // FAKE_TIMETAG_DEVICE_PLUGIN_H
