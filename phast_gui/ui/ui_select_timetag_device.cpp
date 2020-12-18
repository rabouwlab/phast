/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "ui_select_timetag_device.h"
#include "ui_ui_select_timetag_device.h"

#include <QPushButton>

ui_select_timetag_device::ui_select_timetag_device(QWidget *parent,
                                                   const std::vector<timetag_device>& devices) :
    QDialog(parent),
    ui(new Ui::ui_select_timetag_device),
    devices(devices)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    QPushButton* ok_btn = this->ui->buttonBox->button(QDialogButtonBox::Ok);
    ok_btn->setEnabled(false);

    this->sel_device.any_item_selected = false;
    this->add_devices();
}

void ui_select_timetag_device::add_devices()
{
    for (uint64_t i = 0; i < this->devices.size(); i++) {
        if (!this->devices.at(i).is_real_device) {
            this->ui->lw_items->addItem((QString)(this->devices[i].device_name.c_str()) + QString(" (ID ") + QString::number(this->devices[i].device_ID) + ") (fake)");
            continue;
        }

        this->ui->lw_items->addItem((QString)(this->devices[i].device_name.c_str()) + QString(" (ID ") + QString::number(this->devices[i].device_ID) + ")");
    }
}

ui_select_timetag_device::~ui_select_timetag_device()
{
    delete ui;
}

void ui_select_timetag_device::on_lw_items_itemChanged(QListWidgetItem *item)
{
    QPushButton* ok_btn = this->ui->buttonBox->button(QDialogButtonBox::Ok);

    for (uint64_t i = 0; i < this->ui->lw_items->count(); i++) {
        if (this->ui->lw_items->item(i) == item) {
            this->sel_device.device = this->devices[i];
            this->sel_device.any_item_selected = true;
            ok_btn->setEnabled(true);
            return;
        }
    }

    this->sel_device.any_item_selected = false;
    ok_btn->setEnabled(false);
}

ui_select_timetag_device::selected_device ui_select_timetag_device::DeviceSelected() const
{
    return this->sel_device;
}

void ui_select_timetag_device::on_lw_items_itemClicked(QListWidgetItem *item)
{
    QPushButton* ok_btn = this->ui->buttonBox->button(QDialogButtonBox::Ok);

    for (uint64_t i = 0; i < this->ui->lw_items->count(); i++) {
        if (this->ui->lw_items->item(i) == item) {
            this->sel_device.device = this->devices[i];
            this->sel_device.any_item_selected = true;
            ok_btn->setEnabled(true);
            return;
        }
    }

    this->sel_device.any_item_selected = false;
    ok_btn->setEnabled(false);
}
