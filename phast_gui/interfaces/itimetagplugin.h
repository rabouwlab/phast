/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef ITIMETAGPLUGIN_H
#define ITIMETAGPLUGIN_H

#include <QString>
#include <qobject.h>

#include "../support/typedefs.h"

class ITimeTaggerCommunicator;

class ITimetagPlugin
{
public:
    virtual ~ITimetagPlugin() {}

    virtual QString TimeTagPluginName() const = 0;
    virtual QString TimeTagPluginDescriptor() const = 0;
    virtual QString TimeTagMenuDescriptor() const = 0;
    virtual bool TimeTagHasSettingsMenu() const = 0;

    virtual ITimeTaggerCommunicator* GetComm() const = 0;
    virtual void DisableChan(chan_id ID) = 0;
    virtual void ShowTriggerSettingsUI(QWidget* parent, const std::vector<chan_id>& chan_IDs) = 0;
    virtual void ShowInitDialog(QWidget* parent) = 0;
};

#define ITimetagPlugin_iid "ITimetagPlugin/1.0"

Q_DECLARE_INTERFACE(ITimetagPlugin, ITimetagPlugin_iid)

#endif // ITIMETAGPLUGIN_H
