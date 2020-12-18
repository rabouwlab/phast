/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "mainplot.h"

#include <qwt/qwt_plot_canvas.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_scale_engine.h>
#include <qwt/qwt_symbol.h>
#include <qwt/qwt_plot_marker.h>
#include <qwt/qwt_plot_zoomer.h>
#include <qwt/qwt_picker_machine.h>
#include <qwt/qwt_plot_layout.h>

#include "customscaledraw.h"

#include <iostream>

void MainPlot::set_x_logscale(bool log_on)
{
    if (log_on) {
        this->setAxisScaleEngine(QwtPlot::xBottom, new QwtLogScaleEngine);
    } else {
        this->setAxisScaleEngine(QwtPlot::xBottom, new QwtLinearScaleEngine);
    }

    this->xlog = log_on;
}

void MainPlot::set_y_logscale(bool log_on)
{
    if (log_on) {
        this->setAxisScaleEngine(QwtPlot::yLeft, new QwtLogScaleEngine);
    } else {
        this->setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine);
    }

    this->ylog = log_on;
}

void MainPlot::change_axis_min(int axisId, double value)
{
    double curmax = this->axisScaleDiv(axisId).upperBound();

    this->setAxisScale(axisId, value, curmax);
}

void MainPlot::change_axis_max(int axisId, double value)
{
    double curmin = this->axisScaleDiv(axisId).lowerBound();

    this->setAxisScale(axisId, curmin, value);
}

void MainPlot::change_axis_limits(int axisId, double min, double max)
{
    this->setAxisScale(axisId, min, max);
}

double MainPlot::get_axis_min(int axisId) const
{
    return this->axisScaleDiv(axisId).lowerBound();
}

double MainPlot::get_axis_max(int axisId) const
{
    return this->axisScaleDiv(axisId).upperBound();
}

void MainPlot::set_x_min(double value)
{
    this->change_axis_min(QwtPlot::xBottom, value);
}

void MainPlot::set_x_max(double value)
{
    this->change_axis_max(QwtPlot::xBottom, value);
}

void MainPlot::set_x_lims(double xmin, double xmax)
{
    this->change_axis_limits(QwtPlot::xBottom, xmin, xmax);
}

void MainPlot::set_y_lims(double ymin, double ymax)
{
    this->change_axis_limits(QwtPlot::yLeft, ymin, ymax);
}

void MainPlot::set_y_min(double value)
{
    this->change_axis_min(QwtPlot::yLeft, value);
}

void MainPlot::set_y_max(double value)
{
    this->change_axis_max(QwtPlot::yLeft, value);
}

double MainPlot::get_x_min() const
{
    return this->get_axis_min(QwtPlot::xBottom);
}

double MainPlot::get_x_max() const
{
    return this->get_axis_max(QwtPlot::xBottom);
}

double MainPlot::get_y_min() const
{
    return this->get_axis_min(QwtPlot::yLeft);
}

double MainPlot::get_y_max() const
{
    return this->get_axis_max(QwtPlot::yLeft);
}

bool MainPlot::get_x_log() const
{
    return this->xlog;
}

bool MainPlot::get_y_log() const
{
    return this->ylog;
}

MainPlot::~MainPlot()
{
    delete this->d_marker1;
    delete this->d_marker2;
    delete this->picker;
    delete this->zoomer;
}

MainPlot::MainPlot( QWidget *parent ):
    QwtPlot( parent ),
    d_marker1(new QwtPlotMarker()),
    d_marker2(new QwtPlotMarker()),
    zoomer(nullptr),
    picker(nullptr),
    xlog(false),
    ylog(false)
{
    this->setAutoReplot( false );

    this->setTitle("");

    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setBorderRadius(0);

    this->setCanvas(canvas);
    setCanvasBackground( QColor::fromRgb(10, 10, 10) );

    // legend
    QwtLegend *legend = new QwtLegend;
    this->insertLegend( legend, QwtPlot::BottomLegend );

    // marker
    d_marker1->setValue( 0.0, 0.0 );
    d_marker1->setLineStyle( QwtPlotMarker::VLine );
    d_marker1->setLabelAlignment( Qt::AlignRight | Qt::AlignBottom );
    d_marker1->setLinePen( Qt::green, 0, Qt::DashDotLine );
    d_marker1->attach( this );

    d_marker2->setLineStyle( QwtPlotMarker::HLine );
    d_marker2->setLabelAlignment( Qt::AlignRight | Qt::AlignBottom );
    d_marker2->setLinePen( QColor( 200, 150, 0 ), 0, Qt::DashDotLine );
    d_marker2->setSymbol( new QwtSymbol( QwtSymbol::Diamond,
        QColor( Qt::yellow ), QColor( Qt::green ), QSize( 8, 8 ) ) );
    d_marker2->attach( this );

    this->zoomer = new QwtPlotZoomer(QwtPlot::xBottom, QwtPlot::yLeft, this->canvas(), true);
    this->zoomer->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier);
    this->zoomer->setMousePattern(QwtEventPattern::MouseSelect3, Qt::RightButton);
    this->zoomer->setRubberBand(QwtPicker::RectRubberBand);
    this->zoomer->setRubberBandPen(QColor(Qt::green));
    this->zoomer->setTrackerMode(QwtPicker::AlwaysOff);
    this->zoomer->setTrackerPen(QColor(Qt::white));
    this->zoomer->setZoomBase(QRectF(0,0, 1e-9, 1e-9));
    this->picker = new QwtPlotPicker( QwtPlot::xBottom, QwtPlot::yLeft,
                                      QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn,
                                      this->canvas() );

    this->picker->setStateMachine(new QwtPickerTrackerMachine());
    this->picker->setRubberBandPen(QColor( Qt::green));
    this->picker->setRubberBand(QwtPlotPicker::CrossRubberBand);
    this->picker->setTrackerMode(QwtPicker::AlwaysOn);
    this->picker->setTrackerPen(QColor(Qt::white));

    this->axisScaleEngine(QwtPlot::xBottom)->setAttribute(QwtScaleEngine::Floating, true);
    this->plotLayout()->setAlignCanvasToScales(true);

    this->setAxisScaleDraw(QwtPlot::xBottom, new CustomScaleDraw(true));
    this->setAxisScaleDraw(QwtPlot::yLeft, new CustomScaleDraw);
    setAutoReplot( false );
}
