/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef PLOT_CORRELATION_UI_H
#define PLOT_CORRELATION_UI_H

#include <phast_gui/base/base_view_plot.h>
#include <qwt/qwt_plot_curve.h>

class plot_correlation_ui : public base_view_plot
{
private:
    double bin_low;
    double bin_high;

    bool correlate_FCS;
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
    plot_correlation_ui(double time_unit,
                        QWidget *parent,
                  ITempDataStorage *data,
                  const std::vector<chan_id> &channels, bool corr_FCS,
                  uint64_t bin_width,
                  double bin_low,
                  double bin_high,
                  bool display_only_initial_data=false,
                  double linewidth = 1.5,
                  QwtPlotCurve::CurveStyle style=QwtPlotCurve::Lines,
                  QString x_title = "Lag time (ns)",
                  QString y_title = "Correlation (norm.)");
};

#endif // PLOT_CORRELATION_UI_H
