QT += core gui
QT += widgets

TARGET = qutau_timetag_plugin
TEMPLATE = lib
CONFIG += plugin

DEFINES += QUTAU_TIMETAG_PLUGIN_LIBRARY

DEFINES += QT_DEPRECATED_WARNINGS

LIBS += -L$$OUT_PWD/../phast_gui -lphast_gui

# Link with QuTool's library
LIBS += -L$$PWD/qutau_lib -ltdcbase

# Add qwt directory to search path
INCLUDEPATH += $$[QT_INSTALL_PREFIX]"/include/qwt-qt5"

# Also the qwt lib
LIBS += -lqwt-qt5
INCLUDEPATH += $$PWD/..


INCLUDEPATH += ./../

SOURCES += \
        qutau_timetag_plugin.cpp \
    qutau_communicator.cpp \
    triggersettings_ui.cpp

HEADERS += \
        qutau_timetag_plugin.h \
        qutau_timetag_plugin_global.h \ 
    qutau_communicator.h \
    triggersettings_ui.h \
    chan_trigger_settings.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    triggersettings_ui.ui


