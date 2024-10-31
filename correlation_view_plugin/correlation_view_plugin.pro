QT += core gui
QT += widgets

TARGET = correlation_view_plugin
TEMPLATE = lib

QMAKE_CXXFLAGS += -Wno-unused-parameter

LIBS += -llibtimetag
LIBS += -L$$OUT_PWD/../phast_gui -lphast_gui

# Add qwt directory to search path
INCLUDEPATH += $$[QT_INSTALL_PREFIX]"/include/qwt-qt5"

# Also the qwt lib
LIBS += -lqwt-qt5

INCLUDEPATH += $$PWD/..

DEFINES += CORRELATION_VIEW_PLUGIN_LIBRARY

# The following define makes your compiler emit warnings if you use
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        correlation_view_plugin.cpp \
    widget_correlation_bin_settings.cpp \
    plot_correlation_ui.cpp \
    correlation_calc.cpp

HEADERS += \
        correlation_view_plugin.h \
        correlation_view_plugin_global.h \
    plot_correlation_ui.h \
    correlation_calc.h \
    widget_correlation_bin_settings.h

FORMS += \
        widget_correlation_bin_settings.ui

unix {
    target.path = /usr/lib
    INSTALLS += target
}
