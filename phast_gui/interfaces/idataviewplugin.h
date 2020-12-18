/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef IDATAVIEWPLUGIN_H
#define IDATAVIEWPLUGIN_H

#include <QString>
#include <QWidget>
#include <QObject>
#include <qobject.h>

#include "../support/typedefs.h"

class IDataWindowStub;
class ITempDataStorage;

class IDataviewPlugin
{
public:
    virtual ~IDataviewPlugin() {}

    virtual QString DataviewPluginName() const = 0;
    virtual QString DataviewPluginDescriptor() const = 0;
    virtual QString DataviewMenuDescriptor() const = 0;

    virtual bool HandlesOnlyMicrotimeChans() const = 0;
    virtual bool HandlesOnlyNonMicrotimeChans() const = 0;
    virtual bool HandlesOnlyPulseChans() const = 0;

    virtual IDataWindowStub* ConstructView(double time_unit,
                                           QWidget *parent,
                                           ITempDataStorage *data,
                                           const std::vector<chan_id> &channels,
                                           bool online) const = 0;


};

#define IDataviewPlugin_iid "IDataviewPlugin/1.0"

Q_DECLARE_INTERFACE(IDataviewPlugin, IDataviewPlugin_iid)

#endif // IDATAVIEWPLUGIN_H
