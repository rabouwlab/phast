#ifndef ONLINE_DATASAVING_UI_H
#define ONLINE_DATASAVING_UI_H

#include <QDialog>
#include <QTime>
#include <QTimer>

#include "interfaces/isettingscontainer.h"
#include "interfaces/itempdatastorage.h"

#include "writer_coord.h"
#include "support/small_tcspc_io2.h"

namespace Ui {
class online_datasaving_ui;
}

class online_datasaving_ui : public QDialog
{
    Q_OBJECT

private:
    double time_unit;
    QString device_type;
    QString abs_path;
    QString filename;

    bool ready_to_save;
    bool is_saving;
    double timer_interval;
    QTime save_start_time;
    QTimer timer;

    bool stop_after_max_time;
    bool time_changed;
    QTime max_time;

public:
    online_datasaving_ui(const online_datasaving_ui&) = delete;
    online_datasaving_ui& operator=(const online_datasaving_ui&) = delete;

    explicit online_datasaving_ui(double time_unit,
                                  QString device_type,
                                  QWidget *parent,
                                  ISettingsContainer* settings,
                                  ITempDataStorage* data_storage);
    ~online_datasaving_ui();

    bool IsCurrentlySaving() const;

signals:
    void saving_started(QString filedir, QString filename);
    void saving_ended();
    void request_clear_views();

public slots:
    void new_temp_datastorage(ITempDataStorage* storage);

private slots:
    void on_btn_start_stop_clicked();

    void on_btn_browse_clicked();
    void update_timer();

    void on_chk_maxtime_toggled(bool checked);

    void on_te_maxtime_timeChanged(const QTime &time);

private:
    Ui::online_datasaving_ui *ui;

    ISettingsContainer* settings;
    ITempDataStorage* data_storage;
    writer_coord<small_tcspc_io2>* wc;

    void get_new_saveloc();
    void quit_saving();
    void start_saving();

    void stop_saving();
    void initiate_saving();
};

#endif // ONLINE_DATASAVING_UI_H
