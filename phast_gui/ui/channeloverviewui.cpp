/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "channeloverviewui.h"
#include "ui_channeloverviewui.h"
#include "mainwindow.h"

#include "appchannelinfo_form.h"
#include <vector>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QtGlobal>
#include <qwt/qwt_counter.h>
#include <QMessageBox>

void ChannelOverviewUI::init_ui_table()
{
    QLabel* lbl_chan = new QLabel("Chan #", this);
    QLabel* lbl_is_pulses_channel = new QLabel("Is pulses?", this);
    QLabel* lbl_pulses_channel = new QLabel("Pulses chan.", this);
    QLabel* lbl_additional_sync_div = new QLabel("Additional sync divider", this);

    // Add the very first row
    // *Widget, row, column, rowspan, colspan
    this->ui->gridLayout->addWidget(lbl_chan, 0, 0, 1, 1, Qt::AlignTop);
    this->ui->gridLayout->addWidget(lbl_pulses_channel, 0, 1, 1, 1, Qt::AlignTop);
    this->ui->gridLayout->addWidget(lbl_is_pulses_channel, 0, 2, 1, 1, Qt::AlignTop);
    this->ui->gridLayout->addWidget(lbl_additional_sync_div, 0, 3, 1, 1, Qt::AlignTop);
}

void ChannelOverviewUI::use_channel_prefs(chaninfo ci)
{
    chan_widgets cw = this->channels_widgets[ci.ID];

    this->set_channel_as_pulses_channel(ci.ID, ci.is_pulses_channel);

    if (ci.has_pulses_channel) {
        int index = cw.combo->findText(QString::number(ci.corresponding_pulses_channel));
        cw.combo->setCurrentIndex(index);

        cw.cnt_delay->setValue(ci.micro_display_delaytime);
        cw.cnt_delay->setEnabled(true);

    }

    cw.cnt_additional_sync_div->setValue(ci.additional_sync_divider);
}

void ChannelOverviewUI::add_channel_widgets(chaninfo ci)
{
    chan_widgets cw;
    cw.channel_number = ci.ID;
    cw.chan_num = new QLabel(QString::number(ci.ID), this);

    cw.combo = new QComboBox(this);
    cw.combo->addItem("None");

    cw.combo->setCurrentIndex(0);

    cw.is_pulses = new QLabel("No", this);

    uint64_t n_photons = 0;

    cw.cnt_delay = new QwtCounter(this);
    cw.cnt_delay->setMinimum(-9e99);
    cw.cnt_delay->setMaximum(9e99);
    cw.cnt_delay->setNumButtons(0);
    cw.cnt_delay->setMinimumWidth(1);
    cw.cnt_delay->setGeometry(0, 0, 10, 10);
    cw.cnt_delay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    cw.cnt_delay->setEnabled(false);

    cw.cnt_additional_sync_div = new QwtCounter(this);
    cw.cnt_additional_sync_div->setMinimum(1.0);
    cw.cnt_additional_sync_div->setMaximum(9e99);
    cw.cnt_additional_sync_div->setNumButtons(0);
    cw.cnt_additional_sync_div->setMinimumWidth(1);
    cw.cnt_additional_sync_div->setEnabled(true);

    int row = this->ui->gridLayout->rowCount() + 1;
    this->ui->gridLayout->addWidget(cw.chan_num, row, 0, 1, 1, Qt::AlignTop);
    this->ui->gridLayout->addWidget(cw.combo, row, 1, 1, 1, Qt::AlignTop);
    this->ui->gridLayout->addWidget(cw.is_pulses, row, 2, 1, 1, Qt::AlignTop);
    this->ui->gridLayout->addWidget(cw.cnt_additional_sync_div, row, 3, 1, 1, Qt::AlignTop);

    //this->ui->gridLayout->addWidget(cw.num_counts, row, 3, 1, 1, Qt::AlignTop);
    //this->ui->gridLayout->addWidget(cw.avg_cps, row, 4, 1, 1, Qt::AlignTop);
    //this->ui->gridLayout->addWidget(cw.cnt_delay, row, 5, 1, 1, Qt::AlignTop);

    this->channels_widgets[ci.ID] = cw;

    connect(cw.combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](int index){this->pulses_chan_changed(ci.ID, index);});

    // connect(cw.cnt_delay, SIGNAL(valueChanged(double)), this, SLOT(delay_changed(double)));
    connect(cw.cnt_additional_sync_div, SIGNAL(valueChanged(double)), this, SLOT(additional_sync_div_changed(double)));
}

void ChannelOverviewUI::update_all_pulsechan_dropdowns()
{
    for (const auto& pair2 : this->chan_info) {
        this->update_chan_pulsechan_dropdown(pair2.second.ID);
    }
}

void ChannelOverviewUI::update_chan_pulsechan_dropdown(uint64_t chan_id)
{
    this->updating_prefs = true;

    chan_widgets cw = this->channels_widgets.at(chan_id);

    cw.combo->clear();

    cw.combo->addItem("None");

    for (const auto &pair2 : this->chan_info ) {
        if (pair2.second.ID == chan_id)
            continue;

       cw.combo->addItem(QString::number(pair2.second.ID));
    }

    this->updating_prefs = false;
}

ChannelOverviewUI::ChannelOverviewUI(MainWindow *parent, IPermanentDataStorage *data, ISettingsContainer *settings) :
    QDialog(static_cast<QWidget*>(parent)),
    ui(new Ui::ChannelOverviewUI),
    settings(settings),
    data(data),
    chan_info(settings->GetChannelInfos()),
    channels_widgets(),
    updating_prefs(false),
    state((data == nullptr) ? ONLINE : OFFLINE)
{
    ui->setupUi(this);

    connect(this->ui->btn_OK, &QPushButton::clicked, this, &QDialog::accept);
    connect(this->ui->btn_cancel, &QPushButton::clicked, this, &QDialog::reject);

    this->init_ui_table();

    // Add the file info
    for (const auto &pair : settings->GetChannelInfos()) {
        chaninfo ci = pair.second;

        this->add_channel_widgets(ci);
    }

    // Update the possible pulse channels
    this->update_all_pulsechan_dropdowns();

    this->updating_prefs = true;

    for (const auto &pair : settings->GetChannelInfos()) {
        chaninfo ci = pair.second;

        this->use_channel_prefs(ci);
    }

    this->updating_prefs = false;
}

ChannelOverviewUI::~ChannelOverviewUI()
{
    delete ui;
}

const std::map<uint64_t, chaninfo> &ChannelOverviewUI::ChannelInfo() const
{
    return this->chan_info;
}

void ChannelOverviewUI::set_channel_as_pulses_channel(uint64_t index, bool is_pulses)
{
    chan_widgets cw_receiver = this->channels_widgets[index];

    if (is_pulses) {
        this->chan_info.at(index).is_pulses_channel = true;

        cw_receiver.is_pulses->setText("Yes");
        // cw_receiver.combo->setCurrentIndex(0);
        // cw_receiver.combo->setDisabled(true);
    } else {
        cw_receiver.is_pulses->setText("No");
        cw_receiver.combo->setEnabled(true);

        this->chan_info.at(index).is_pulses_channel = false;
    }

}

void ChannelOverviewUI::additional_sync_div_changed(double new_val)
{
    if (updating_prefs)
        return;

    if (new_val < 1)
        return;

    QObject* sender = QObject::sender();

    bool found_id = false;
    uint64_t id = 0;
    chan_widgets w;

    // Now find out what widget sent this
    for (auto pair : this->channels_widgets) {
        id = pair.first;
        w = pair.second;

        if (w.cnt_additional_sync_div == sender) {
            found_id = true;
            break;
        }
    }

    if (!found_id) {
        return;
    }

    uint64_t val = (uint64_t)new_val;

    if (!this->chan_info.at(id).is_pulses_channel && val != 1) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Warning!",
                       "WARNING: you are setting a non-unity sync divider on a channel which is not a pulses channel.\n\n"
                       "THIS WILL LEAD TO DATA LOSS.\n\n"
                       "Are you sure?",
                       QMessageBox::Yes|QMessageBox::No, QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            this->chan_info.at(id).additional_sync_divider = val;
            w.cnt_additional_sync_div->setValue(val);
        } else {
            w.cnt_additional_sync_div->setValue(1);
        }
    } else {
        this->chan_info.at(id).additional_sync_divider = val;
        w.cnt_additional_sync_div->setValue(val);
    }
}

void ChannelOverviewUI::delay_changed(double new_val)
{
    if (updating_prefs)
        return;

    QObject* sender = QObject::sender();

    bool found_id = false;
    uint64_t id = 0;
    chan_widgets w;

    // Now find out what widget sent this
    for (auto pair : this->channels_widgets) {
        id = pair.first;
        w = pair.second;

        if (w.cnt_delay == sender) {
            found_id = true;
            break;
        }
    }

    if (!found_id) {
        return;
    }


    this->chan_info.at(id).micro_display_delaytime = (int64_t)new_val;
    w.cnt_delay->setValue((int64_t)new_val);
}

void ChannelOverviewUI::pulses_chan_changed(uint64_t sending_chan_num, int64_t combobox_index)
{
    if (updating_prefs)
        return;

    chan_widgets cw_sender = this->channels_widgets[sending_chan_num];

     uint64_t target_index = cw_sender.combo->currentText().toULongLong();

    // Check if we changed from pulsing channel
     if (this->chan_info.at(sending_chan_num).has_pulses_channel) {
         // We changed pulsing channel.
         uint64_t prev_pulses = this->chan_info[sending_chan_num].corresponding_pulses_channel;

         this->chan_info[prev_pulses].num_dependent_chans -= 1;

         if (this->chan_info[prev_pulses].num_dependent_chans == 0) {
             this->set_channel_as_pulses_channel(prev_pulses, false);
         }
     }

    if (combobox_index == 0) {
        // We don't have a pulses channel any more
        this->channels_widgets[sending_chan_num].cnt_delay->setEnabled(false);

        this->chan_info[sending_chan_num].corresponding_pulses_channel = 0;
        this->chan_info[sending_chan_num].has_pulses_channel = false;
        this->chan_info[sending_chan_num].channel_has_microtime = false;

        return;
    }

    this->chan_info[sending_chan_num].corresponding_pulses_channel = target_index;
    this->chan_info[sending_chan_num].has_pulses_channel = true;
    this->chan_info[sending_chan_num].channel_has_microtime = true;
    this->chan_info[target_index].num_dependent_chans += 1;

    this->channels_widgets[sending_chan_num].cnt_delay->setEnabled(true);

    this->set_channel_as_pulses_channel(target_index, true);
}

void ChannelOverviewUI::on_btn_add_channel_clicked()
{
    appchannelinfo_form f(this);

    int ret = f.exec();

    if (ret == QDialog::Accepted) {
        chaninfo ci = f.ChannelInfo();

        if (this->chan_info.count(ci.ID) != 0) {
            return;
        }


        this->chan_info[ci.ID] = ci;

        // bool success = this->owner->AddChannel(ci);
        this->add_channel_widgets(ci);

        this->update_all_pulsechan_dropdowns();
        this->use_channel_prefs(ci);
    }
}
