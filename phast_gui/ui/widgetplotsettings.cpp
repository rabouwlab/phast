/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "widgetplotsettings.h"
#include "ui_widgetplotsettings.h"

#include "../support/dataview.h"

#include <iostream>

WidgetPlotSettings::WidgetPlotSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WidgetPlotSettings),
    view(nullptr),
    plot(nullptr)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint);

    this->p.xmax = 0;
    this->p.xmin = 0;
    this->p.ymax = 0;
    this->p.ymin = 0;
    this->p.xmax_autoscale = false;
    this->p.xmin_autoscale = false;
    this->p.x_log_scale = false;

    this->p.ymax_autoscale = false;
    this->p.ymin_autoscale = false;
    this->p.y_log_scale = false;
    this->p.keep_x_const_size = false;
    this->p.keep_y_const_size = false;
    this->p.y_const_size = 1;
    this->p.x_const_size = 1;
}

WidgetPlotSettings::~WidgetPlotSettings()
{
    if (this->view != nullptr) {
        disconnect(this, &WidgetPlotSettings::params_changed, this->view, &IDataPlot::set_plot_params);
    }

    delete ui;
}

void WidgetPlotSettings::view_deleted(IDataPlot* value)
{
    std::lock_guard<std::mutex> l(this->view_mutex);

    if (value == this->view) {
        disconnect(this, &WidgetPlotSettings::params_changed, this->view, &IDataPlot::set_plot_params);
        this->view = nullptr;
    }
}

void WidgetPlotSettings::plot_deleted(IBinningClass* plot)
{
    std::lock_guard<std::mutex> l(this->plot_mutex);

    if (plot == this->plot) {
        disconnect(this->cur_plot_connection);
        this->plot = nullptr;
    }
}

void WidgetPlotSettings::use_new_params(plot_params p)
{
    this->ui->spb_xmin->setValue(p.xmin);
    this->ui->spb_xmax->setValue(p.xmax);
    this->ui->spb_ymin->setValue(p.ymin);
    this->ui->spb_ymax->setValue(p.ymax);

    this->ui->chk_auto_xmin->setChecked(p.xmin_autoscale);
    this->ui->chk_xmax_auto->setChecked(p.xmax_autoscale);

    this->ui->chk_auto_ymin->setChecked(p.ymin_autoscale);
    this->ui->chk_ymax_auto->setChecked(p.ymax_autoscale);

    this->ui->chk_xlog->setChecked(p.x_log_scale);
    this->ui->chk_ylog->setChecked(p.y_log_scale);

    this->ui->chk_x_const_size->setChecked(p.keep_x_const_size);
    this->ui->chk_y_const_size->setChecked(p.keep_y_const_size);

    this->ui->cnt_x_const_width->setValue(p.x_const_size);
    this->ui->cnt_y_const_width->setValue(p.y_const_size);
}

void WidgetPlotSettings::new_view(IDataPlot *value)
{
    std::lock_guard<std::mutex> l(this->view_mutex);

    if (this->view != nullptr) {
        disconnect(this, &WidgetPlotSettings::params_changed, this->view, &IDataPlot::set_plot_params);
    }

    this->view = value;

    this->p = value->get_plot_params();

    this->use_new_params(this->p);

    connect(this, &WidgetPlotSettings::params_changed, this->view, &IDataPlot::set_plot_params);
}

void WidgetPlotSettings::new_plot(IBinningClass *plot)
{
    uint64_t bin_width = 1;
    {
        std::lock_guard<std::mutex> l(this->plot_mutex);

        this->plot = plot;
        bin_width = this->plot->get_bin_width();
    }

    this->ui->cnt_bin_width->setValue((double)bin_width);
}

void WidgetPlotSettings::on_spb_xmin_valueChanged(double arg1)
{
    this->p.xmin = arg1;

    emit params_changed(this->p);
}

void WidgetPlotSettings::on_chk_auto_xmin_toggled(bool checked)
{
    this->p.xmin_autoscale = checked;

    if (this->p.xmin_autoscale && this->p.xmax_autoscale) {
        this->p.keep_x_const_size = false;
        this->ui->chk_x_const_size->setChecked(false);
    }

    emit params_changed(this->p);
}

void WidgetPlotSettings::on_chk_xmax_auto_clicked(bool checked)
{
    this->p.xmax_autoscale = checked;

    emit params_changed(this->p);
}

void WidgetPlotSettings::on_chk_xlog_toggled(bool checked)
{
    this->p.x_log_scale = checked;

    emit params_changed(this->p);
}

void WidgetPlotSettings::on_spb_xmax_valueChanged(double arg1)
{
    this->p.xmax = arg1;

    emit params_changed(this->p);
}

void WidgetPlotSettings::on_chk_auto_ymin_toggled(bool checked)
{
    this->p.ymin_autoscale = checked;

    emit params_changed(this->p);
}

void WidgetPlotSettings::on_spb_ymin_valueChanged(double arg1)
{
    this->p.ymin = arg1;

    emit params_changed(this->p);
}

void WidgetPlotSettings::on_chk_ymax_auto_toggled(bool checked)
{
    this->p.ymax_autoscale = checked;

    emit params_changed(this->p);
}

void WidgetPlotSettings::on_spb_ymax_valueChanged(double arg1)
{
    this->p.ymax = arg1;

    emit params_changed(this->p);
}

void WidgetPlotSettings::on_chk_ylog_toggled(bool checked)
{
    this->p.y_log_scale = checked;

    emit params_changed(this->p);
}

void WidgetPlotSettings::on_chk_x_const_size_toggled(bool checked)
{
    this->p.keep_x_const_size = checked;

    this->ui->cnt_x_const_width->setEnabled(checked);
    emit params_changed(this->p);
}

void WidgetPlotSettings::on_cnt_x_const_width_valueChanged(double value)
{
    if (value == 0 || value < 0) {
        return;
    }

    this->p.x_const_size = value;

    emit params_changed(this->p);
}

void WidgetPlotSettings::on_chk_y_const_size_toggled(bool checked)
{
    this->p.keep_y_const_size = checked;

    this->ui->cnt_y_const_width->setEnabled(checked);
    emit params_changed(this->p);
}

void WidgetPlotSettings::on_cnt_y_const_width_valueChanged(double value)
{
    if (value == 0 || value < 0) {
        return;
    }

    this->p.y_const_size = value;

    emit params_changed(this->p);
}

void WidgetPlotSettings::on_cnt_bin_width_valueChanged(double value)
{
    if (value == 0 || value < 0) {
        return;
    }

    {
        std::lock_guard<std::mutex> l(this->plot_mutex);

        if (this->plot != nullptr)
            this->plot->on_new_bin_width((uint64_t)value);
    }

    emit bin_width_changed((uint64_t)value);
}

void WidgetPlotSettings::on_btn_clear_all_views_clicked()
{
    emit clear_all_views();
}
