/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "plot_timetrace_ui.h"

#include "timetrace_calc.h"

plot_timetrace_ui::plot_timetrace_ui(double time_unit, QWidget *parent,
                                     ITempDataStorage *data,
                                     const std::vector<chan_id> &channels,
                                     bool sum_channels,
                                     uint64_t bin_width,
                                     bool online,
                                     double linewidth,
                                     QwtPlotCurve::CurveStyle style,
                                     QString x_title,
                                     QString y_title) :
    base_view_plot(time_unit, parent, data, channels, sum_channels, bin_width, x_title, y_title, linewidth, style, !online),
    linewidth(linewidth),
    curve_style(style),
    x_title(x_title),
    y_title(y_title),
    online(online)
{
    this->init_calcs();

    plot_params p = this->get_default_plotparams();

    this->view->set_plot_params(p);
}

double plot_timetrace_ui::get_def_linewidth()
{
    return this->linewidth;
}

QwtPlotCurve::CurveStyle plot_timetrace_ui::get_def_pen_style()
{
    return this->curve_style;
}

QString plot_timetrace_ui::get_x_axis_title()
{
    return this->x_title;
}

QString plot_timetrace_ui::get_y_axis_title()
{
    return this->y_title;
}

Base_ui_calc* plot_timetrace_ui::init_single_calc(const std::vector<chan_id> &channels, uint64_t bin_width, opaque_ptr ref, bool display_only_initial_data)
{
    return new timetrace_calc(channels, bin_width, ref, display_only_initial_data, this->time_unit);
}

plot_params plot_timetrace_ui::get_default_plotparams()
{
    plot_params p;
    if (this->online) {
        p.xmin = 0;
        p.xmax = 1000;
        p.x_log_scale = false;
        p.xmax_autoscale = true;
        p.xmin_autoscale = false;

        p.ymin = 0;
        p.ymax = 100;
        p.y_log_scale = false;
        p.ymax_autoscale = true;
        p.ymin_autoscale = false;

        p.keep_x_const_size = true;
        p.keep_y_const_size = false;
        p.x_const_size = 20;
        p.y_const_size = 1000;
    } else {
        p.xmin = 0;
        p.xmax = 1000;
        p.x_log_scale = false;
        p.xmax_autoscale = true;
        p.xmin_autoscale = true;

        p.ymin = 0;
        p.ymax = 100;
        p.y_log_scale = false;
        p.ymax_autoscale = true;
        p.ymin_autoscale = false;

        p.keep_x_const_size = false;
        p.keep_y_const_size = false;
        p.x_const_size = 20;
        p.y_const_size = 1000;
    }

    return p;
}
