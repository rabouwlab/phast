/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "create_trace_ui_dialog.h"
#include "ui_create_trace_ui_dialog.h"

create_trace_ui_dialog::create_trace_ui_dialog(QWidget *parent,
                                               const std::vector<chan_id>& channels,
                                               uint64_t time_unit_hundreths_of_ps) :
    QDialog(parent),
    channels(channels),
    time_unit(time_unit_hundreths_of_ps),
    ui(new Ui::create_trace_ui_dialog)
{
    ui->setupUi(this);

    this->p.bin_width = 1;
    this->p.sum_values = false;

    this->setup_chanlist();
}

create_trace_ui_dialog::~create_trace_ui_dialog()
{
    delete ui;
}

void create_trace_ui_dialog::setup_chanlist()
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

void create_trace_ui_dialog::on_list_chans_itemClicked(QListWidgetItem *item)
{
    bool pressed = (item->checkState() == Qt::Checked);

    chan_id chan_num = item->text().toULongLong();

    if (pressed) {
        this->p.channels.push_back(chan_num);
    } else {
        auto it = std::find(this->p.channels.begin(), this->p.channels.end(), chan_num);

        if(it != this->p.channels.end())
            this->p.channels.erase(it);
    }
}

void create_trace_ui_dialog::on_cnt_num_bins_valueChanged(double value)
{
    if (value < 1) {
        return;
    }

    this->p.bin_width = (uint64_t)value;
}

void create_trace_ui_dialog::on_chk_sum_chans_toggled(bool checked)
{
    this->p.sum_values = checked;
}

create_trace_ui_dialog::init_vals create_trace_ui_dialog::GetValues() const
{
    return this->p;
}
