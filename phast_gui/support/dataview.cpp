/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "dataview.h"

#include "plotparams.h"
#include "mainplot.h"

#include <QTimer>
#include <qwt/qwt_plot_curve.h>

DataView::~DataView()
{
}

DataView::DataView(MainPlot* plot, double linewidth, QwtPlotCurve::CurveStyle pen_style) :
    plot(plot),
    scale_xmin(false),
    scale_xmax(false),
    scale_ymin(false),
    scale_ymax(false),
    keep_x_const_size(false),
    keep_y_const_size(false),
    x_const_size(0.0),
    y_const_size(0.0),
    chan_colors(),
    linewidth(linewidth),
    pen_style(pen_style)
{
    chan_colors.push_back(Qt::yellow);
    chan_colors.push_back(Qt::cyan);
    chan_colors.push_back(Qt::magenta);
    chan_colors.push_back(Qt::green);

    plot_params temp_p;
    temp_p.keep_x_const_size = false;
    temp_p.keep_y_const_size = false;
    temp_p.xmax = 1e3;
    temp_p.xmax_autoscale = true;
    temp_p.xmin = 0;
    temp_p.xmin_autoscale = true;
    temp_p.x_const_size = 1.0;
    temp_p.x_log_scale = false;
    temp_p.ymax = 1e3;
    temp_p.ymax_autoscale = true;
    temp_p.ymin = 1;
    temp_p.ymin_autoscale = true;
    temp_p.y_const_size = 1.0;
    temp_p.y_log_scale = false;

    this->p = temp_p;
}

QwtPlotCurve* DataView::make_new_curve(int64_t chan_num)
{
    QwtPlotCurve* c = new QwtPlotCurve(QString::number(chan_num));

    QColor col;
    if (chan_num >= this->chan_colors.size() ||
            chan_num < 0) {
        col = Qt::white;
    } else {
        col = this->chan_colors[chan_num];
    }

    c->setPen(col, 1.2);
    c->setStyle(this->pen_style);

    return c;
}

plot_params DataView::get_plot_params() const
{
    return this->p;

    plot_params ret;
    ret.xmin = this->plot->get_x_min();
    ret.xmax = this->plot->get_x_max();
    ret.x_log_scale = this->plot->get_x_log();
    ret.xmin_autoscale = this->scale_xmin;
    ret.xmax_autoscale = this->scale_xmax;

    ret.ymin = this->plot->get_y_min();
    ret.ymax = this->plot->get_y_max();
    ret.y_log_scale = this->plot->get_y_log();
    ret.ymin_autoscale = this->scale_ymin;
    ret.ymax_autoscale = this->scale_ymax;

    ret.keep_x_const_size = this->keep_x_const_size;
    ret.keep_y_const_size = this->keep_y_const_size;
    ret.x_const_size = this->x_const_size;
    ret.y_const_size = this->y_const_size;

    return ret;
}

void DataView::set_plot_params(plot_params value)
{
    this->p = value;


    // TODO: input validation
    //this->plot->set_x_min(value.xmin);
    //this->plot->set_x_max(value.xmax);
    this->plot->set_x_logscale(value.x_log_scale);
    this->scale_xmin = value.xmin_autoscale;
    this->scale_xmax = value.xmax_autoscale;

    //this->plot->set_y_min(value.ymin);
    //this->plot->set_y_max(value.ymax);
    this->plot->set_y_logscale(value.y_log_scale);
    this->scale_ymin = value.ymin_autoscale;
    this->scale_ymax = value.ymax_autoscale;

    this->keep_x_const_size = value.keep_x_const_size;
    this->keep_y_const_size = value.keep_y_const_size;
    this->x_const_size = value.x_const_size;
    this->y_const_size = value.y_const_size;

    this->do_plot();
}

void DataView::do_plot() // Plot with a constant frame rate, e.g. 30/s
{
    double xmin = 0;
    double xmax = 0;

    this->get_xlims(&xmin, &xmax);

    double ymin = 0;
    double ymax = 0;

    this->get_ylims(xmin, xmax, &ymin, &ymax);

    this->plot->set_x_lims(xmin, xmax);
    this->plot->set_y_lims(ymin, ymax);

    this->plot->replot();

    this->p.xmin = xmin;
    this->p.xmax = xmax;
    this->p.ymin = ymin;
    this->p.ymax = ymax;
}

void DataView::get_xlims(int64_t channel, double *xmin, double *xmax)
{
    if (this->chan_curves.count(channel) == 0) {
        return;
    }

    if (xmin == nullptr || xmax == nullptr) {
        return;
    }

    QwtSeriesData<QPointF>* data = this->chan_curves.at(channel)->data();

    for (uint64_t i = 0; i < data->size(); i++) {
        double x = data->sample(i).x();

        if (x > *xmax) {
            *xmax = x;
        }

        if (x < *xmin) {
            *xmin = x;
        }
    }
}

void DataView::get_xlims(double* xmin, double* xmax)
{
    if (xmin == nullptr || xmax == nullptr) {
        return;
    }

    *xmin = 9e99;
    *xmax = 0;

    if (!this->scale_xmin && !scale_xmax) {
        *xmin = this->p.xmin;
        *xmax = this->p.xmax;

        return;
    }

    for (auto pair: this->chan_curves) {
        double xmin_ = 9e99;
        double xmax_ = 0;

        this->get_xlims(pair.first, &xmin_, &xmax_);

        if (xmin_ < *xmin) {
            *xmin = xmin_;
        }

        if (xmax_ > *xmax) {
           *xmax = xmax_;
        }
    }

    if (!this->scale_xmin) {
        *xmin = this->p.xmin;
    }

    if (!this->scale_xmax) {
        *xmax = this->p.xmax;
    }

    if (this->keep_x_const_size) {
        if (scale_xmin && !scale_xmax) {
            *xmax = this->x_const_size - *xmin;
        } else if (scale_xmax && !scale_xmin) {
            *xmin = *xmax - this->x_const_size;
        }
    }
}

void DataView::get_ylims(int64_t channel, double xmin, double xmax, double *ymin, double *ymax)
{
    if (this->chan_curves.count(channel) == 0) {
        return;
    }

    if (ymin == nullptr || ymax == nullptr) {
        return;
    }

    QwtSeriesData<QPointF>* data = this->chan_curves.at(channel)->data();

    for (uint64_t i = 0; i < data->size(); i++) {
        double x = data->sample(i).x();

        if (x < xmin || x > xmax) {
            continue;
        }

        double y = data->sample(i).y();

        if (y > *ymax) {
            *ymax = y;
        }

        if (y < *ymin) {
            *ymin = y;
        }
    }
}

void DataView::get_ylims(double xmin, double xmax, double* ymin, double* ymax)
{
    if (ymin == nullptr || ymax == nullptr) {
        return;
    }

    *ymin = 9e99;
    *ymax = 0;

    if (!this->scale_ymin && !scale_ymax) {
        *ymin = this->p.ymin;
        *ymax = this->p.ymax;

        return;
    }

    for (auto pair: this->chan_curves) {
        double ymin_ = 9e99;
        double ymax_ = 0;

        this->get_ylims(pair.first, xmin, xmax, &ymin_, &ymax_);

        if (ymin_ < *ymin) {
            *ymin = ymin_;
        }

        if (ymax_ > *ymax) {
           *ymax = ymax_;
        }
    }

    if (!this->scale_ymin) {
        *ymin = this->p.ymin;
    }

    if (!this->scale_ymax) {
        *ymax = this->p.ymax;
    }

    if (this->keep_y_const_size) {
        if (scale_ymin && !scale_ymax) {
            *ymax = this->y_const_size - *ymin;
        } else if (scale_ymax && !scale_ymin) {
            *ymin = *ymax - this->y_const_size;
        }
    }
}

void DataView::on_new_data(data_update data)
{
    if (this->chan_curves.count(data.chan_num) == 0) {
        this->chan_curves[data.chan_num] = this->make_new_curve(data.chan_num);
        this->chan_curves[data.chan_num]->attach(this->plot);
    }

    if (data.xs.size() != 0 && data.ys.size() != 0)
        this->chan_curves[data.chan_num]->setSamples(&data.xs[0], &data.ys[0], data.ys.size());

    return;
}

