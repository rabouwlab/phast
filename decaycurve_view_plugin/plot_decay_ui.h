/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef PLOT_DECAY_UI_H
#define PLOT_DECAY_UI_H

#include <phast_gui/base/base_view_plot.h>

class ISettingsContainer;

class plot_decay_ui : public base_view_plot
{
private:
    double bin_low;
    double bin_high;

    double linewidth;
    QwtPlotCurve::CurveStyle curve_style;
    QString x_title;
    QString y_title;

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
    plot_decay_ui(const plot_decay_ui&) = delete;
    plot_decay_ui& operator=(const plot_decay_ui&) = delete;

    plot_decay_ui(double time_unit,
                  QWidget *parent,
                  ITempDataStorage *data,
                  const std::vector<chan_id> &channels,
                  uint64_t bin_width,
                  double bin_low,
                  double bin_high,
                  bool sum_channels,
                  bool display_only_initial_data=false,
                  double linewidth = 1.5,
                  QwtPlotCurve::CurveStyle style=QwtPlotCurve::Dots,
                  QString x_title = "Time after pulse (ns)",
                  QString y_title = "Counts / bin");
};

#endif // PLOT_DECAY_UI_H
