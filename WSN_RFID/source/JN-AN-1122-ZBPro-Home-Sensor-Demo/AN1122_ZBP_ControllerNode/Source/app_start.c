/*****************************************************************************
 *
 * MODULE:             Home Sensor Demo
 *
 * COMPONENT:          app_start.c
 *
 * AUTHOR:             MRW
 *
 * DESCRIPTION:        Startup the home sensor demo application
 *
 * $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Application_Notes/JN-AN-1122-ZBPro-Home-Sensor-Demo/Tags/ZBPRO-HOME-SENSOR-DEMO_1v7_RC4/AN1122_ZBP_ControllerNode/Source/app_start.c $
 *
 * $Revision: 29685 $
 *
 * $LastChangedBy: nxp29741 $
 *
 * $LastChangedDate: 2011-03-02 14:49:31 +0000 (Wed, 02 Mar 2011) $
 *
 * $Id: app_start.c 29685 2011-03-02 14:49:31Z nxp29741 $
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
 * Copyright Jennic Ltd. 2007 All rights reserved
 *
 ****************************************************************************/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "os.h"
#include "os_gen.h"
#include "pwrm.h"
#include "pdum_nwk.h"
#include "pdum_apl.h"
#include "pdum_gen.h"
#include "pdm.h"
#include "dbg.h"
#include "dbg_uart.h"
#include "rnd_pub.h"
#include "zps_gen.h"
#include "zps_apl.h"
#include "zps_apl_af.h"
#include "zps_apl_zdo.h"
#include "zps_tsv.h"
#include "appapi.h"
#include "app_controller_node.h"
#ifdef OVERLAYS_BUILD
#include "ovly.h"
#endif
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifndef TRACE_APP
#define TRACE_APP   FALSE
#endif

#ifndef TRACE_OVERLAYS
#define TRACE_OVERLAYS FALSE
#endif

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void vInitialiseApp(void);
PRIVATE void vUnclaimedInterrupt(void);
#ifdef OVERLAYS_BUILD
PRIVATE void vGrabLock(void);
PRIVATE void vReleaseLock(void);
PRIVATE void vOverlayEvent(OVLY_teEvent eEvent, OVLY_tuEventData *puData);
#endif
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern void *stack_low_water_mark;
PUBLIC bool_t APP_bAppHealthy = FALSE;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
// encryption key for PDM
PRIVATE const tsReg128 g_sKey = { 0x45FDF4C9, 0xAE9A6214, 0x7B27285B, 0xDB7E4557 };
#ifdef OVERLAYS_BUILD
OVLY_tsInitData sInitData;
uint32 u32MutexCount;
#endif
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: vAppMain
 *
 * DESCRIPTION:
 * Entry point for application from a cold start.
 *
 * RETURNS:
 * Never returns.
 *
 ****************************************************************************/
PUBLIC void vAppMain(void)
{

    /* Initialise the debug diagnostics module to use UART0 at 115K Baud;
     * UART 1 cannot be used as it shares DIO with the LEDS */
    DBG_vUartInit(DBG_E_UART_0, DBG_E_UART_BAUD_RATE_115200);

    /*
     * Initialise the stack overflow exception to trigger if the end of the
     * stack is reached. See the linker command file to adjust the allocated
     * stack size.
     */
    vAHI_SetStackOverflow(TRUE, (uint32)&stack_low_water_mark);

    /*
     * Catch resets due to watchdog timer expiry.
     */
    if (bAHI_WatchdogResetEvent())
    {
        DBG_vPrintf(TRACE_APP, "APP: Watchdog timer has reset device!\n");
        /* un-comment to trap watchdog resets here */
        //vAHI_WatchdogStop();
        //while (1);
    }

    /* initialise ROM based software modules */
    u32AppApiInit(NULL, NULL, NULL, NULL, NULL, NULL);

    /* Un-comment this line in order to enable high power module */
    //vAHI_HighPowerModuleEnable(TRUE, TRUE);

    /* start the RTOS */
    OS_vStart(vInitialiseApp, vUnclaimedInterrupt);

    /* idle task commences here */

    while (TRUE)
    {
        /* Re-load the watch-dog timer. Execution must return through the idle
         * task before the CPU is suspended by the power manager. This ensures
         * that at least one task / ISR has executed with in the watchdog period
         * otherwise the system will be reset. APP_bAppHealthy is a belt-and-braces check
         * on correct application functionality and is a flag set by the main task.
         */
        if (APP_bAppHealthy)
        {
            vAHI_WatchdogRestart();
            APP_bAppHealthy = FALSE;
        }

        /*
         * suspends CPU operation when the system is idle or puts the device to
         * sleep if there are no activities in progress
         */
        PWRM_vManagePower();
    }
}

void vAppRegisterPWRMCallbacks(void)
{
    /* nothing to register as device does not sleep */
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: vInitialiseApp
 *
 * DESCRIPTION:
 * Initialises JenOS modules and application.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vInitialiseApp(void)
{

#ifdef OVERLAYS_BUILD
    sInitData.u32ImageOffset =  0;
    sInitData.prGetMutex = vGrabLock;
    sInitData.prReleaseMutex = vReleaseLock;
    sInitData.prOverlayEvent = &vOverlayEvent;
    OVLY_bInit(&sInitData);
#endif

    /* UART 1 cannot be used as it shares DIO with the LEDS */
    DBG_vUartInit(DBG_E_UART_0, DBG_E_UART_BAUD_RATE_115200);

    /* initialise JenOS modules */
    PWRM_vInit(E_AHI_SLEEP_OSCON_RAMON);
    /*
     *  Initialise the PDM, use an application supplied key (g_sKey),
     *  The key value can be set to the desired value here, or the key in eFuse can be used.
     *  To use the key stored in eFuse set the pointer to the key to Null, and remove the
     *  key structure here.
     */
    PDM_vInit(7, 1, 64 * 1024 , NULL, mutexMEDIA, NULL, &g_sKey);
    PDUM_vInit();

    /* initialise application */
    APP_vInitialiseControllerNode();
}

/****************************************************************************
 *
 * NAME: vUnclaimedInterrupt
 *
 * DESCRIPTION:
 * Catches any unexpected interrupts
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vUnclaimedInterrupt(void)
{
    DBG_vPrintf(TRACE_APP, "Unclaimed Interrupt\n");
    while(1);
}

/****************************************************************************
 *
 * NAME: APP_isrBusErrorException
 *
 * DESCRIPTION:
 * Catches any bus error exceptions.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
OS_ISR(APP_isrBusErrorException)
{
    DBG_vPrintf(TRACE_APP, "Bus Error Exception\n");
    DBG_vDumpStack();
    while(1);
}

/****************************************************************************
 *
 * NAME: APP_isrAlignmentException
 *
 * DESCRIPTION:
 * Catches any address alignment exceptions.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
OS_ISR(APP_isrAlignmentException)
{
    DBG_vPrintf(TRACE_APP, "Address Alignment Exception\n");
    DBG_vDumpStack();
    while(1);
}

/****************************************************************************
 *
 * NAME: APP_isrIllegalInstructionException
 *
 * DESCRIPTION:
 * Catches any illegal instruction exceptions.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
OS_ISR(APP_isrIllegalInstructionException)
{
    DBG_vPrintf(TRACE_APP, "Illegal Instruction Exception\n");
    DBG_vDumpStack();
    while(1);
}

/****************************************************************************
 *
 * NAME: APP_isrStackOverflowException
 *
 * DESCRIPTION:
 * Catches any stack overflow exceptions.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
OS_ISR(APP_isrStackOverflowException)
{
    DBG_vPrintf(TRACE_APP, "Stack Overflow Exception\n");
    DBG_vDumpStack();
    while(1);
}

/****************************************************************************
 *
 * NAME: APP_isrUnimplementedModuleException
 *
 * DESCRIPTION:
 * Catches any unimplemented module exceptions.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
OS_ISR(APP_isrUnimplementedModuleException)
{
    DBG_vPrintf(TRACE_APP, "Unimplemented Module Exception\n");
    while(1);
}

#ifdef OVERLAYS_BUILD
/****************************************************************************
 *
 * NAME: vGrabLock
 *
 * DESCRIPTION:
 *  Implements counting semaphore protection (grab routine)
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vGrabLock(void)
{
    if (u32MutexCount == 0)
    {
        OS_eEnterCriticalSection(mutexMEDIA);
    }
    u32MutexCount++;
}


/****************************************************************************
 *
 * NAME: vReleaseLock
 *
 * DESCRIPTION:
 * Implements counting semaphore protection (release routine)
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vReleaseLock(void)
{
    u32MutexCount--;
    if (u32MutexCount == 0)
    {
        OS_eExitCriticalSection(mutexMEDIA);
    }
}

/****************************************************************************
 *
 * NAME: vOverlayEvent
 *
 * DESCRIPTION:
 * Provides debug capability in the overlays.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vOverlayEvent(OVLY_teEvent eEvent, OVLY_tuEventData *puData)
{
    switch (eEvent)
    {
        case OVLY_E_EVENT_LOAD:
            DBG_vPrintf(TRACE_OVERLAYS, "\nLoad %d %x > %d %x",
                    puData->sLoad.u16ReturnPage,
                    puData->sLoad.u32ReturnAddress,
                    puData->sLoad.u16TargetPage,
                    puData->sLoad.u32TargetAddress);
            break;

        case OVLY_E_EVENT_READ:
            DBG_vPrintf(TRACE_OVERLAYS, "\nRead Off %x %d ",
                    puData->sRead.u32Offset,
                    puData->sRead.u16Length);
            break;

        case OVLY_E_EVENT_INTERRUPTED:
            DBG_vPrintf(TRACE_OVERLAYS, "\nAbort");
            break;

        case OVLY_E_EVENT_ERROR_SIZE:
            DBG_vPrintf(TRACE_OVERLAYS, "\nError Size");
            break;

        case OVLY_E_EVENT_ERROR_INDEX:
            DBG_vPrintf(TRACE_OVERLAYS, "\nError Index");
            break;

        case OVLY_E_EVENT_ERROR_CHECKSUM:
            DBG_vPrintf(TRACE_OVERLAYS, "\nError checksum");
            break;

        case OVLY_E_EVENT_FAILED:
            DBG_vPrintf(TRACE_OVERLAYS, "\nLOAD FAILED");
            while(1);
            break;
    }
}
#endif
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
