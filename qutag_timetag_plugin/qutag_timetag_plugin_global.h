#ifndef QUTAG_TIMETAG_PLUGIN_GLOBAL_H
#define QUTAG_TIMETAG_PLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QUTAG_TIMETAG_PLUGIN_LIBRARY)
#  define QUTAG_TIMETAG_PLUGINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QUTAG_TIMETAG_PLUGINSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QUTAU_TIMETAG_PLUGIN_GLOBAL_H
