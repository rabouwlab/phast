#-------------------------------------------------
#
# Project created by QtCreator 2018-11-04T20:06:59
#
#-------------------------------------------------

TARGET = fake_timetag_device_plugin
TEMPLATE = lib

QT += core
QT += gui
QT += widgets

DEFINES += FAKE_TIMETAG_DEVICE_PLUGIN_LIBRARY
CONFIG += plugin

QMAKE_CXXFLAGS += -Wno-unused-parameter

# Add qwt directory to search path
INCLUDEPATH += $$[QT_INSTALL_PREFIX]"/include/qwt"

# Also the qwt lib
LIBS += -lqwt

INCLUDEPATH += $$PWD/..

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

LIBS += -lgsl

SOURCES += \
        fake_timetag_device_plugin.cpp \
        fakecommunicator.cpp \
        ui_fake_device_settings.cpp

HEADERS += \
        fake_timetag_device_plugin.h \
        fake_timetag_device_plugin_global.h \
        ui_fake_device_settings.h \
        fakecommunicator.h

FORMS += \
        ui_fake_device_settings.ui

unix {
    target.path = /usr/lib
    INSTALLS += target
}
