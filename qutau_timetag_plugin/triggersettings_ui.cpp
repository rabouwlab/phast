/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "triggersettings_ui.h"
#include "ui_triggersettings_ui.h"

#include <iostream>

#include "chan_trigger_settings.h"

void triggersettings_ui::init_ui_table()
{
    QLabel* lbl_chan = new QLabel("Channel no.", this);
    QLabel* lbl_trigger_edge = new QLabel("Trigger edge", this);
    QLabel* lbl_term_enabled = new QLabel("Termination enabled", this);
    QLabel* lbl_threshold = new QLabel("Threshold (V)", this);
    QLabel* lbl_divider = new QLabel("Sync divider", this);
    QLabel* lbl_delay_time = new QLabel("Delay time (time units)", this);

    // Add the very first row
    // *Widget, row, column, rowspan, colspan
    this->ui->gridLayout->addWidget(lbl_chan, 0, 0, 1, 1, Qt::AlignTop);
    this->ui->gridLayout->addWidget(lbl_threshold, 0, 1, 1, 1, Qt::AlignTop);
    this->ui->gridLayout->addWidget(lbl_delay_time, 0, 2, 1, 1, Qt::AlignTop);
    this->ui->gridLayout->addWidget(lbl_trigger_edge, 0, 3, 1, 1, Qt::AlignTop);
    this->ui->gridLayout->addWidget(lbl_divider, 0, 4, 1, 1, Qt::AlignTop);
    this->ui->gridLayout->addWidget(lbl_term_enabled, 0, 5, 1, 1, Qt::AlignTop);
}

void triggersettings_ui::add_channel_widgets(chan_trigger_settings chan_info)
{
    chan_widgets cw;
    cw.channel_number = chan_info.ID;
    cw.chan_num = new QLabel(QString::number(chan_info.ID), this);

    cw.delay_time = new QSpinBox(this);
    cw.delay_time->setMinimum(INT_MIN);
    cw.delay_time->setMaximum(INT_MAX);
    cw.delay_time->setValue(0);
    cw.delay_time->setEnabled(false);

    cw.combo_trigger_edge = new QComboBox(this);
    cw.combo_trigger_edge->addItem("Rising");
    cw.combo_trigger_edge->addItem("Falling");
    cw.combo_trigger_edge->setCurrentIndex(0);

    cw.termination_enabled = new QCheckBox(this);
    cw.termination_enabled->setCheckState(Qt::Unchecked);
    cw.termination_enabled->setEnabled(false);

    cw.voltage_threshold = new QDoubleSpinBox(this);
    cw.voltage_threshold->setMinimum(-2);
    cw.voltage_threshold->setMaximum(3);
    cw.voltage_threshold->setValue(0);

    cw.sync_divider = new QComboBox(this);
    cw.sync_divider->addItem("1");
    cw.sync_divider->addItem("128");
    cw.sync_divider->setCurrentIndex(0);

    // TODO: this is only for QuTau...
    if (chan_info.ID != 0) {
        cw.sync_divider->setEnabled(false);
    }

    int row = this->ui->gridLayout->rowCount() + 1;
    this->ui->gridLayout->addWidget(cw.chan_num, row, 0, 1, 1, Qt::AlignTop);
    this->ui->gridLayout->addWidget(cw.voltage_threshold, row, 1, 1, 1, Qt::AlignTop);
    this->ui->gridLayout->addWidget(cw.delay_time, row, 2, 1, 1, Qt::AlignTop);
    this->ui->gridLayout->addWidget(cw.combo_trigger_edge, row, 3, 1, 1, Qt::AlignTop);
    this->ui->gridLayout->addWidget(cw.sync_divider, row, 4, 1, 1, Qt::AlignTop);
    this->ui->gridLayout->addWidget(cw.termination_enabled, row, 5, 1, 1, Qt::AlignTop);


    this->channels_widgets[chan_info.ID] = cw;

    connect(cw.combo_trigger_edge, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](int index){this->trigger_edge_changed(chan_info.ID, index);});

    connect(cw.termination_enabled, &QCheckBox::stateChanged,
            [=](int check_state){this->termination_enabled_changed(chan_info.ID, check_state);});

    connect(cw.voltage_threshold, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [=](double value){this->voltage_threshold_changed(chan_info.ID, value);});

    connect(cw.delay_time, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int value){this->delay_time_changed(chan_info.ID, value);});

    connect(cw.sync_divider, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int index){this->sync_divider_changed(chan_info.ID, index);});
}

void triggersettings_ui::use_channel_prefs(chan_trigger_settings ci)
{
    chan_widgets cw = this->channels_widgets[ci.ID];

    this->set_channel_conditioning_enabled(ci.ID, ci.signal_conditioning_enabled);

    int index = -1;

    if (ci.edge == chan_trigger_settings::RISING) {
        index = cw.combo_trigger_edge->findText("Rising");
    } else if (ci.edge == chan_trigger_settings::FALLING) {
        index = cw.combo_trigger_edge->findText("Falling");
    }

    cw.combo_trigger_edge->setCurrentIndex(index);

    cw.termination_enabled->setChecked(ci.terminate_in_signal_path);

    cw.voltage_threshold->setValue(ci.voltage_threshold);

    cw.delay_time->setValue(ci.delay_time);

    if (ci.sync_divider == 1) {
        index = cw.sync_divider->findText("1");
    } else {
        index = cw.sync_divider->findText("128");
    }

    cw.sync_divider->setCurrentIndex(index);
}

void triggersettings_ui::set_channel_conditioning_enabled(uint64_t chan_ID, bool enabled)
{
    chan_widgets cw = this->channels_widgets[chan_ID];

    cw.combo_trigger_edge->setEnabled(enabled);
    cw.termination_enabled->setEnabled(enabled);
    cw.voltage_threshold->setEnabled(enabled);
}

triggersettings_ui::triggersettings_ui(QWidget *parent, qutau_communicator *tt_comm, const std::map<chan_id, chan_trigger_settings> &chan_settings) :
    QDialog((QWidget*)parent),
    ui(new Ui::triggersettings_ui),
    chan_info(chan_settings),
    original_chan_info(chan_settings),
    tt_comm(tt_comm)
{
    ui->setupUi(this);

    connect(this->ui->btn_OK, &QPushButton::clicked, this, &QDialog::accept);
    connect(this->ui->btn_cancel, &QPushButton::clicked, this, &QDialog::reject);

    this->init_ui_table();

    // Add the file info
    for (const auto &pair : this->chan_info) {
        chan_trigger_settings ci = pair.second;

        this->add_channel_widgets(ci);
    }

    this->updating_prefs = true;

    for (const auto &pair : this->chan_info) {
        chan_trigger_settings ci = pair.second;

        this->use_channel_prefs(ci);
    }

    this->updating_prefs = false;
}

triggersettings_ui::~triggersettings_ui()
{
    delete ui;
}

void triggersettings_ui::trigger_edge_changed(uint64_t chan_ID, int64_t combobox_index)
{
    if (this->updating_prefs)
        return;

    chan_widgets cw = this->channels_widgets[chan_ID];

    QString text = cw.combo_trigger_edge->currentText();

    if (text == "Rising") {
        this->chan_info[chan_ID].edge = chan_trigger_settings::RISING;
    } else if (text == "Falling") {
        this->chan_info[chan_ID].edge = chan_trigger_settings::FALLING;
    }
}

void triggersettings_ui::sync_divider_changed(uint64_t chan_ID, int64_t combobox_index)
{
    if (this->updating_prefs)
        return;

    chan_widgets cw = this->channels_widgets[chan_ID];

    QString text = cw.sync_divider->currentText();
    int new_sync_val = text.toInt();

    this->chan_info[chan_ID].sync_divider = new_sync_val;
}

void triggersettings_ui::termination_enabled_changed(uint64_t chan_ID, int64_t check_state)
{
    if (this->updating_prefs)
        return;

    if (check_state == Qt::Checked) {
        this->chan_info[chan_ID].terminate_in_signal_path = true;
    } else {
        this->chan_info[chan_ID].terminate_in_signal_path = false;
    }
}

void triggersettings_ui::voltage_threshold_changed(uint64_t chan_ID, double new_val)
{
    if (this->updating_prefs)
        return;

    this->chan_info[chan_ID].voltage_threshold = new_val;
}

void triggersettings_ui::delay_time_changed(uint64_t chan_ID, int new_val)
{
    if (this->updating_prefs)
        return;

    this->chan_info[chan_ID].delay_time = new_val;
}

const std::map<chan_id, chan_trigger_settings> &triggersettings_ui::ChannelInfo() const
{
    return this->chan_info;
}

void triggersettings_ui::push_to_device(uint64_t chan_ID)
{
    chan_trigger_settings checked_val = this->tt_comm->UpdateSignalConditioning(chan_ID, this->chan_info[chan_ID]);

    this->chan_info[chan_ID] = checked_val;
}

void triggersettings_ui::on_btn_apply_clicked()
{
    for (auto pair : this->chan_info) {
        chan_id id = pair.first;

        this->push_to_device(id);
    }
}

void triggersettings_ui::on_btn_cancel_clicked()
{
    this->chan_info = this->original_chan_info;

    for (auto pair : this->chan_info) {
        chan_id id = pair.first;

        this->push_to_device(id);
    }

    this->chan_info = this->original_chan_info;

    this->setResult(QDialog::Rejected);
}

void triggersettings_ui::on_btn_OK_clicked()
{
    this->on_btn_apply_clicked();

    this->setResult(QDialog::Accepted);
    this->close();
}
