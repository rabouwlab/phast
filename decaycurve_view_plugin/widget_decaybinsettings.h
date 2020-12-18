/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef WIDGET_DECAYBINSETTINGS_H
#define WIDGET_DECAYBINSETTINGS_H

#include <QDialog>
#include <QListWidgetItem>

#include <phast_gui/support/typedefs.h>

class decaycurve_calc;

namespace Ui {
class widget_decaybinsettings;
}

class widget_decaybinsettings : public QDialog
{
    Q_OBJECT

public:
    struct decaybinsettings
    {
        uint64_t bin_size;
        double bin_low;
        double bin_high;
        std::vector<chan_id> channels;
        bool sum_channels;
    };

    widget_decaybinsettings(const widget_decaybinsettings&) = delete;
    widget_decaybinsettings& operator=(const widget_decaybinsettings&) = delete;
    explicit widget_decaybinsettings(double time_unit, QWidget *parent,
                                     const std::vector<chan_id>& channels);
    ~widget_decaybinsettings();

    decaybinsettings GetSettings() const;

private slots:
    void on_cnt_bin_low_valueChanged(double value);

    void on_cnt_bin_high_valueChanged(double value);

    void on_cnt_num_bins_valueChanged(double value);

    void on_list_chans_itemClicked(QListWidgetItem *item);

    void on_btn_OK_clicked();

    void on_btn_cancel_clicked();

    void on_chk_sum_toggled(bool checked);

private:
    double time_unit;
    Ui::widget_decaybinsettings *ui;
    std::vector<chan_id> channels;

    decaybinsettings settings;

    void setup_chanlist();
};
#endif // WIDGET_DECAYBINSETTINGS_H
