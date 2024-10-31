#-------------------------------------------------
#
# Project created by QtCreator 2017-12-22T19:38:53
#
#-------------------------------------------------

QT       += core gui
QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += TT_BUILDING_APPLICATION

QMAKE_CXXFLAGS += -Weffc++ -Wno-unused-parameter

# Add qwt directory to search path
INCLUDEPATH += $$[QT_INSTALL_PREFIX]"/include/qwt-qt5"

# Also the qwt lib
LIBS += -lqwt-qt5

INCLUDEPATH += $$PWD/..

LIBS += -llibtimetag -Wl,--out-implib=libphast_gui.dll.a

TARGET = phast
TEMPLATE = app

CONFIG += console

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        mainwindow.cpp \
    base/base_chandata.cpp \
    base/base_datarouter.cpp \
    base/base_settingscontainer.cpp \
    base/base_tempdatastorage.cpp \
    base/base_ui_calc.cpp \
    base/base_view_plot.cpp \
    impl/nonpulsechandata.cpp \
    impl/pulsechandata.cpp \
    impl/writer_listeners.cpp \
    support/customscaledraw.cpp \
    support/mainplot.cpp \
    support/qutau_filereader.cpp \
    support/small_tcspc_io.cpp \
    support/small_tcspc_io_reader.cpp \
    ui/about_dialog.cpp \
    ui/appchannelinfo_form.cpp \
    ui/channeloverviewui.cpp \
    ui/qutau_fileimport_dialog.cpp \
    ui/timetagger_infoscreen.cpp \
    ui/widgetplotsettings.cpp \
    main.cpp \
    offline_online_dialog.cpp \
    online_datasaving_ui.cpp \
    reader_coord.cpp \
    writer_coord.cpp \
    ui/ui_select_timetag_device.cpp \
    support/dataview.cpp \
    support/small_tcspc_io2.cpp \
    support/chaninfo.cpp

HEADERS += \
        mainwindow.h \
    base/base_chandata.h \
    base/base_datarouter.h \
    base/base_settingscontainer.h \
    base/base_tempdatastorage.h \
    base/base_ui_calc.h \
    base/base_view_plot.h \
    support/bitpacker.h \
    impl/nonpulsechandata.h \
    impl/pulsechandata.h \
    impl/writer_listeners.h \
    interfaces/ibinningclass.h \
    interfaces/ichandata.h \
    interfaces/ichandataprovider.h \
    interfaces/idatarouter.h \
    interfaces/ilistener.h \
    interfaces/ipermanentdatastorage.h \
    interfaces/isettingscontainer.h \
    interfaces/itempdatastorage.h \
    interfaces/itimetaggercommunicator.h \
    support/customscaledraw.h \
    support/detectionupdate.h \
    support/lockfreequeue.h \
    support/logspace.h \
    support/mainplot.h \
    support/photon_event.h \
    support/plotparams.h \
    support/qutau_filereader.h \
    support/raw_photon_data.h \
    support/small_tcspc_io.h \
    support/small_tcspc_io_reader.h \
    support/typedefs.h \
    support/zoomer.h \
    ui/about_dialog.h \
    ui/appchannelinfo_form.h \
    ui/channeloverviewui.h \
    ui/qutau_fileimport_dialog.h \
    ui/timetagger_infoscreen.h \
    ui/widgetplotsettings.h \
    support/chaninfo.h \
    offline_online.h \
    offline_online_dialog.h \
    online_datasaving_ui.h \
    reader_coord.h \
    writer_coord.h \
    support/timetag_device.h \
    ui/ui_select_timetag_device.h \
    interfaces/itimetagplugin.h \
    interfaces/idataviewplugin.h \
    interfaces/idataplot.h \
    interfaces/idatawindow.h \
    interfaces/idatawindowstub.h \
    support/dataview.h \
    support/global.h \
    interfaces/ifilewriter.h \
    support/small_tcspc_io2.h

FORMS += \
        mainwindow.ui \
    channeloverviewui.ui \
    timetagger_infoscreen.ui \
    appchannelinfo_form.ui \
    qutau_fileimport_dialog.ui \
    appchannelinfo_form.ui \
    offline_online_dialog.ui \
    widgetplotsettings.ui \
    online_datasaving_ui.ui \
    base_view_plot.ui \
    about_dialog.ui \
    ui/ui_select_timetag_device.ui


