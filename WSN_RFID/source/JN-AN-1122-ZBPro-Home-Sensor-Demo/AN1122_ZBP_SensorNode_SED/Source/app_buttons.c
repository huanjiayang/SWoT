/*****************************************************************************
 *
 * MODULE:             Home Sensor Demo
 *
 * COMPONENT:          app_buttons.c
 *
 * AUTHOR:             MRW
 *
 * DESCRIPTION:        Monitor the state of the buttons on the DK2 development
 *                     board, ensuring they are debounced.
 *
 * $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Application_Notes/JN-AN-1122-ZBPro-Home-Sensor-Demo/Tags/ZBPRO-HOME-SENSOR-DEMO_1v7_RC4/AN1122_ZBP_SensorNode_SED/Source/app_buttons.c $
 *
 * $Revision: 27498 $
 *
 * $LastChangedBy: fbhai $
 *
 * $LastChangedDate: 2010-11-08 14:35:22 +0000 (Mon, 08 Nov 2010) $
 *
 * $Id: app_buttons.c 27498 2010-11-08 14:35:22Z fbhai $
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
#include "pdum_gen.h"
#include "zps_gen.h"
#include "zps_apl_af.h"
#include "DBG.h"
#include "AppHardwareApi.h"
#include "app_home_sensor_demo.h"
#include "app_timer_driver.h"
#include "app_buttons.h"
#include "app_sensor_node_SED.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifndef TRACE_APP_BUTTON
#define TRACE_APP_BUTTON            FALSE
#endif

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void vSendLedData(PDUM_thAPduInstance hAPduInst);
PRIVATE void vHandleButtonEvent(APP_tsEvent sButtonEvent);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
#ifdef APP_CONTROLLER
PRIVATE uint8 s_u8ButtonDebounce[APP_BUTTONS_NUM] =
{   0xff, 0xff, 0xff, 0xff};
PRIVATE uint8 s_u8ButtonState[APP_BUTTONS_NUM] =
{   0, 0, 0, 0};
PRIVATE const uint8 s_u8ButtonDIOLine[APP_BUTTONS_NUM] =
{
    APP_BUTTONS_BUTTON_1,
    APP_BUTTONS_BUTTON_2,
    APP_BUTTONS_BUTTON_3,
    APP_BUTTONS_BUTTON_4
};
#else
PRIVATE uint8 s_u8ButtonDebounce[APP_BUTTONS_NUM] =
{ 0xff, 0xff };
PRIVATE uint8 s_u8ButtonState[APP_BUTTONS_NUM] =
{ 0, 0 };
PRIVATE const uint8 s_u8ButtonDIOLine[APP_BUTTONS_NUM] =
{ APP_BUTTONS_BUTTON_1, APP_BUTTONS_BUTTON_2 };
#endif

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: APP_vButtonsInitialise
 *
 * DESCRIPTION:
 * Initialises buttons
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC bool_t APP_bButtonInitialise(void)
{
    /* Set DIO lines to inputs with buttons connected */
    vAHI_DioSetDirection(APP_BUTTONS_DIO_MASK, 0);

    /* Turn on pull-ups for DIO lines with buttons connected */
    vAHI_DioSetPullup(APP_BUTTONS_DIO_MASK, 0);

    /* Set the edge detection for falling edges */
    vAHI_DioInterruptEdge(0, APP_BUTTONS_DIO_MASK);

    /* Enable interrupts to occur on selected edge */
    vAHI_DioInterruptEnable(APP_BUTTONS_DIO_MASK, 0);

    uint32 u32Buttons = u32AHI_DioReadInput() & APP_BUTTONS_DIO_MASK;
    if (u32Buttons != APP_BUTTONS_DIO_MASK)
    {
        return TRUE;
    }
    return FALSE;
}

/****************************************************************************
 *
 * NAME: APP_taskScanButtons
 *
 * DESCRIPTION:
 * De-bounces buttons
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
OS_TASK(APP_taskScanButtons)
{
    /*
     * The DIO changed status register is reset here before the scan is performed.
     * This avoids a race condition between finishing a scan and re-enabling the
     * DIO to interrupt on a falling edge.
     */
    (void) u32AHI_DioInterruptStatus();

    uint8 u8AllReleased = 0xff;
    unsigned int i;
    uint32 u32DIOState = u32AHI_DioReadInput() & APP_BUTTONS_DIO_MASK;
    DBG_vPrintf(TRACE_APP_BUTTON, "DIO=%x\n", u32DIOState);

    for (i = 0; i < APP_BUTTONS_NUM; i++)
    {
        uint8 u8Button = (uint8) ((u32DIOState >> s_u8ButtonDIOLine[i]) & 1);

        s_u8ButtonDebounce[i] <<= 1;
        s_u8ButtonDebounce[i] |= u8Button;
        u8AllReleased &= s_u8ButtonDebounce[i];

        if (0 == s_u8ButtonDebounce[i] && !s_u8ButtonState[i])
        {
            APP_tsEvent sButtonEvent;
            DBG_vPrintf(TRACE_APP_BUTTON, "Button DN=%d\n", i);
            sButtonEvent.eType = APP_E_EVENT_BUTTON_DOWN;
            sButtonEvent.sButton.u8Button = i;
            s_u8ButtonState[i] = TRUE;
            vHandleButtonEvent(sButtonEvent);
        }
        else if (0xff == s_u8ButtonDebounce[i] && s_u8ButtonState[i] != FALSE)
        {
            DBG_vPrintf(TRACE_APP_BUTTON, "Button UP=%d\n", i);
            s_u8ButtonState[i] = FALSE;
        }
    }

    if (0xff == u8AllReleased)
    {
        /*
         * all buttons high so set dio to interrupt on change
         */
        DBG_vPrintf(TRACE_APP_BUTTON, "ALL UP\n", i);
        vAHI_DioInterruptEnable(APP_BUTTONS_DIO_MASK, 0);
    }
    else
    {
        /*
         * one or more buttons is still depressed so continue scanning
         */
        OS_eContinueSWTimer(APP_tmrButtonsScan, APP_TIME_MS(10), NULL);
    }
}

/****************************************************************************
 *
 * NAME: vHandleButtonEvent
 *
 * DESCRIPTION:
 * Requests frame to control remote LED according to button press
 *
 * PARAMETERS:      Name            RW  Usage
 *                  sButtonEvent    R   Contains button data
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vHandleButtonEvent(APP_tsEvent sButtonEvent)
{
    PRIVATE PDUM_thAPduInstance s_hAPduInst = PDUM_INVALID_HANDLE;

    s_hAPduInst = PDUM_hAPduAllocateAPduInstance(apduLedControl);
    if (PDUM_INVALID_HANDLE == s_hAPduInst)
    {
        DBG_vPrintf(TRACE_APP_BUTTON, "No APDUs left to send data\n");
        return;
    }

    if (0 == sButtonEvent.sButton.u8Button)
    {
        /* button 0 turns remote LED on */
        PDUM_u16APduInstanceWriteNBO(s_hAPduInst, APDU_LEDCTRL_STATE, "b", 0);
    }
    else if (1 == sButtonEvent.sButton.u8Button)
    {
        /* button 1 turns remote LED off */
        PDUM_u16APduInstanceWriteNBO(s_hAPduInst, APDU_LEDCTRL_STATE, "b", 1);
    }
    vSendLedData(s_hAPduInst);
}

/****************************************************************************
 *
 * NAME: vSendLedControl
 *
 * DESCRIPTION:
 * Generates and sends a frame containing the LED control signal
 *
 * PARAMETERS:      Name        RW  Usage
 *                  hAPduInst   R   Handle of the PDU to be sent
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vSendLedData(PDUM_thAPduInstance hAPduInst)
{
    uint64 u64ExtAddr;
    uint8 u8Status;
    uint8 u8SeqNum;

    u8SeqNum = APP_u8GetSequenceNumber(TRUE);

    if (PDUM_INVALID_HANDLE != hAPduInst)
    {
        /* Get our extended address */
        u64ExtAddr = ZPS_u64NwkNibGetExtAddr(ZPS_pvAplZdoGetNwkHandle());

        (void) PDUM_u16APduInstanceWriteNBO(hAPduInst,
                                            APDU_LEDCTRL_LENGTH,
                                            "bl",
                                            10,
                                            u64ExtAddr);
        PDUM_eAPduInstanceSetPayloadSize(hAPduInst, 10);

        u8Status = ZPS_eAplAfUnicastDataReq(hAPduInst,
                                           JENNIC_HSD_LED_CONTROL_CLUSTER_ID,
                                           AN1122_ZBP_SENSORNODE_SED_LED_CONTROL_ENDPOINT,
                                           AN1122_ZBP_CONTROLLERNODE_LED_CONTROL_ENDPOINT,
                                           0x0000 /* Coordinator */,
                                           ZPS_E_APL_AF_UNSECURE,
                                           0 /* default radius */,
                                           &u8SeqNum);

    }
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
