/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "widget_correlation_bin_settings.h"
#include "ui_widget_correlation_bin_settings.h"

widget_correlation_bin_settings::widget_correlation_bin_settings(double time_unit,
                                                                 QWidget *parent,
                                                                 const std::vector<chan_id>& channels) :
    QDialog(parent),
    time_unit(time_unit),
    ui(new Ui::widget_correlation_bin_settings),
    channels(channels)
{
    ui->setupUi(this);

    this->settings.bin_low = -3000;
    this->settings.bin_high = 3000;

    this->settings.bin_size = (uint64_t)(((this->settings.bin_high - this->settings.bin_low)*1e-9/time_unit)/20000.0);

    if (this->settings.bin_size < 1)
        this->settings.bin_size = 1;

    this->settings.left_chan = -1;
    this->settings.right_chan = -1;
    this->settings.FCS = false;

    this->ui->cnt_bin_high->setValue(this->settings.bin_high);
    this->ui->cnt_bin_low->setValue(this->settings.bin_low);
    this->ui->cnt_num_bins->setValue(this->settings.bin_size);

    this->setup_chanlist();
}

void widget_correlation_bin_settings::setup_chanlist()
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

    for (chan_id c : this->channels) {
        QListWidgetItem* item = new QListWidgetItem;
        item->setData( Qt::DisplayRole, "text" );
        item->setText(QString::number(c));
        item->setData( Qt::CheckStateRole, Qt::Unchecked );
        item->setCheckState(Qt::Unchecked);

        this->ui->list_chans_2->addItem( item );
    }
}

widget_correlation_bin_settings::~widget_correlation_bin_settings()
{
    delete ui;
}

void widget_correlation_bin_settings::on_cnt_bin_low_valueChanged(double value)
{
    if (!this->settings.FCS)
        this->settings.bin_low = value*1e-9/this->time_unit;
    else {
        this->settings.bin_low = value/this->time_unit;

        if (this->settings.bin_low <= 0)
            this->settings.bin_low = 1;
    }
}

void widget_correlation_bin_settings::on_cnt_bin_high_valueChanged(double value)
{
    if (!this->settings.FCS)
        this->settings.bin_high = value*1e-9/this->time_unit;
    else {
        this->settings.bin_high = value/this->time_unit;

        if (this->settings.bin_high <= 0)
            this->settings.bin_high = 1;
    }
}

void widget_correlation_bin_settings::on_cnt_num_bins_valueChanged(double value)
{
    this->settings.bin_size = value;
}

void widget_correlation_bin_settings::on_list_chans_itemClicked(QListWidgetItem *item)
{
    bool pressed = (item->checkState() == Qt::Checked);

    uint64_t chan_num = item->text().toULongLong();

    if (pressed) {
        this->settings.left_chan = chan_num;

        for (uint64_t i = 0; i < this->ui->list_chans->count(); i++) {
            QListWidgetItem* item2 = this->ui->list_chans->item(i);

            if (item2 != item) {
                item2->setCheckState(Qt::Unchecked);
            }
        }
    } else {
        this->settings.left_chan = -1;
    }
}

void widget_correlation_bin_settings::on_list_chans_2_itemClicked(QListWidgetItem *item)
{
    bool pressed = (item->checkState() == Qt::Checked);

    uint64_t chan_num = item->text().toULongLong();

    if (pressed) {
        this->settings.right_chan = chan_num;

        for (uint64_t i = 0; i < this->ui->list_chans_2->count(); i++) {
            QListWidgetItem* item2 = this->ui->list_chans_2->item(i);

            if (item2 != item) {
                item2->setCheckState(Qt::Unchecked);
            }
        }
    } else {
        this->settings.right_chan = -1;
    }
}

widget_correlation_bin_settings::corrbinsettings widget_correlation_bin_settings::GetSettings() const
{
    return this->settings;
}

void widget_correlation_bin_settings::on_chk_FCS_toggled(bool checked)
{
    this->settings.FCS = checked;

    if (this->settings.FCS) {
        this->settings.bin_size = 120;
        this->settings.bin_low = (1e-7)/this->time_unit;
        this->settings.bin_high = (1e0)/this->time_unit;

        this->ui->cnt_bin_high->setValue(this->settings.bin_high*this->time_unit);
        this->ui->cnt_bin_low->setValue(this->settings.bin_low*this->time_unit);
        this->ui->cnt_num_bins->setValue(this->settings.bin_size);

        this->ui->lbl_num_bins->setText("Num bins");
        this->ui->lbl_time_unit_3->setText("s");
        this->ui->lbl_time_unit_4->setText("s");
        this->ui->lbl_time_unit_5->setText("");
    } else {
        this->settings.bin_size = 1;
        this->settings.bin_low = (-3000e-9)/this->time_unit;
        this->settings.bin_high = (3000e-9)/this->time_unit;

        this->settings.bin_size = (uint64_t)((this->settings.bin_high - this->settings.bin_low)/20000.0);

        if (this->settings.bin_size < 1)
            this->settings.bin_size = 1;

        this->ui->cnt_bin_high->setValue(this->settings.bin_high*this->time_unit*1e9);
        this->ui->cnt_bin_low->setValue(this->settings.bin_low*this->time_unit*1e9);
        this->ui->cnt_num_bins->setValue(this->settings.bin_size);

        this->ui->lbl_num_bins->setText("Bin size");
        this->ui->lbl_time_unit_3->setText("ns");
        this->ui->lbl_time_unit_4->setText("ns");
        this->ui->lbl_time_unit_5->setText("t.u.");
    }
}
