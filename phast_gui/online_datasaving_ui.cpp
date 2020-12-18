#include "online_datasaving_ui.h"
#include "ui_online_datasaving_ui.h"

#include <QFileDialog>

#include "writer_coord.h"
#include "support/small_tcspc_io2.h"

#include <iostream>
#include <QTimer>

// TODO: handle destruction while saving graciously

online_datasaving_ui::online_datasaving_ui(double time_unit, QString device_type, QWidget *parent, ISettingsContainer *settings, ITempDataStorage *data_storage) :
    QDialog(parent),
    ui(new Ui::online_datasaving_ui),
    time_unit(time_unit),
    device_type(device_type),
    abs_path(),
    filename(),
    ready_to_save(false),
    is_saving(false),
    settings(settings),
    data_storage(data_storage),
    timer(this),
    stop_after_max_time(false),
    time_changed(false)
{
    this->wc = nullptr;

    ui->setupUi(this);
    this->ui->btn_start_stop->setEnabled(false);

    connect(&(this->timer), &QTimer::timeout, this, &online_datasaving_ui::update_timer);
}

void online_datasaving_ui::get_new_saveloc()
{
    QString filename = QFileDialog::getSaveFileName(this, "Choose file save location", QString(), "Simple Small Time-Tagged (*.sstt)");

    if (filename == "")
        return;

    QFileInfo qfi(filename);

    this->abs_path = qfi.absolutePath() + "/";
    this->filename = qfi.fileName();

    this->ui->le_filepath->setText(this->abs_path + this->filename);
    this->ready_to_save = true;
    this->ui->btn_start_stop->setEnabled(true);
}

online_datasaving_ui::~online_datasaving_ui()
{
    delete ui;
}

void online_datasaving_ui::new_temp_datastorage(ITempDataStorage *storage)
{
    if (this->is_saving) {
        // This should not happen! We have a problem.
        std::cout << "ERROR: temp data storage changed while saving online data!" << std::endl;

        this->quit_saving();

        return;
    }

    this->data_storage = storage;
}

void online_datasaving_ui::start_saving()
{
    this->wc = new writer_coord<small_tcspc_io2>(this->time_unit, this->device_type, this->filename, this->abs_path, this->settings->GetChannelInfos(), false);
    wc->RegisterListeners(this->data_storage);
}

void online_datasaving_ui::quit_saving()
{
    if (this->wc == nullptr) {
        return;
    }

    this->wc->Finalize();
    delete this->wc;
}

void online_datasaving_ui::update_timer()
{
    int n_ms = this->save_start_time.elapsed();

    QTime t = QTime(0, 0);
    t = t.addMSecs(n_ms);
    QString txt = t.toString("hh:mm:ss");
    this->ui->lbl_time_elapsed->setText(txt);

    if (this->stop_after_max_time && this->time_changed) {
        if (t >= this->max_time) {
            this->stop_saving();
        }
    }
}

void online_datasaving_ui::initiate_saving()
{
    this->save_start_time.start();
    this->timer.start(100);

    this->start_saving();
    this->ui->btn_start_stop->setText("Stop saving");
    this->is_saving = true;

    emit saving_started(this->abs_path, this->filename);
}

void online_datasaving_ui::stop_saving()
{
    this->timer.stop();

    this->quit_saving();
    this->ui->btn_start_stop->setText("Start saving");
    this->is_saving = false;

    this->abs_path = "";
    this->filename = "";
    this->ui->le_filepath->setText("");
    this->ready_to_save = false;
    this->ui->btn_start_stop->setEnabled(false);

    emit saving_ended();
}

void online_datasaving_ui::on_btn_start_stop_clicked()
{
    if (this->is_saving) {
        this->stop_saving();
    } else if (this->ready_to_save && !this->is_saving) {
        this->initiate_saving();
    }
}

bool online_datasaving_ui::IsCurrentlySaving() const
{
    return this->is_saving;
}

void online_datasaving_ui::on_btn_browse_clicked()
{
    this->get_new_saveloc();
}

void online_datasaving_ui::on_chk_maxtime_toggled(bool checked)
{
    if (this->is_saving && QTime(0,0).addMSecs(this->save_start_time.elapsed()) >= this->max_time) {
        this->time_changed = false;
    }

    this->stop_after_max_time = checked;
}

void online_datasaving_ui::on_te_maxtime_timeChanged(const QTime &time)
{
    if (this->is_saving && QTime(0,0).addMSecs(this->save_start_time.elapsed()) >= this->max_time) {
        this->time_changed = false;
    } else {
        this->time_changed = true;
    }

    this->max_time = time;
}
