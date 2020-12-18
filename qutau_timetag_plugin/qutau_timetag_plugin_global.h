/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef QUTAU_TIMETAG_PLUGIN_GLOBAL_H
#define QUTAU_TIMETAG_PLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QUTAU_TIMETAG_PLUGIN_LIBRARY)
#  define QUTAU_TIMETAG_PLUGINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QUTAU_TIMETAG_PLUGINSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QUTAU_TIMETAG_PLUGIN_GLOBAL_H
