/****************************************************************************
 *
 * MODULE:             Wireless UART
 *
 * COMPONENT:          config.h,v
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
 * config.h,v
 * Revision 1.3  2006/11/09 12:57:10
 * Updated copyright information in file header
 *
 * Revision 1.2  2006/11/06 15:42:10
 * Updated to use interrupt driven comms
 *
 * Revision 1.1  2006/08/24 14:59:31
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

#ifndef  CONFIG_H_INCLUDED
#define  CONFIG_H_INCLUDED

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
/* Network parameters */
#define PAN_ID                  0xDEA1U
#define COORD_ADDR              0x0000U

/* Wireless UART device data */
#define MAX_UART_NODES          1
#define UART_NODE_ADDR_BASE     0x0001U
#define MAX_DATA_PER_FRAME      64

#define TICK_PERIOD_ms          10UL
#define TICK_PERIOD_COUNT       (16000UL * TICK_PERIOD_ms)

/* Defines the channels to scan. Each bit represents one channel. All channels
   in the channels (11-26) in the 2.4GHz band are scanned. */
#define SCAN_CHANNELS           0x07FFF800UL
#define CHANNEL_MIN             11
#define ACTIVE_SCAN_DURATION    3
#define ENERGY_SCAN_DURATION    3 /* Duration (ms) = ((960 * (2^ENERGY_SCAN_DURATION + 1)) / 62.5) */

/* Define which of the two available hardware UARTs and what baud rate to use */
#define UART                    E_AHI_UART_0
//#define UART_BAUD_RATE          19200

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* CONFIG_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/


