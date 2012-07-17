/*****************************************************************************
 *
 * MODULE:             Home Sensor Demo
 *
 * COMPONENT:          app_syscon.c
 *
 * AUTHOR:             GPfef
 *
 * DESCRIPTION:       Split system controller interrupt into actions for wake timers, buttons and sensors
 *
 * $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Application_Notes/JN-AN-1122-ZBPro-Home-Sensor-Demo/Tags/ZBPRO-HOME-SENSOR-DEMO_1v7_RC4/AN1122_ZBP_SensorNode/Source/app_syscon.c $
 *
 * $Revision: 16722 $
 *
 * $LastChangedBy: gpfef $
 *
 * $LastChangedDate: 2009-07-15 14:57:39 +0100 (Wed, 15 Jul 2009) $
 *
 * $Id: app_syscon.c 16722 2009-07-15 13:57:39Z gpfef $
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
#include "pwrm.h"
#include "AppHardwareApi.h"
#include "app_home_sensor_demo.h"
#include "app_timer_driver.h"
#include "app_buttons.h"
#include "HtsDriver.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifndef TRACE_APP_SYSCON
#define TRACE_APP_SYSCON            FALSE
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
 * NAME: APP_SysConISR
 *
 * DESCRIPTION:
 * Interrupt
 *
 ****************************************************************************/
OS_ISR(APP_isrSysCon)
{
    /* clear pending DIO changed bits by reading register */
    uint32 u32IntSource = u32AHI_DioInterruptStatus();
    uint8 u8WakeInt = u8AHI_WakeTimerFiredStatus();

    if (u8WakeInt & E_AHI_WAKE_TIMER_MASK_1)
    {
        /* wake timer interrupt got us here */
        DBG_vPrintf(TRACE_APP_SYSCON, "APP: Wake Timer 1 Interrupt\n");
        PWRM_vWakeInterruptCallback();
    }
    else if (u8WakeInt & E_AHI_WAKE_TIMER_MASK_0)
    {
        DBG_vPrintf(TRACE_APP_SYSCON, "APP: Wake Timer 0 Interrupt\n");
    }

    if (u32IntSource & HTS_DATA_DIO_BIT_MASK)
    {
        /* Disable further interrupts */
        vAHI_DioInterruptEnable(0, HTS_DATA_DIO_BIT_MASK);

        DBG_vPrintf(TRACE_APP_SYSCON, "APP: Sample ready\r\n");

        /* activate task to sample sensors now sample is ready */
        OS_eActivateTask(APP_taskSampleSensors);
    }

    if (u32IntSource & APP_BUTTONS_DIO_MASK)
    {
        /* disable edge detection until scan complete */
        vAHI_DioInterruptEnable(0, APP_BUTTONS_DIO_MASK);

        DBG_vPrintf(TRACE_APP_SYSCON, "APP: Button press\r\n");

        OS_eStartSWTimer(APP_tmrButtonsScan, APP_TIME_MS(5), NULL);
    }
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
