/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "appchannelinfo_form.h"
#include "ui_appchannelinfo_form.h"

appchannelinfo_form::appchannelinfo_form(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::appchannelinfo_form),
    chan_ID(0),
    ret()
{
    this->ret.ID = 0;
    this->ret.micro_display_delaytime = 0;
    this->ret.additional_sync_divider = 1;
    this->ret.channel_has_microtime = false;
    this->ret.has_pulses_channel = false;
    this->ret.corresponding_pulses_channel = 0;
    this->ret.is_pulses_channel = false;
    this->ret.num_dependent_chans = 0;

    ui->setupUi(this);
}

appchannelinfo_form::~appchannelinfo_form()
{
    delete ui;
}

void appchannelinfo_form::on_spinBox_valueChanged(int arg1)
{
    this->ret.ID = arg1;
}

void appchannelinfo_form::on_buttonBox_accepted()
{

}

chaninfo appchannelinfo_form::ChannelInfo() const
{
    return this->ret;
}
