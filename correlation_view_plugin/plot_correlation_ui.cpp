/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "plot_correlation_ui.h"

#include "correlation_calc.h"

plot_correlation_ui::plot_correlation_ui(double time_unit,
                                         QWidget *parent,
                                         ITempDataStorage *data,
                                         const std::vector<chan_id> &channels,
                                         bool corr_FCS,
                                         uint64_t bin_width,
                                         double bin_low,
                                         double bin_high,
                                         bool display_only_initial_data,
                                         double linewidth,
                                         QwtPlotCurve::CurveStyle style,
                                         QString x_title,
                                         QString y_title) :
    base_view_plot(time_unit, parent, data, channels, true, bin_width, x_title, y_title, linewidth, style, display_only_initial_data),
    bin_low(bin_low),
    bin_high(bin_high),
    correlate_FCS(corr_FCS),
    linewidth(linewidth),
    curve_style(style),
    x_title(x_title),
    y_title(y_title)
{
    this->init_calcs();

    plot_params p = this->get_default_plotparams();

    p.xmin = bin_low * this->time_unit * 1e9;
    p.xmax = bin_high * this->time_unit * 1e9;

    this->view->set_plot_params(p);
}

double plot_correlation_ui::get_def_linewidth()
{
    return this->linewidth;
}

QwtPlotCurve::CurveStyle plot_correlation_ui::get_def_pen_style()
{
    return this->curve_style;
}

QString plot_correlation_ui::get_x_axis_title()
{
    return this->x_title;
}

QString plot_correlation_ui::get_y_axis_title()
{
    return this->y_title;
}

plot_params plot_correlation_ui::get_default_plotparams()
{
    plot_params p;

    if (this->correlate_FCS) {
        p.xmin = this->bin_low*1e9*this->time_unit;
        p.xmax = this->bin_high*1e9*this->time_unit;
        p.x_log_scale = true;
        p.xmax_autoscale = true;
        p.xmin_autoscale = true;

        p.ymin = 0;
        p.ymax = 10;
        p.y_log_scale = false;
        p.ymax_autoscale = true;
        p.ymin_autoscale = false;

        p.keep_x_const_size = false;
        p.keep_y_const_size = false;
    } else {
        p.xmin = this->bin_low*1e9*this->time_unit;
        p.xmax = this->bin_high*1e9*this->time_unit;
        p.x_log_scale = false;
        p.xmax_autoscale = false;
        p.xmin_autoscale = false;

        p.ymin = 0;
        p.ymax = 10;
        p.y_log_scale = false;
        p.ymax_autoscale = true;
        p.ymin_autoscale = false;

        p.keep_x_const_size = false;
        p.keep_y_const_size = false;
    }

    return p;
}

Base_ui_calc* plot_correlation_ui::init_single_calc(const std::vector<chan_id> &channels,
                                                    uint64_t bin_width,
                                                    opaque_ptr ref,
                                                    bool display_only_initial_data)
{
    return new correlation_calc(channels,
                                bin_width,ref,
                                this->bin_low,
                                this->bin_high,
                                display_only_initial_data,
                                this->time_unit,
                                this->correlate_FCS);
}
