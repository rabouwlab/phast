/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "decaycurve_view_plugin.h"

#include "widget_decaybinsettings.h"
#include "plot_decay_ui.h"

QString decaycurve_view_plugin::DataviewPluginName() const
{
    return "decay curve view plugin";
}

QString decaycurve_view_plugin::DataviewPluginDescriptor() const
{
    return "provides a GUI to view the fluorescence decay curve live";
}

QString decaycurve_view_plugin::DataviewMenuDescriptor() const
{
    return "Decay curve";
}

bool decaycurve_view_plugin::HandlesOnlyMicrotimeChans() const
{
    return true;
}

bool decaycurve_view_plugin::HandlesOnlyNonMicrotimeChans() const
{
    return false;
}

bool decaycurve_view_plugin::HandlesOnlyPulseChans() const
{
    return false;
}

IDataWindowStub* decaycurve_view_plugin::ConstructView(double time_unit,
                                       QWidget *parent,
                                       ITempDataStorage *data,
                                       const std::vector<chan_id> &channels, bool online) const
{
    widget_decaybinsettings d(time_unit, parent, channels);
    int ret = d.exec();

    if (ret != QDialog::Accepted) {
        return nullptr;
    }

    widget_decaybinsettings::decaybinsettings p = d.GetSettings();

    if (p.bin_size < 1 || p.channels.size() == 0) {
        return nullptr;
    }

    IDataWindowStub* t_ui = nullptr;

    t_ui = new plot_decay_ui(time_unit,
                             parent,
                             data,
                             p.channels,
                             p.bin_size,
                             p.bin_low,
                             p.bin_high,
                             p.sum_channels,
                             !online);

    return t_ui;
}
