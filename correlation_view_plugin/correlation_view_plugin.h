/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef TIMETRACE_VIEW_PLUGIN_H
#define TIMETRACE_VIEW_PLUGIN_H

#include "correlation_view_plugin_global.h"

#include <phast_gui/interfaces/idataviewplugin.h>
#include <qobject.h>

class CORRELATION_VIEW_PLUGINSHARED_EXPORT correlation_view_plugin : public QObject,
                                                                 public IDataviewPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "correlation_view_plugin" )
    Q_INTERFACES(IDataviewPlugin)

public:
    correlation_view_plugin() {}

    virtual QString DataviewPluginName() const override;
    virtual QString DataviewPluginDescriptor() const override;
    virtual QString DataviewMenuDescriptor() const override;

    virtual bool HandlesOnlyMicrotimeChans() const override;
    virtual bool HandlesOnlyNonMicrotimeChans() const override;
    virtual bool HandlesOnlyPulseChans() const override;

    virtual IDataWindowStub* ConstructView(double time_unit,
                                           QWidget *parent,
                                           ITempDataStorage *data,
                                           const std::vector<chan_id> &channels,
                                           bool online) const override;
};

#endif // TIMETRACE_VIEW_PLUGIN_H
