/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef CHANNELOVERVIEWUI_H
#define CHANNELOVERVIEWUI_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>

#include <qwt/qwt_counter.h>

#include <vector>

#include "../support/chaninfo.h"
#include "offline_online.h"

class IPermanentDataStorage;
class ISettingsContainer;
class MainWindow;

namespace Ui {
class ChannelOverviewUI;
}

class ChannelOverviewUI : public QDialog
{
    Q_OBJECT

private:
    struct chan_widgets {
        uint64_t channel_number;
        QLabel* chan_num;
        QComboBox* combo;
        QLabel* is_pulses;
        QwtCounter* cnt_additional_sync_div;
        QwtCounter* cnt_delay;
    };

    void update_all_pulsechan_dropdowns();
    void update_chan_pulsechan_dropdown(uint64_t chan_id);

public:
    ChannelOverviewUI(const ChannelOverviewUI&) = delete;
    ChannelOverviewUI& operator=(const ChannelOverviewUI&) = delete;

    explicit ChannelOverviewUI(MainWindow *parent, IPermanentDataStorage* data, ISettingsContainer* settings);

    ~ChannelOverviewUI();

    const std::map<uint64_t, chaninfo> &ChannelInfo() const;

private slots:
    void on_btn_add_channel_clicked();
    void delay_changed(double new_val);
    void additional_sync_div_changed(double new_val);

private:
    Ui::ChannelOverviewUI *ui;
    ISettingsContainer* settings;
    IPermanentDataStorage* data;

    std::map<uint64_t, chaninfo> chan_info;
    std::map<uint64_t, chan_widgets> channels_widgets;

    bool updating_prefs;

    void pulses_chan_changed(uint64_t sending_chan_num, int64_t combobox_index);


    void init_ui_table();
    void add_channel_widgets(chaninfo ci);
    void use_channel_prefs(chaninfo ci);
    void set_channel_as_pulses_channel(uint64_t index, bool is_pulses);

    LineState state;
};

#endif // CHANNELOVERVIEWUI_H
