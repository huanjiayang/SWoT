/*****************************************************************************
 *
 * MODULE:             Home Sensor Demo
 *
 * COMPONENT:          app_led.c
 *
 * AUTHOR:             GPfef
 *
 * DESCRIPTION:        Controls LEDs
 *
 * $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Application_Notes/JN-AN-1122-ZBPro-Home-Sensor-Demo/Tags/ZBPRO-HOME-SENSOR-DEMO_1v7_RC4/AN1122_ZBP_ControllerNode/Source/app_led.c $
 *
 * $Revision: 27476 $
 *
 * $LastChangedBy: fbhai $
 *
 * $LastChangedDate: 2010-11-08 11:43:26 +0000 (Mon, 08 Nov 2010) $
 *
 * $Id: app_led.c 27476 2010-11-08 11:43:26Z fbhai $
 *
 *****************************************************************************
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
 * Copyright Jennic Ltd. 2009 All rights reserved
 *
 ****************************************************************************/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "os.h"
#include "os_gen.h"

#include "DBG.h"
#include "AppHardwareApi.h"
#include "app_home_sensor_demo.h"
#include "zps_apl_af.h"
#include "ledcontrol.h"
#include "app_led.h"
#include "app_log.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifndef TRACE_APP_LED
#define TRACE_APP_LED               FALSE
#endif

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME: APP_vLedsInitialise
 *
 * DESCRIPTION:
 * Initialises LEDs and sets them off
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vLedsInitialise(void)
{
    /* UARTs share DIO with LEDs - must disable RTS and CTS lines to prevent interference  */
    vAHI_UartSetRTSCTS(0, 0);
    vAHI_UartSetRTSCTS(1, 0);

    vLedInitFfd();
    vLedControl(0, 0);
    vLedControl(1, 0);
    vLedControl(2, 0);
    vLedControl(3, 0);
}

/****************************************************************************
 *
 * NAME: APP_vLedSet
 *
 * DESCRIPTION:
 * Turns specified LED on or off
 *
 * PARAMETERS:      Name        RW  Usage
 *                  u8Led       R   Identifies LED
 *                  bOn         R   TRUE = ON
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vLedSet(uint8 u8Led, bool_t bOn)
{
    vLedControl(u8Led, bOn);
}

/****************************************************************************
 *
 * NAME: APP_taskLedControl
 *
 * DESCRIPTION:
 * Receives LED control messages and turns LEDs on/off as necessary
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
OS_TASK(APP_taskLedControl)
{
    ZPS_tsAfEvent sLedEvent;

    /* collect the message for the endpoint associated with APP_msgLedEvent */
    if (OS_E_OK == OS_eCollectMessage(APP_msgLedEvent, &sLedEvent))
    {
        /* Ensure that we are dealing with a data indication - note that
         * data confirmations for this endpoint will also be received in this task
         */
        if (ZPS_EVENT_APS_DATA_INDICATION == sLedEvent.eType)
        {
            uint8 u8LedState;
            if (0 != PDUM_u16APduInstanceReadNBO(sLedEvent.uEvent.sApsDataIndEvent.hAPduInst,
                                                 APDU_LEDCTRL_STATE,
                                                 "b",
                                                 &u8LedState))
            {
                uint8 u8Node = APP_u8GetSensorNodeId(sLedEvent.uEvent.sApsDataIndEvent.uSrcAddress.u16Addr,0);
                if (NOT_FOUND != u8Node)
                {
                    vLedControl(u8Node, u8LedState);
                }
            }
            PDUM_eAPduFreeAPduInstance(sLedEvent.uEvent.sApsDataIndEvent.hAPduInst);
        }
    }
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
