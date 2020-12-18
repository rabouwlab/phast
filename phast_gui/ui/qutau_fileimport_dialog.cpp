/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "qutau_fileimport_dialog.h"
#include "ui_qutau_fileimport_dialog.h"

#include <QFileDialog>

qutau_fileimport_dialog::qutau_fileimport_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qutau_fileimport_dialog)
{
    ui->setupUi(this);
    this->info.pulses_chan_defined = false;
    this->info.filename = "";
    this->info.pulses_channel = 0;
    this->info.trash_useless_pulses = false;

    ui->chk_neglect->setDisabled(true);
    ui->cnt_pulses_chan_ID->setDisabled(true);
}

qutau_fileimport_dialog::~qutau_fileimport_dialog()
{
    delete ui;
}

void qutau_fileimport_dialog::on_chk_have_pulses_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked) {
        ui->chk_neglect->setDisabled(false);
        ui->cnt_pulses_chan_ID->setDisabled(false);

        this->info.pulses_chan_defined = true;
    } else {
        ui->chk_neglect->setDisabled(true);
        ui->cnt_pulses_chan_ID->setDisabled(true);

        this->info.pulses_chan_defined = false;
    }
}

void qutau_fileimport_dialog::on_cnt_pulses_chan_ID_valueChanged(int arg1)
{
    this->info.pulses_channel = arg1;
}

void qutau_fileimport_dialog::on_chk_neglect_stateChanged(int arg1)
{
    this->info.trash_useless_pulses = (arg1 == Qt::Checked) ? true : false;
}

void qutau_fileimport_dialog::on_buttonBox_accepted()
{
    this->info.filename = QFileDialog::getOpenFileName(this, "Select file to import.", "", "QuTau files (*.qtau)");

    QDialog::accepted();
}
