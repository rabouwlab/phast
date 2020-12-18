/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "timetagger_infoscreen.h"
#include "ui_timetagger_infoscreen.h"

timetagger_infoscreen::timetagger_infoscreen(QWidget *parent, bool connected, QString dev_type, double timeunit_seconds) :
    QDialog(parent),
    ui(new Ui::timetagger_infoscreen)
{
    ui->setupUi(this);

    this->ui->lbl_connected->setText( (connected) ? "Yes" : "No");
    this->ui->lbl_dev_type->setText(dev_type);
    this->ui->lbl_unit->setText(QString::number(timeunit_seconds) + " s");
}

timetagger_infoscreen::~timetagger_infoscreen()
{
    delete ui;
}
