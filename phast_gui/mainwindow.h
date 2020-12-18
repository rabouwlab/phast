#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "base/base_settingscontainer.h"
// #include "../support/expinfo.h"

#include "offline_online.h"
#include "interfaces/itempdatastorage.h"
#include "online_datasaving_ui.h"

class QwtPlotCurve;
class MainPlot;

class ITimetagPlugin;
class ITimeTaggerCommunicator;
class IDataRouter;
class IPermanentDataStorage;
class WidgetPlotSettings;
class widget_decaybinsettings;
class IDataviewPlugin;

// TODO: check if number of exported photons is correct.

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    double time_unit;
    QString device_descriptor;

    LineState state;
    Base_SettingsContainer settings;
    Ui::MainWindow *ui;

    ITimeTaggerCommunicator* tt_comm;
    ITempDataStorage* data_storage;
    IDataRouter* data_router;

    WidgetPlotSettings* plot_settings_ui;
    QTimer* plot_timer;

    LockFreeQueue<raw_photon_data> queue;
    LockFreeQueue<std::vector<photon_event>*> clean_queue;
    online_datasaving_ui* saving_ui;
    ITimetagPlugin* tt_plugin;

    std::vector<IDataviewPlugin*> dataview_plugins;

public:
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;

    explicit MainWindow(QWidget *parent,
                        ITimetagPlugin* tt_plugin,
                        const std::vector<IDataviewPlugin*>& dataviewplugins);
    ~MainWindow();

private:
    void prep_for_offline();
    bool prep_for_online();

    void clear_app_channel_infos();
    void rebuild_app_channel_infos_from_file();

    bool handle_new_channel_info(chaninfo value);

signals:
    void NewTempDataStorage(ITempDataStorage* storage);

    void clear_data();

private slots:
    void tt_plugin_activated();

    void dataviewplugin_activated(IDataviewPlugin* plugin);

    void on_actionQuTau_file_triggered();

    void on_actionChannels_triggered();

    void on_actionConnection_info_triggered();

    void on_actionExport_as_stcspc_triggered();

    void on_action_sstt_triggered();

    void on_actionData_writer_triggered();

    void started_saving(QString abspath, QString filename);

    void saving_ended();

    void on_actionAbout_this_program_triggered();
};

#endif // MAINWINDOW_H
