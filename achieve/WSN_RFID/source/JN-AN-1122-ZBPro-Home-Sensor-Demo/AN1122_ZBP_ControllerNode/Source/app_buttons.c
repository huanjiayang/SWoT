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
 * $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Application_Notes/JN-AN-1122-ZBPro-Home-Sensor-Demo/Tags/ZBPRO-HOME-SENSOR-DEMO_1v7_RC4/AN1122_ZBP_ControllerNode/Source/app_buttons.c $
 *
 * $Revision: 22329 $
 *
 * $LastChangedBy: mwild $
 *
 * $LastChangedDate: 2009-11-23 18:36:36 +0000 (Mon, 23 Nov 2009) $
 *
 * $Id: app_buttons.c 22329 2009-11-23 18:36:36Z mwild $
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
#include "app_timer_driver.h"
#include "app_buttons.h"
#include "button.h"
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
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
#ifdef APP_CONTROLLER
PRIVATE uint8 s_u8ButtonDebounce[APP_BUTTONS_NUM] =
{ 0xff, 0xff, 0xff, 0xff };
PRIVATE uint8 s_u8ButtonState[APP_BUTTONS_NUM] =
{ 0, 0, 0, 0 };
PRIVATE const uint8 s_u8ButtonDIOLine[APP_BUTTONS_NUM] =
{ APP_BUTTONS_BUTTON_1, APP_BUTTONS_BUTTON_2, APP_BUTTONS_BUTTON_3,
    APP_BUTTONS_BUTTON_4 };
#else
PRIVATE uint8 s_u8ButtonDebounce[APP_BUTTONS_NUM] =
{   0xff, 0xff};
PRIVATE uint8 s_u8ButtonState[APP_BUTTONS_NUM] =
{   0, 0};
PRIVATE const uint8 s_u8ButtonDIOLine[APP_BUTTONS_NUM] =
{
    APP_BUTTONS_BUTTON_1,
    APP_BUTTONS_BUTTON_2
};
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
 * bool_t   Returns TRUE if any button was pressed
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
 * NAME: APP_isrButtonsDioChanged
 *
 * DESCRIPTION:
 * ISR for system controller - Only buttons should be generating interrupts
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
OS_ISR(APP_isrButtonsDioChanged)
{
    uint32 u32IntSource = u32AHI_DioInterruptStatus();
    DBG_vPrintf(TRACE_APP_BUTTON, "ISR 0x%08x\n", u32IntSource);

    /* disable edge detection until scan complete */
    vAHI_DioInterruptEnable(0, APP_BUTTONS_DIO_MASK);
    OS_eStartSWTimer(APP_tmrButtonsScan, APP_TIME_MS(5), NULL);
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

    for (i = 0; i < APP_BUTTONS_NUM; i++)
    {
        uint8 u8Button = (uint8) ((u32DIOState >> s_u8ButtonDIOLine[i]) & 1);

        s_u8ButtonDebounce[i] <<= 1;
        s_u8ButtonDebounce[i] |= u8Button;
        u8AllReleased &= s_u8ButtonDebounce[i];

        if (0 == s_u8ButtonDebounce[i] && !s_u8ButtonState[i])
        {
            s_u8ButtonState[i] = TRUE;

            /*
             * button consistently depressed for 8 scan periods
             * so post message to application task to indicate
             * a button down event
             */
            APP_tsEvent sButtonEvent;
            sButtonEvent.eType = APP_E_EVENT_BUTTON_DOWN;
            sButtonEvent.sButton.u8Button = i;

            DBG_vPrintf(TRACE_APP_BUTTON, "Button DN=%d\n", i);

            OS_ePostMessage(APP_msgButtonEvent, &sButtonEvent);
        }
        else if (0xff == s_u8ButtonDebounce[i] && s_u8ButtonState[i] != FALSE)
        {
            s_u8ButtonState[i] = FALSE;

            /*
             * button consistently released for 8 scan periods
             * so post message to application task to indicate
             * a button up event
             */
            APP_tsEvent sButtonEvent;
            sButtonEvent.eType = APP_E_EVENT_BUTTON_UP;
            sButtonEvent.sButton.u8Button = i;

            DBG_vPrintf(TRACE_APP_BUTTON, "Button UP=%i\n", i);

            OS_ePostMessage(APP_msgButtonEvent, &sButtonEvent);
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

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
