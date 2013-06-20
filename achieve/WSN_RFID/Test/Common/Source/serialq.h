/****************************************************************************
 *
 * MODULE:             Wireless UART
 *
 * COMPONENT:          serialq.h,v
 *
 * VERSION:
 *
 * REVISION:           1.3
 *
 * DATED:              2006/11/09 12:57:10
 *
 * STATUS:             Exp
 *
 * AUTHOR:
 *
 * DESCRIPTION
 *
 * CHANGE HISTORY:
 *
 * serialq.h,v
 * Revision 1.3  2006/11/09 12:57:10
 * Updated copyright information in file header
 *
 * Revision 1.2  2006/11/06 15:42:10
 * Updated to use interrupt driven comms
 *
 * Revision 1.1  2006/08/24 14:58:59
 * Initial version
 *
 *
 *
 * LAST MODIFIED BY:
 *                     $Modtime: $
 *
 *
 ****************************************************************************
 *
 * This software is owned by Jennic and/or its supplier and is protected
 * under applicable copyright laws. All rights are reserved. We grant You,
 * and any third parties, a license to use this software solely and
 * exclusively on Jennic products. You, and any third parties must reproduce
 * the copyright and warranty notice and any other legend of ownership on each
 * copy or partial copy of the software.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". JENNIC MAKES NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
 * ACCURACY OR LACK OF NEGLIGENCE. JENNIC SHALL NOT, IN ANY CIRCUMSTANCES,
 * BE LIABLE FOR ANY DAMAGES, INCLUDING, BUT NOT LIMITED TO, SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER.
 *
 * Copyright Jennic Ltd 2005, 2006, 2007. All rights reserved
 *
 ****************************************************************************/


#ifndef  SERIAL_Q_H_INCLUDED
#define  SERIAL_Q_H_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <jendefs.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {RX_QUEUE = 0, TX_QUEUE } eQueueRef;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void   vSerialQ_Init(void);
PUBLIC bool_t bSerialQ_Full(eQueueRef eQueue);
PUBLIC bool_t bSerialQ_Empty(eQueueRef eQueue);
PUBLIC uint8  u8SerialQ_RemoveItem(eQueueRef eQueue);
PUBLIC void   vSerialQ_AddItem(eQueueRef eQueue, uint8 u8Item);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* SERIAL_Q_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/


