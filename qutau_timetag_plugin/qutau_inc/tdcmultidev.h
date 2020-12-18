/******************************************************************************
 *
 *  Project:        TDC Control Library
 *
 *  Filename:       tdcmultidev.h
 *
 *  Purpose:        Multi Device Handling for TDC devices
 *
 *  Author:         NHands GmbH & Co KG
 */
/*****************************************************************************/
/*****************************************************************************/
/* $Id: tdcmultidev.h,v 1.2 2016/04/05 15:06:09 trurl Exp $ */

#ifndef __TDCMULTIDEV_H
#define __TDCMULTIDEV_H

#include "tdcdecl.h"

TDC_API int TDC_CC TDC_discover( unsigned int * devCount );


TDC_API int TDC_CC TDC_getDeviceInfo( unsigned int  devNo,
                                      TDC_DevType * type,
                                      int         * id,
                                      char        * serialNo,
                                      Bln32       * connected );


TDC_API int TDC_CC TDC_connect( unsigned int devNo );


TDC_API int TDC_CC TDC_disconnect( unsigned int devNo );


TDC_API int TDC_CC TDC_addressDevice( unsigned int devNo );


TDC_API int TDC_CC TDC_getCurrentAddress( unsigned int * devNo );

#endif