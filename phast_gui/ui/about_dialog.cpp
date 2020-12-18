/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "about_dialog.h"
#include "ui_about_dialog.h"
#include "support/global.h"

about_dialog::about_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about_dialog)
{
    ui->setupUi(this);

    this->ui->lbl_author->setText(QString("Author: ") + QString(PHAST_AUTHOR));
    this->ui->lbl_author_email->setText(PHAST_AUTHOR_EMAIL);
    this->ui->lbl_version->setText(QString("Version ") + QString(PHAST_GUI_VERSION_STR));
}

about_dialog::~about_dialog()
{
    delete ui;
}
