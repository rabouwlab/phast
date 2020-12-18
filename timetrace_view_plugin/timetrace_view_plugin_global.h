/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef TIMETRACE_VIEW_PLUGIN_GLOBAL_H
#define TIMETRACE_VIEW_PLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TIMETRACE_VIEW_PLUGIN_LIBRARY)
#  define TIMETRACE_VIEW_PLUGINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TIMETRACE_VIEW_PLUGINSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TIMETRACE_VIEW_PLUGIN_GLOBAL_H
