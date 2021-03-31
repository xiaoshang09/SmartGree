/*****************************************************************************
 * CIOIIDevice.h
 *****************************************************************************
 *
 * Project:     HALCON/libhalcon
 * Description: Integration interface for IO Devices
 *
 * (c) 2013-2017 by MVTec Software GmbH
 *                  www.mvtec.com
 *
 *****************************************************************************
 *
 *
 *****************************************************************************/

#ifndef CIOIIDEVICE_H
#define CIOIIDEVICE_H

#ifndef HC_NO_IOIINTERFACE

#include "hlib/CIOIIDeviceDefines.h"

#if defined(__cplusplus)
extern "C" {
#endif


extern HLibExport  Herror IOPrintErrorMessage(char *err);

extern Herror HStartupCIOIIDevice( void);
extern Herror HShutdownCIOIIDevice( void);


#if defined(__cplusplus)
}
#endif


#endif /* ifndef HC_NO_IOIINTERFACE */

#endif /* ifndef CIODEVICE_H */
