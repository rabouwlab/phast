/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef WIDGETPLOTSETTINGS_H
#define WIDGETPLOTSETTINGS_H

#include <QDialog>

#include "../support/plotparams.h"
#include "../interfaces/ibinningclass.h"

#include <mutex>

class DataView;
class IDataPlot;

namespace Ui {
class WidgetPlotSettings;
}

class WidgetPlotSettings : public QDialog
{
    Q_OBJECT

private:
    IDataPlot* view;
    IBinningClass* plot;
    plot_params p;

    QMetaObject::Connection cur_view_connection;
    QMetaObject::Connection cur_plot_connection;

    void use_new_params(plot_params p);

    std::mutex view_mutex;
    std::mutex plot_mutex;

signals:
    void params_changed(plot_params new_value);

    void bin_width_changed(uint64_t bin_width);

    void clear_all_views();

public:
    WidgetPlotSettings(const WidgetPlotSettings&) = delete;
    WidgetPlotSettings& operator=(const WidgetPlotSettings&) = delete;
    explicit WidgetPlotSettings(QWidget *parent = nullptr);
    ~WidgetPlotSettings();

public slots:
    void new_view(IDataPlot *value);
    void new_plot(IBinningClass* plot);
    void view_deleted(IDataPlot *value);
    void plot_deleted(IBinningClass* plot);

private slots:
    void on_spb_xmin_valueChanged(double arg1);

    void on_chk_xmax_auto_clicked(bool checked);

    void on_spb_xmax_valueChanged(double arg1);

    void on_spb_ymin_valueChanged(double arg1);

    void on_chk_ymax_auto_toggled(bool checked);

    void on_spb_ymax_valueChanged(double arg1);

    void on_chk_ylog_toggled(bool checked);

    void on_chk_x_const_size_toggled(bool checked);

    void on_cnt_x_const_width_valueChanged(double value);

    void on_chk_y_const_size_toggled(bool checked);

    void on_cnt_y_const_width_valueChanged(double value);

    void on_chk_auto_xmin_toggled(bool checked);

    void on_cnt_bin_width_valueChanged(double value);

    void on_chk_xlog_toggled(bool checked);

    void on_btn_clear_all_views_clicked();

    void on_chk_auto_ymin_toggled(bool checked);

private:
    Ui::WidgetPlotSettings *ui;
};

#endif // WIDGETPLOTSETTINGS_H
