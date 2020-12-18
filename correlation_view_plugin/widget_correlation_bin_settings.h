/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef WIDGET_CORRELATION_BIN_SETTINGS_H
#define WIDGET_CORRELATION_BIN_SETTINGS_H

#include <QDialog>
#include <QListWidgetItem>

#include <phast_gui/support/typedefs.h>

namespace Ui {
class widget_correlation_bin_settings;
}

class widget_correlation_bin_settings : public QDialog
{
    Q_OBJECT

public:
    struct corrbinsettings
    {
        uint64_t bin_size;
        double bin_low;
        double bin_high;
        int64_t left_chan;
        int64_t right_chan;
        bool FCS;
    };

public:
    widget_correlation_bin_settings(const widget_correlation_bin_settings&) = delete;
    widget_correlation_bin_settings& operator=(const widget_correlation_bin_settings&) = delete;
    explicit widget_correlation_bin_settings(double time_unit, QWidget *parent,
                                             const std::vector<chan_id>& channels);
    ~widget_correlation_bin_settings();

    corrbinsettings GetSettings() const;

private slots:
    void on_cnt_bin_low_valueChanged(double value);

    void on_cnt_bin_high_valueChanged(double value);

    void on_cnt_num_bins_valueChanged(double value);

    void on_list_chans_itemClicked(QListWidgetItem *item);

    void on_list_chans_2_itemClicked(QListWidgetItem *item);

    void on_chk_FCS_toggled(bool checked);

private:
    double time_unit;
    Ui::widget_correlation_bin_settings *ui;
    std::vector<chan_id> channels;
    corrbinsettings settings;

    void setup_chanlist();
};

#endif // WIDGET_CORRELATION_BIN_SETTINGS_H
