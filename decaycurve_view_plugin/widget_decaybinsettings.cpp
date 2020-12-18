/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "widget_decaybinsettings.h"
#include "ui_widget_decaybinsettings.h"

#include "decaycurve_calc.h"
#include <QListWidgetItem>
#include <iostream>

widget_decaybinsettings::widget_decaybinsettings(double time_unit,
                                                 QWidget *parent,
                                                 const std::vector<chan_id>& channels) :
    QDialog(parent),
    time_unit(time_unit),
    channels(channels),
    ui(new Ui::widget_decaybinsettings)
{
    ui->setupUi(this);

    settings.bin_low = 0;
    settings.bin_high = 10000;
    settings.bin_size = 1;
    settings.sum_channels = false;
    this->setup_chanlist();
}

widget_decaybinsettings::decaybinsettings widget_decaybinsettings::GetSettings() const
{
    return this->settings;
}

widget_decaybinsettings::~widget_decaybinsettings()
{
    delete ui;
}

void widget_decaybinsettings::setup_chanlist()
{
    // Add any items we did not have yet
    for (chan_id c : this->channels) {
        QListWidgetItem* item = new QListWidgetItem;
        item->setData( Qt::DisplayRole, "text" );
        item->setText(QString::number(c));
        item->setData( Qt::CheckStateRole, Qt::Unchecked );
        item->setCheckState(Qt::Unchecked);
        this->ui->list_chans->addItem( item );
    }
}

void widget_decaybinsettings::on_cnt_bin_low_valueChanged(double value)
{
    this->settings.bin_low = value*1e-9/this->time_unit;
}

void widget_decaybinsettings::on_cnt_bin_high_valueChanged(double value)
{
    // Convert from nanosecond to elemental time units
    this->settings.bin_high = value*1e-9/this->time_unit;
}

void widget_decaybinsettings::on_cnt_num_bins_valueChanged(double value)
{
    this->settings.bin_size = value;
}

void widget_decaybinsettings::on_list_chans_itemClicked(QListWidgetItem *item)
{
    bool pressed = (item->checkState() == Qt::Checked);

    uint64_t chan_num = item->text().toULongLong();

    if (pressed) {
        this->settings.channels.push_back(chan_num);
    } else {
        auto it = std::find(this->settings.channels.begin(), this->settings.channels.end(), chan_num);

        if(it != this->settings.channels.end())
            this->settings.channels.erase(it);
    }
}

void widget_decaybinsettings::on_btn_OK_clicked()
{
    this->accept();
}

void widget_decaybinsettings::on_btn_cancel_clicked()
{
    this->reject();
}

void widget_decaybinsettings::on_chk_sum_toggled(bool checked)
{
    this->settings.sum_channels = checked;
}
