/****************************************************************************/
/*!
 *\MODULE              Tick Timer (Application Layer)
 *
 *\COMPONENT           $HeadURL: https://www.collabnet.nxp.com/svn/lprf_apps/Application_Notes/JN-AN-1134-ZigBee-PRO-Serial-Cable-Replacement/Tags/Release_1v3-Public/Common/Source/tick.c $
 *
 *\VERSION			   $Revision: 5797 $
 *
 *\REVISION            $Id: tick.c 5797 2010-04-20 12:26:04Z mlook $
 *
 *\DATED               $Date: 2010-04-20 13:26:04 +0100 (Tue, 20 Apr 2010) $
 *
 *\AUTHOR              $Author: mlook $
 *
 *\DESCRIPTION         Tick Timer (Application Layer) - implementation
 */
/****************************************************************************
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
 * Copyright Jennic Ltd 2010. All rights reserved
 *
 ****************************************************************************/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
/* Jennic includes */
#include <jendefs.h>
#include <AppHardwareApi.h>

/* Generated includes */
#include "os.h"
#include "os_gen.h"
#include "pwrm.h"

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
 * NAME: TICK_vEnable
 *
 * DESCRIPTION:
 * Callback function to enable the tick timer
 *
 * RETURNS:
 * Nothing
 *
 ****************************************************************************/
OS_HWCOUNTER_ENABLE_CALLBACK(TICK_vEnable)
{
    vAHI_TickTimerIntEnable(FALSE);
    vAHI_TickTimerConfigure(E_AHI_TICK_TIMER_CONT);
    vAHI_TickTimerIntPendClr();
    vAHI_TickTimerIntEnable(TRUE);

    PWRM_eStartActivity();
}

/****************************************************************************
 *
 * NAME: TICK_vDisable
 *
 * DESCRIPTION:
 * Callback to disable the tick timer
 *
 * RETURNS:
 * Nothing
 *
 ****************************************************************************/
OS_HWCOUNTER_DISABLE_CALLBACK(TICK_vDisable)
{
	vAHI_TickTimerIntEnable(FALSE);
    vAHI_TickTimerConfigure(E_AHI_TICK_TIMER_DISABLE);

    PWRM_eFinishActivity();
}

/****************************************************************************
 *
 * NAME: TICK_u32Get
 *
 * DESCRIPTION:
 * Callback to read the current value of the tick timer counter
 *
 * RETURNS:
 * Tick timer counter value
 *
 ****************************************************************************/
OS_HWCOUNTER_GET_CALLBACK(TICK_u32Get)
{
    return u32AHI_TickTimerRead();
}

/****************************************************************************
 *
 * NAME: TICK_bSet
 *
 * DESCRIPTION:
 * Set the tick timer compare register
 *
 * RETURNS:
 * True if the compare register value is in the future
 *
 ****************************************************************************/
OS_HWCOUNTER_SET_CALLBACK(TICK_bSet, u32CompareValue)
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

    if (u32CurDelta < u32NextDelta) {
        vAHI_TickTimerInterval(u32CompareValue);
        s_u32CompareTime = u32CompareValue;
        return TRUE;
    }

    return FALSE;
}

/****************************************************************************
 *
 * NAME: TICK_vIsr
 *
 * DESCRIPTION:
 * Interrupt service routine which is invoked by the tick timer when the
 * counter value matches the compare register value
 *
 ****************************************************************************/
OS_ISR(TICK_vIsr)
{
    vAHI_TickTimerIntPendClr();

    /*
     * compare register is only 28bits wide so make sure the upper 4bits match
     * the set compare point
     */
    uint32 u32Delta = u32AHI_TickTimerRead() - s_u32CompareTime;
    if (0 == (u32Delta >> 28)) {
        uint32 u32Temp = s_u32CompareTime;
        OS_eExpireSWTimers(TickCounter);
        s_u32LastExpiredTime = u32Temp;
    }
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

