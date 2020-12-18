/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef PLOT_TIMETRACE_UI_H
#define PLOT_TIMETRACE_UI_H

#include <phast_gui/base/base_view_plot.h>

class plot_timetrace_ui : public base_view_plot
{
private:
    double linewidth;
    QwtPlotCurve::CurveStyle curve_style;
    QString x_title;
    QString y_title;
    bool online;

protected:
    virtual Base_ui_calc *init_single_calc(const std::vector<chan_id> & channels,
                    uint64_t bin_width,
                    opaque_ptr ref,
                    bool display_only_initial_data);

    virtual plot_params get_default_plotparams();

    virtual double get_def_linewidth();
    virtual QwtPlotCurve::CurveStyle get_def_pen_style();
    virtual QString get_x_axis_title();
    virtual QString get_y_axis_title();

public:
    plot_timetrace_ui(double time_unit,
                      QWidget *parent,
                      ITempDataStorage *data,
                      const std::vector<chan_id> &channels,
                      bool sum_channels,
                      uint64_t bin_width,
                      bool online = true,
                      double linewidth = 1.2,
                      QwtPlotCurve::CurveStyle style=QwtPlotCurve::Lines,
                      QString x_title = "Time (s)",
                      QString y_title = "Counts / bin");
};

#endif // PLOT_TIMETRACE_UI_H
