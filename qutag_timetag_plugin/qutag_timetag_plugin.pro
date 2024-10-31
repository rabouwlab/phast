QT += core gui
QT += widgets

TARGET = qutag_timetag_plugin
TEMPLATE = lib
CONFIG += plugin

DEFINES += QUTAG_TIMETAG_PLUGIN_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

LIBS += -L$$OUT_PWD/../phast_gui -lphast_gui

# Link with QuTool's library
LIBS += -L$$PWD/qutag_lib -ltdcbase

# Add qwt directory to search path
INCLUDEPATH += $$[QT_INSTALL_PREFIX]"/include/qwt-qt5"

# Also the qwt lib
LIBS += -lqwt-qt5
INCLUDEPATH += $$PWD/..

SOURCES += \
        qutag_timetag_plugin.cpp \
    qutag_communicator.cpp \
    triggersettings_ui.cpp

HEADERS += \
        qutag_timetag_plugin.h \
        qutag_timetag_plugin_global.h \ 
    qutag_communicator.h \
    triggersettings_ui.h \
    chan_trigger_settings.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    triggersettings_ui.ui


