/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef APPCHANNELINFO_FORM_H
#define APPCHANNELINFO_FORM_H

#include <QDialog>

#include "../support/chaninfo.h"

namespace Ui {
class appchannelinfo_form;
}

class appchannelinfo_form : public QDialog
{
    Q_OBJECT

public:
    appchannelinfo_form(const appchannelinfo_form&) = delete;
    appchannelinfo_form& operator=(const appchannelinfo_form&) = delete;
    explicit appchannelinfo_form(QWidget *parent = nullptr);
    ~appchannelinfo_form();

    chaninfo ChannelInfo() const;
private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_buttonBox_accepted();

private:
    Ui::appchannelinfo_form *ui;

    int chan_ID;
    chaninfo ret;
};

#endif // APPCHANNELINFO_FORM_H
