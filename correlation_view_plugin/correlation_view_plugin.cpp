/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "correlation_view_plugin.h"

#include "widget_correlation_bin_settings.h"
#include "plot_correlation_ui.h"

QString correlation_view_plugin::DataviewPluginName() const
{
    return "correlation view plugin";
}

QString correlation_view_plugin::DataviewPluginDescriptor() const
{
    return "provides a GUI to view correlation curves (anti-bunching, FCS) live";
}

QString correlation_view_plugin::DataviewMenuDescriptor() const
{
    return "Correlation curve";
}

bool correlation_view_plugin::HandlesOnlyMicrotimeChans() const
{
    return false;
}

bool correlation_view_plugin::HandlesOnlyNonMicrotimeChans() const
{
    return false;
}

bool correlation_view_plugin::HandlesOnlyPulseChans() const
{
    return false;
}

IDataWindowStub* correlation_view_plugin::ConstructView(double time_unit,
                                       QWidget *parent,
                                       ITempDataStorage *data,
                                       const std::vector<chan_id> &channels,
                                                        bool online) const
{
    widget_correlation_bin_settings d (time_unit, parent, channels);
    int ret = d.exec();

    if (ret != QDialog::Accepted) {
        return nullptr;
    }

    widget_correlation_bin_settings::corrbinsettings p = d.GetSettings();


    if (p.bin_size < 1) {
        return nullptr;
    }

    IDataWindowStub* t_ui = nullptr;

    std::vector<chan_id> c;
    c.push_back(p.left_chan);
    c.push_back(p.right_chan);

    t_ui = new plot_correlation_ui(time_unit,
                                   parent,
                                   data,
                                   c,
                                   p.FCS,
                                   p.bin_size,
                                   p.bin_low,
                                   p.bin_high,
                                   !online);

    return t_ui;
}
