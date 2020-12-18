/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "timetrace_view_plugin.h"

#include "create_trace_ui_dialog.h"
#include "plot_timetrace_ui.h"

Timetrace_view_plugin::Timetrace_view_plugin()
{
}

QString Timetrace_view_plugin::DataviewPluginName() const
{
    return "intensity trace view plugin";
}

QString Timetrace_view_plugin::DataviewPluginDescriptor() const
{
    return "provides a GUI to view the signal (intensity trace) live";
}

QString Timetrace_view_plugin::DataviewMenuDescriptor() const
{
    return "intensity trace";
}

bool Timetrace_view_plugin::HandlesOnlyMicrotimeChans() const
{
    return false;
}

bool Timetrace_view_plugin::HandlesOnlyNonMicrotimeChans() const
{
    return false;
}

bool Timetrace_view_plugin::HandlesOnlyPulseChans() const
{
    return false;
}

IDataWindowStub* Timetrace_view_plugin::ConstructView(double time_unit,
                                       QWidget *parent,
                                       ITempDataStorage *data,
                                       const std::vector<chan_id> &channels,
                                                      bool online) const
{
    create_trace_ui_dialog d(parent, channels, time_unit);
    int ret = d.exec();

    if (ret != QDialog::Accepted) {
        return nullptr;
    }

    create_trace_ui_dialog::init_vals p = d.GetValues();

    if (p.bin_width < 1 || p.channels.size() == 0) {
        return nullptr;
    }

    IDataWindowStub* t_ui = nullptr;

    t_ui = new plot_timetrace_ui(time_unit,
                                 parent,
                                 data,
                                 p.channels,
                                 p.sum_values,
                                 p.bin_width,
                                 online);

    return t_ui;
}
