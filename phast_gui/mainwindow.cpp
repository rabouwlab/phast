#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "support/mainplot.h"

#include <qwt/qwt_plot_curve.h>

#include <vector>
#include <math.h>

#include <QFileDialog>
#include <QFile>
#include <iostream>

#include "support/qutau_filereader.h"

#include "ui/channeloverviewui.h"

#include "interfaces/itimetaggercommunicator.h"
#include "ui/timetagger_infoscreen.h"
#include "base/base_datarouter.h"
#include "base/base_tempdatastorage.h"
#include "ui/qutau_fileimport_dialog.h"
#include "base/base_view_plot.h"
#include "support/dataview.h"
#include "ui/about_dialog.h"

#include "base/base_settingscontainer.h"
#include "ui/widgetplotsettings.h"
#include "writer_coord.h"
#include "support/small_tcspc_io_reader.h"
#include "reader_coord.h"
#include "interfaces/itimetagplugin.h"

#include "base/base_view_plot.h"
#include "interfaces/idatawindowstub.h"
#include "interfaces/idataviewplugin.h"

#include <qmessagebox.h>
#include <QProgressDialog>
#include <QThread>
#include <QTimer>
#include <cassert>

MainWindow::MainWindow(QWidget *parent,
                       ITimetagPlugin* tt_plugin,
                       const std::vector<IDataviewPlugin*>& dataviewplugins) :
    QMainWindow(parent),
    time_unit((tt_plugin == nullptr) ? 1 : tt_plugin->GetComm()->TimeUnit()),
    state((tt_plugin == nullptr) ? OFFLINE : ONLINE),
    settings(),
    ui(new Ui::MainWindow),
    tt_comm((tt_plugin == nullptr) ? nullptr : tt_plugin->GetComm()),
    data_storage(new Base_TempDataStorage(&this->queue)),
    data_router((tt_plugin == nullptr) ? nullptr : new Base_DataRouter(this->tt_comm, &this->queue, 5)), // poll interval = 20 ms
    plot_settings_ui(new WidgetPlotSettings(this)),
    plot_timer(new QTimer()),
    queue(),
    clean_queue(),
    saving_ui(nullptr),
    tt_plugin(tt_plugin),
    dataview_plugins(dataviewplugins)
{
    this->plot_timer->setInterval(30);
    this->plot_timer->start();

    ui->setupUi(this);

    for (uint64_t i = 0; i < this->dataview_plugins.size(); i++) {
        IDataviewPlugin* p = this->dataview_plugins.at(i);
        QAction* qa = this->ui->menuViews->addAction(p->DataviewMenuDescriptor());
        bool connection = this->connect(qa, &QAction::triggered, [=]() {this->dataviewplugin_activated(p);});
        assert(connection);
    }

    this->setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    if (this->state == OFFLINE) {
        this->prep_for_offline();
    } else if (this->state == ONLINE) {
        this->prep_for_online();
    }

    this->plot_settings_ui->move(0, 70);
    this->plot_settings_ui->show();
}

MainWindow::~MainWindow()
{
    emit NewTempDataStorage(nullptr);

    delete ui;
    delete data_router;

    if (this->data_storage != nullptr) {
        delete this->data_storage;
    }
}

void MainWindow::tt_plugin_activated()
{
    std::vector<chan_id> active_chans;

    for (auto pair : this->settings.GetChannelInfos()) {
        chan_id id = pair.first;
        active_chans.push_back(id);
    }

    this->tt_plugin->ShowTriggerSettingsUI(this, active_chans);

    // Now get the sync dividers
    bool significant_change = false;

    for (auto pair : this->settings.GetChannelInfos()) {
        chan_id id = pair.first;

        uint64_t initial_div = this->settings.chan_infos.at(id).sync_divider;
        this->settings.chan_infos.at(id).sync_divider = this->tt_comm->GetSyncDivider(id);

        if (this->settings.chan_infos.at(id).sync_divider != initial_div) {
            significant_change = true;
        }
    }

    if (significant_change) {
        if (this->data_storage != nullptr)
            delete this->data_storage;

        this->data_storage = new Base_TempDataStorage(this->settings.GetChannelInfos(),
                                                      false,
                                                      (this->state == OFFLINE),
                                                      20,
                                                      &this->queue);

        this->data_storage->StartThread();
        emit NewTempDataStorage(this->data_storage);
    }
}

void MainWindow::dataviewplugin_activated(IDataviewPlugin *plugin)
{
    // Create a new view! :-)
    std::vector<chan_id> channels;

    for (auto pair : this->settings.GetChannelInfos()) {
        chaninfo ci(pair.second);

        if (plugin->HandlesOnlyMicrotimeChans() && !ci.channel_has_microtime)
            continue;

        if (plugin->HandlesOnlyNonMicrotimeChans() && ci.channel_has_microtime)
            continue;

        if (plugin->HandlesOnlyPulseChans() && !ci.is_pulses_channel)
            continue;

        channels.push_back(ci.ID);
    }

    if (channels.size() == 0) {
        return;
    }

    bool online = (this->tt_plugin == nullptr) ? false : true;


    IDataWindowStub* t_ui = plugin->ConstructView(this->time_unit,
                                                  this,
                                                  this->data_storage,
                                                  channels,
                                                  online);

    if (t_ui == nullptr) {
        return; // No need to construct a view, we abort.
    }

    bool connection = connect(this, &MainWindow::NewTempDataStorage, t_ui->GetDataWindow(), &IDataWindow::on_new_tempdatastorage);
    assert( connection );

    connection = connect(t_ui->GetDataWindow(),&IDataWindow::got_focus, this->plot_settings_ui, &WidgetPlotSettings::new_view);
    assert( connection );
    connection = connect(t_ui->GetDataWindow(),&IDataWindow::got_binner_focus, this->plot_settings_ui, &WidgetPlotSettings::new_plot);
    assert( connection );
    connection = connect(t_ui->GetDataWindow(), &IDataWindow::plot_deleted, this->plot_settings_ui, &WidgetPlotSettings::view_deleted);
    assert( connection );
    connection = connect(t_ui->GetDataWindow(), &IDataWindow::plot_deleted, this->plot_settings_ui, &WidgetPlotSettings::view_deleted);
    assert( connection );
    connection = connect(t_ui->GetDataWindow(), &IDataWindow::plot_binner_deleted, this->plot_settings_ui, &WidgetPlotSettings::plot_deleted);
    assert( connection );
    connection = connect(this->plot_timer, &QTimer::timeout, t_ui->GetDataPlot(), &IDataPlot::do_plot);
    assert( connection );
    connection = connect(this, &MainWindow::clear_data, t_ui->GetDataWindow(), &IDataWindow::clear_data);
    assert( connection );

    t_ui->GetDataWindow()->show_window();
}

void MainWindow::started_saving(QString abspath, QString filename)
{
    this->ui->menuSettings->setEnabled(false);
    this->ui->actionChannels->setEnabled(false);
}

void MainWindow::saving_ended()
{
    this->ui->menuSettings->setEnabled(true);
    this->ui->actionChannels->setEnabled(true);
}

void MainWindow::prep_for_offline()
{
    // TODO: set the time unit somehow
    this->ui->actionAbout_this_program->setEnabled(true);
    this->ui->actionChannels->setEnabled(true);
    this->ui->actionConnection_info->setEnabled(false);
    this->ui->actionCorrelation_curve->setEnabled(true);
    this->ui->actionDecay_Curve->setEnabled(true);
    this->ui->actiondT_TimeTagged_file->setEnabled(true);
    this->ui->actionExport_as_ttdt->setEnabled(true);
    this->ui->actionIntensity_trace->setEnabled(true);
    this->ui->actionQuTau_file->setEnabled(true);
    this->ui->actionData_writer->setEnabled(false);

    this->data_storage->StartThread();

    bool connection = connect(this->plot_settings_ui, &WidgetPlotSettings::clear_all_views, this, &MainWindow::clear_data);
    assert( connection );
}

bool MainWindow::prep_for_online()
{
    if (this->tt_plugin->TimeTagHasSettingsMenu()) {
        QAction* qa = this->ui->menuSettings->addAction(this->tt_plugin->TimeTagMenuDescriptor());
        bool connection = this->connect(qa, &QAction::triggered, this, &MainWindow::tt_plugin_activated);
        assert( connection );
    }

    // We are connected!
    this->ui->actionAbout_this_program->setEnabled(true);
    this->ui->actionChannels->setEnabled(true);
    this->ui->actionConnection_info->setEnabled(true);
    this->ui->actionCorrelation_curve->setEnabled(true);
    this->ui->actionDecay_Curve->setEnabled(true);
    this->ui->actiondT_TimeTagged_file->setEnabled(false);
    this->ui->actionExport_as_ttdt->setEnabled(false);
    this->ui->actionIntensity_trace->setEnabled(true);
    this->ui->actionQuTau_file->setEnabled(false);
    this->ui->menuImport->setEnabled(false);

    this->saving_ui = new online_datasaving_ui(this->time_unit, QString::fromStdString(this->tt_comm->DeviceDescriptor()), this, &this->settings, this->data_storage);

    bool connection = connect(this, &MainWindow::NewTempDataStorage, this->saving_ui, &online_datasaving_ui::new_temp_datastorage);
    assert( connection );
    connection = connect(this->saving_ui, &online_datasaving_ui::saving_started, this, &MainWindow::started_saving);
    assert( connection );
    connection = connect(this->saving_ui, &online_datasaving_ui::saving_ended, this, &MainWindow::saving_ended);
    assert( connection );
    connection = connect(this->plot_settings_ui, &WidgetPlotSettings::clear_all_views, this, &MainWindow::clear_data);
    assert( connection );

    this->data_router->StartThread();
    this->data_storage->StartThread();

    return true;
}

void MainWindow::clear_app_channel_infos()
{
    this->settings.chan_infos.clear();
}

void MainWindow::on_actionQuTau_file_triggered()
{
    qutau_fileimport_dialog d;
    int ret = d.exec();

    if (ret != QDialog::Accepted) {
        return;
    }

    qutau_fileimport_dialog::dialog_info di = d.info;

    QFile f(di.filename);

    if (!f.exists()) {
        std::cerr << "Desired file does not exist!" << std::endl;
        return;
    }

    uint64_t file_size = f.size();

    quTAU_filereader qfr;

    raw_photon_data data;

    this->settings.chan_infos.clear();

    QProgressDialog progress("Importing file.", "Cancel", 0, file_size);
    progress.setWindowModality(Qt::WindowModal);

    connect(&qfr, &quTAU_filereader::signalProgress, &progress, &QProgressDialog::setValue);
    connect(&qfr, &quTAU_filereader::finished, &progress, &QProgressDialog::close);

    progress.open();

    bool success = qfr.read_quTau(di.filename.toStdString().c_str(),
                                  di.trash_useless_pulses,
                                  &this->settings.chan_infos,
                                  &data,
                                  (int64_t)di.pulses_channel);

    if (success) {
        // Pop up window to set channel settings
        this->on_actionChannels_triggered();

        if (this->data_storage != nullptr)
            delete this->data_storage;

        this->data_storage = new Base_TempDataStorage(this->settings.GetChannelInfos(),
                                                      true,
                                                      true,
                                                      20,
                                                      &this->queue);

        this->data_storage->StartThread();
        emit NewTempDataStorage(this->data_storage);

        // Now feed in the data
        this->queue.Produce(data);
    }
}

bool MainWindow::handle_new_channel_info(chaninfo value)
{
    if (!value.has_pulses_channel) {
        value.channel_has_microtime = false;
    }

    value.sync_divider = this->tt_comm->GetSyncDivider(value.ID);

    bool change = false;

    if (this->settings.chan_infos.count(value.ID) != 0) {
        if (this->settings.chan_infos.at(value.ID) != value) {
            change = true;
        }        
    } else {
        change = true;
    }

    this->settings.chan_infos[value.ID] = value;

    return change;
}

void MainWindow::on_actionChannels_triggered()
{
    ChannelOverviewUI cui(this, nullptr, &this->settings);

    int ret = cui.exec();

    if (this->tt_comm == nullptr)
        return;

    if (ret == QDialog::Accepted) {
        std::map<chan_id, chaninfo> ui_chan_info = cui.ChannelInfo();

        std::vector<uint64_t> all_chans;

        bool significant_change = false;

        for (const auto &pair : ui_chan_info ) {
            bool change = this->handle_new_channel_info(pair.second);

            if (change) {
                significant_change = true;
            }

            all_chans.push_back(pair.second.ID);
        }

        if (significant_change) {
            if (this->data_storage != nullptr)
                delete this->data_storage;

            this->data_storage = new Base_TempDataStorage(this->settings.GetChannelInfos(),
                                                          false,
                                                          (this->state == OFFLINE),
                                                          20,
                                                          &this->queue);

            this->data_storage->StartThread();
            emit NewTempDataStorage(this->data_storage);
        }

        // Notify our control windows
        // this->decay_settings_ui->new_decay_chans_list(chans_with_microtimes);

        /*
        if (this->state == OFFLINE) {
            for (const auto &pair : ui_chan_info ) {
                if (!pair.second.has_pulses_channel)
                    continue;

                chaninfo ci = pair.second;

                timetagcorrelator tc;
                tc.generate_microtimes(this->perm_data.data[ci.ID].begin(), this->perm_data.data[ci.ID].end(),
                                       this->perm_data.data[ci.corresponding_pulses_channel].begin(), this->perm_data.data[ci.corresponding_pulses_channel].end());
            }
        }
        */
    }    
}

void MainWindow::on_actionConnection_info_triggered()
{
    timetagger_infoscreen info(this, this->tt_comm->ConnectedToDevice(), QString::fromStdString(this->tt_comm->DeviceDescriptor()), this->tt_comm->TimeUnit());

    info.exec();
}

void MainWindow::on_actionExport_as_stcspc_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Choose file save location", QString(), "Simple Small Time-Tagged (*.sstt)");

    QFile f(filename);

    if (f.exists()) {
        std::cerr << "Desired file already exist!" << std::endl;
        return;
    }

    QFileInfo qfi(filename);

    QString device_descriptor = (this->tt_comm == nullptr) ? this->device_descriptor : QString::fromStdString(this->tt_comm->DeviceDescriptor());

    writer_coord<small_tcspc_io2> wc(this->time_unit,
                   device_descriptor,
                   qfi.fileName(),
                   qfi.absolutePath(),
                   this->settings.GetChannelInfos(), true);

    wc.RegisterListeners(this->data_storage);

    while (!wc.IsDone()) {
        QCoreApplication::processEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "Finished!" << std::endl;
}

void MainWindow::on_action_sstt_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select file to import.", "", "Small Simple Time Tagged (*.sstt)");

    QFile f(filename);

    if (!f.exists()) {
        std::cerr << "Desired file does not exist!" << std::endl;
        return;
    }

    reader_coord reader(filename);

    this->settings.chan_infos.clear();

    bool success = reader.Read();

    if (!success) {
        std::cout << "Error while reading file :-(" << std::endl;
        return;
    }

    this->time_unit = reader.TimeUnit();
    this->device_descriptor = reader.DeviceDescriptor();
    this->settings.chan_infos = reader.ChannelInfos();

    if (success) {
        if (this->data_storage != nullptr)
            delete this->data_storage;

        emit clear_data();
        this->data_storage = new Base_TempDataStorage(this->settings.GetChannelInfos(),
                                                      true,
                                                      true,
                                                      20,
                                                      &this->clean_queue);

        this->data_storage->StartThread();
        emit NewTempDataStorage(this->data_storage);

        if (reader.all_raw_data->size() != 0) {
            this->clean_queue.Produce(reader.all_raw_data);
        }
        else {
            for (auto pair : reader.AllData()) {
                std::vector<photon_event>* e = pair.second;
                this->clean_queue.Produce(e);
            }
        }
    }
}

void MainWindow::on_actionData_writer_triggered()
{
    if (this->saving_ui == nullptr)
        return;

    this->saving_ui->show();
}

void MainWindow::on_actionAbout_this_program_triggered()
{
    about_dialog ad;

    ad.exec();
}
