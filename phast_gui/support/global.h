/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TT_BUILDING_APPLICATION)
#  define TT_EXPORT Q_DECL_EXPORT
#else
#  define TT_EXPORT Q_DECL_IMPORT
#endif

#define PHAST_GUI_VERSION_STR "0.8"
#define PHAST_GUI_VERSION 0.8
#define PHAST_APP_NAME "Photon Arrival-time STudio"
#define PHAST_AUTHOR "Stijn Hinterding"
#define PHAST_AUTHOR_EMAIL "git@stijnhinterding.nl / s.o.m.hinterding@uu.nl"

#endif // GLOBAL_H
