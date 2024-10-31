/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef IDATAVIEW_H
#define IDATAVIEW_H

#include <QObject>
#include <map>
#include <qwt-qt5/qwt_plot_curve.h>

#include "../support/plotparams.h"
#include "../support/typedefs.h"

#include "../interfaces/idataplot.h"

class MainPlot;

class DataView : public IDataPlot
{
    Q_OBJECT

public:
    struct data_update
    {
    public:
        opaque_ptr sender;
        std::vector<double> xs;
        std::vector<double> ys;
        int64_t chan_num;
    };

private:
    MainPlot* plot;
    bool scale_xmin;
    bool scale_xmax;
    bool scale_ymin;
    bool scale_ymax;

    bool keep_x_const_size;
    bool keep_y_const_size;
    double x_const_size;
    double y_const_size;

    plot_params p;
    std::map<int64_t, QwtPlotCurve*> chan_curves;

    std::vector<QColor> chan_colors;
    double linewidth;
    QwtPlotCurve::CurveStyle pen_style;

    QwtPlotCurve* make_new_curve(int64_t chan_num);

    void get_xlims(int64_t channel, double* xmin, double* xmax);
    void get_xlims(double* xmin, double* xmax);

    void get_ylims(int64_t channel, double xmin, double xmax, double* ymin, double* ymax);
    void get_ylims(double xmin, double xmax, double* ymin, double* ymax);

public:
    DataView(const DataView&) = delete;
    DataView& operator=(const DataView&) = delete;

    ~DataView();
    DataView(MainPlot* plot, double linewidth, QwtPlotCurve::CurveStyle pen_style);

    virtual plot_params get_plot_params() const override;


public slots:
    virtual void on_new_data(data_update data);

    virtual void set_plot_params(plot_params value) override;
    virtual void do_plot() override;
};

#endif // IDATAVIEW_H
