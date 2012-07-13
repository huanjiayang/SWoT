/*****************************************************************************
 *
 * MODULE:             Application
 *
 * COMPONENT:          app_timer_driver.c
 *
 * AUTHOR:             MRW
 *
 * DESCRIPTION:        OS hardware counter driver for tick timer
 *
 * $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Application_Notes/JN-AN-1122-ZBPro-Home-Sensor-Demo/Tags/ZBPRO-HOME-SENSOR-DEMO_1v7_RC4/Common/Source/app_timer_driver.c $
 *
 * $Revision: 15615 $
 *
 * $LastChangedBy: mwild $
 *
 * $LastChangedDate: 2009-07-02 11:32:33 +0100 (Thu, 02 Jul 2009) $
 *
 * $Id: app_timer_driver.c 15615 2009-07-02 10:32:33Z mwild $
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
#include "pwrm.h"
#include "AppHardwareApi.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
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
PRIVATE volatile uint32 s_u32CompareTime = 0;
PRIVATE volatile uint32 s_u32LastExpiredTime = 0;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: APP_cbEnableTickTimer
 *
 * DESCRIPTION:
 * Callback function to enable the tick timer
 *
 * RETURNS:
 * Nothing
 *
 ****************************************************************************/
OS_HWCOUNTER_ENABLE_CALLBACK(APP_cbEnableTickTimer)
{
    vAHI_TickTimerIntEnable(FALSE);
    vAHI_TickTimerConfigure(E_AHI_TICK_TIMER_CONT);
    vAHI_TickTimerIntPendClr();
    vAHI_TickTimerIntEnable(TRUE);

    PWRM_eStartActivity();
}

/****************************************************************************
 *
 * NAME: APP_cbDisableTickTimer
 *
 * DESCRIPTION:
 * Callback to disable the tick timer
 *
 * RETURNS:
 * Nothing
 *
 ****************************************************************************/
OS_HWCOUNTER_DISABLE_CALLBACK(APP_cbDisableTickTimer)
{
    vAHI_TickTimerIntEnable(FALSE);
    vAHI_TickTimerConfigure(E_AHI_TICK_TIMER_DISABLE);

    PWRM_eFinishActivity();
}

/****************************************************************************
 *
 * NAME: APP_cbGetTickTimer
 *
 * DESCRIPTION:
 * Callback to read the current value of the tick timer counter
 *
 * RETURNS:
 * Tick timer counter value
 *
 ****************************************************************************/
OS_HWCOUNTER_GET_CALLBACK(APP_cbGetTickTimer)
{
    return u32AHI_TickTimerRead();
}

/****************************************************************************
 *
 * NAME: SetTickCompare
 *
 * DESCRIPTION:
 * Set the tick timer compare register
 *
 * RETURNS:
 * True if the compare register value is in the future
 *
 ****************************************************************************/
OS_HWCOUNTER_SET_CALLBACK(APP_cbSetTickTimerCompare, u32CompareValue)
{
    /* calculations are relative to the last compare register value to account for counter wrap around */
    uint32 u32NextDelta = u32CompareValue - s_u32LastExpiredTime;
    uint32 u32CurDelta = u32AHI_TickTimerRead() - s_u32LastExpiredTime;

    /*
     * Race condition here. If the tick counter has incremented passed the compare point
     * in the time between reading it and loading the compare register the interrupt will be missed.
     * Increase the delta to take account of this time. To minimise this time, the number of
     * instructions between the register read and write to the interval register should be minimised.
     */
    u32CurDelta += 24;

    if (u32CurDelta < u32NextDelta)
    {
        vAHI_TickTimerInterval(u32CompareValue);
        s_u32CompareTime = u32CompareValue;
        return TRUE;
    }

    return FALSE;
}

/****************************************************************************
 *
 * NAME: TickInterrupt
 *
 * DESCRIPTION:
 * Interrupt service routine which is invoked by the tick timer when the
 * counter value matches the compare register value
 *
 ****************************************************************************/
OS_ISR(APP_isrTickTimer)
{
    vAHI_TickTimerIntPendClr();

    /*
     * compare register is only 28bits wide so make sure the upper 4bits match
     * the set compare point
     */
    uint32 u32Delta = u32AHI_TickTimerRead() - s_u32CompareTime;
    if (0 == (u32Delta >> 28))
    {
        uint32 u32Temp = s_u32CompareTime;
        OS_eExpireSWTimers(APP_cntrTickTimer);
        s_u32LastExpiredTime = u32Temp;
    }
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

