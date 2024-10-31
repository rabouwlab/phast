/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef MAINPLOT_H
#define MAINPLOT_H

#include <qwt-qt5/qwt_plot.h>

class QwtPlotCurve;
class QwtPlotMarker;
class QwtPlotZoomer;
class QwtPlotPicker;

class MainPlot : public QwtPlot
{
    Q_OBJECT

protected:

    void change_axis_min(int axisId, double value);
    void change_axis_max(int axisId, double value);

    void change_axis_limits(int axisId, double min, double max);

public:
    MainPlot(const MainPlot&) = delete;
    MainPlot& operator=(const MainPlot&) = delete;
    ~MainPlot();
    explicit MainPlot( QWidget *parent );

public slots:
    void set_x_logscale(bool log_on);
    void set_y_logscale(bool log_on);

    void set_x_min(double value);
    void set_x_max(double value);
    void set_y_min(double value);
    void set_y_max(double value);

    void set_x_lims(double xmin, double xmax);
    void set_y_lims(double ymin, double ymax);

private:
    QwtPlotMarker *d_marker1;
    QwtPlotMarker *d_marker2;
    QwtPlotZoomer* zoomer;
    QwtPlotPicker* picker;

    bool xlog;
    bool ylog;

    double get_axis_min(int axisId) const;
    double get_axis_max(int axisId) const;

public:
    double get_x_min() const;
    double get_x_max() const;
    double get_y_min() const;
    double get_y_max() const;

    bool get_x_log() const;
    bool get_y_log() const;
};

#endif // MAINPLOT_H
