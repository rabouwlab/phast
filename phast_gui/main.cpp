#include "mainwindow.h"
#include <QApplication>
#include <qmetatype.h>
#include <map>

#include "offline_online_dialog.h"
#include "support/dataview.h"
#include "support/timetag_device.h"
#include "ui/ui_select_timetag_device.h"
#include "support/global.h"

#include "interfaces/itimetagplugin.h"
#include "interfaces/itimetaggercommunicator.h"
#include "interfaces/idataviewplugin.h"

#include <qdir.h>
#include <qpluginloader.h>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#include "tclap/CmdLine.h"

struct cmdline_input
{
public:
    std::vector<std::string> additional_plugin_dirs;
};

cmdline_input handle_input(int argc, char** argv)
{
    cmdline_input input;

    try {
        TCLAP::CmdLine cmd("Photon Arrival-time STudio", ' ', "0.8");

        TCLAP::MultiArg<std::string> additional_plugin_dirs("P", "plugindir", "Additional directory to look for plugins", false, "string");
        cmd.add(additional_plugin_dirs);

        cmd.parse( argc, argv );

        input.additional_plugin_dirs = additional_plugin_dirs.getValue();
    }
    catch (TCLAP::ArgException &e)  // catch any exceptions
    {
        // TODO: handle this
    }

    return input;
}

typedef std::map<int64_t,DataView::data_update> data_map;

Q_DECLARE_METATYPE(DataView::data_update);
//Q_DECLARE_METATYPE(IDataView::data_update);
Q_DECLARE_METATYPE(data_map);

template<typename T>
std::vector<T> load_plugins(QString directory)
{
    std::vector<T> ret;

    // First open the plugin directory
    QDir pluginsDir;
    pluginsDir = QDir(qApp->applicationDirPath());

    pluginsDir.cd(directory);

    const auto entryList = pluginsDir.entryList(QDir::Files);
    for (const QString &fileName : entryList) {
        QLibrary temp(pluginsDir.absoluteFilePath(fileName));
        temp.load();
    }

    for (const QString &fileName : entryList) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));

        QObject *plugin = loader.instance();
        if (plugin) {
            T p = qobject_cast<T>(plugin);

            if (p) {
                ret.push_back(p);
            }
        } else {
#ifdef NDEBUG
            std::cout << "failed to load plugin from " << fileName.toStdString() << " because: " << loader.errorString().toStdString() << std::endl;
#endif
        }
    }

    return ret;
}

ITimetagPlugin* select_timetag_plugin(const std::vector<ITimetagPlugin*>& plugins)
{


    // Now have the plugins make a list of available devices...
    std::vector<timetag_device> devices;

    for (ITimetagPlugin* plugin : plugins) {
        ITimeTaggerCommunicator* comm = plugin->GetComm();

        uint64_t n_devices_connected = comm->GetNumDevicesConnected();

        for (int64_t i = 0; i < n_devices_connected; i++) {
            bool success = comm->TryToConnect(i);

            if (success) {
                timetag_device d;
                d.communicator = plugin;
                d.device_ID = i;
                d.device_name = comm->DeviceDescriptor();
                d.is_real_device = comm->IsRealDevice();

                devices.push_back(d);
            }
        }
    }

    // Pop up a window, allowing the user to select which device to connect to
    ui_select_timetag_device sel_ui(nullptr, devices);

    int ret = sel_ui.exec();

    if (ret == QDialog::Rejected) {
        return nullptr;
    }

    timetag_device selected_device;
    selected_device.device_ID = 0;
    selected_device.device_name = "none";

    if (ret == QDialog::Accepted) {
        if (!sel_ui.DeviceSelected().any_item_selected)
            return nullptr;

        selected_device = sel_ui.DeviceSelected().device;

        bool success = selected_device.communicator->GetComm()->TryToConnect(selected_device.device_ID);


        if (success) {
            return selected_device.communicator;
        } else {
            std::cerr << "Error when trying to connect to device " << selected_device.device_name <<
                         " (ID " << selected_device.device_ID << ")" << std::endl;
            std::cerr << "You may try to turn the device off and on again." << std::endl;
            std::cerr << "If that does not work, check the right driver version is installed " << std::endl
                      << " (Windows sometimes automatically updates drivers to a newer, incompatible, version" << std::endl;
            return nullptr;
        }
    }

    return nullptr;;
}

int main(int argc, char *argv[])
{
    std::cout << PHAST_APP_NAME << std::endl;
    std::cout << "version " << PHAST_GUI_VERSION_STR << std::endl;
    std::cout << "By " << PHAST_AUTHOR << std::endl;
    std::cout << PHAST_AUTHOR_EMAIL << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    qRegisterMetaType<DataView::data_update>("IDataView::data_update");
    qRegisterMetaType<data_map>("std::map<int64_t,IDataView::data_update>");
    QApplication a(argc, argv);

#ifdef _WIN32
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }
#endif

    cmdline_input input = handle_input(argc, argv);

    // See if we have any plugins
	std::cout << " Loading plugins from 'plugins' dir" << std::endl;
    std::vector<ITimetagPlugin*> tt_plugins = load_plugins<ITimetagPlugin*>("plugins");

    for (std::string dir : input.additional_plugin_dirs) { // Also load from additional plugin directories
		std::cout << " Loading plugins from '"<<dir<<"' dir" << std::endl;
        std::vector<ITimetagPlugin*> temp = load_plugins<ITimetagPlugin*>(QString::fromStdString(dir));
        tt_plugins.insert(tt_plugins.end(), temp.begin(), temp.end());
    }

    std::cout << "Found " << tt_plugins.size() << " time tag plugins" << std::endl;
    for (auto plugin : tt_plugins) {
       std::cout << "\t" << plugin->TimeTagPluginName().toStdString() << std::endl;
    }

    std::vector<IDataviewPlugin*> dataviewplugins = load_plugins<IDataviewPlugin*>("plugins");

    for (std::string dir : input.additional_plugin_dirs) { // Also load from additional plugin directories
        std::vector<IDataviewPlugin*> temp = load_plugins<IDataviewPlugin*>(QString::fromStdString(dir));
        dataviewplugins.insert(dataviewplugins.end(), temp.begin(), temp.end());
    }

    std::cout << "found " << dataviewplugins.size() << " data view plugins" << std::endl;

    for (auto plugin : dataviewplugins) {
       std::cout << "\t" << plugin->DataviewPluginName().toStdString() << std::endl;
    }

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    offline_online_dialog dialog;
    dialog.exec();

    if (dialog.state == NONE) {
        return 0;
    }

    ITimetagPlugin* plugin = nullptr;

    if (dialog.state == ONLINE) {
        plugin = select_timetag_plugin(tt_plugins);

        if (plugin == nullptr) {
            return 1;
        }

        plugin->ShowInitDialog(nullptr);
    } else if (dialog.state == OFFLINE) {
        plugin = nullptr;
    }

    MainWindow w(nullptr, plugin, dataviewplugins);
    w.move(0,0);
    w.show();

    return a.exec();
}
