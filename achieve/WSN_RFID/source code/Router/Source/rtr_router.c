/*****************************************************************************/
/*!
 *\MODULE              ZigBeePRO Router (Application Layer)
 *
 *\COMPONENT           $HeadURL: https://www.collabnet.nxp.com/svn/lprf_apps/Application_Notes/JN-AN-1134-ZigBee-PRO-Serial-Cable-Replacement/Tags/Release_1v3-Public/Router/Source/rtr_router.c $
 *
 *\VERSION			   $Revision: 5797 $
 *
 *\REVISION            $Id: rtr_router.c 5797 2010-04-20 12:26:04Z mlook $
 *
 *\DATED               $Date: 2010-04-20 13:26:04 +0100 (Tue, 20 Apr 2010) $
 *
 *\AUTHOR              $Author: mlook $
 *
 *\DESCRIPTION         ZigBeePRO Router (Application Layer) - implementation.
 *
 * This module provides the ZigBeePRO Router funcitonality and serves as
 * the main module of the Router application.
 */
/*****************************************************************************
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
#include <AppApi.h>
#include <AppHardwareApi.h>
#include <Button.h>
#include <LedControl.h>

/* Stack includes */
#include <dbg.h>
#include <dbg_uart.h>
#include <mac_pib.h>
#include <os.h>
#include "os_gen.h"
#include <pwrm.h>
#include <pdum_nwk.h>
#include <pdum_apl.h>
#include "pdum_gen.h"
#include <pdm.h>
#include <rnd_pub.h>
#include "zps_gen.h"
#include <zps_apl.h>
#include <zps_apl_af.h>
#include <zps_apl_aib.h>
#include <zps_apl_zdo.h>
#include <zps_tsv.h>

/* Application includes */
#include "rtr_router.h"
#include "node.h"
#include "tick.h"
#include "wuart.h"
#include "uart.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void RTR_vInit(void);
PRIVATE void RTR_vUncInt(void);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
static PWRM_DECLARE_CALLBACK_DESCRIPTOR(RTR_PreSleep);
static PWRM_DECLARE_CALLBACK_DESCRIPTOR(RTR_Wakeup);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern void *stack_low_water_mark;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
extern void dbg_vPatchInit(void);
extern void os_vPatchInit(void);

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
	#ifdef DBG_ENABLE
		dbg_vPatchInit();
	#endif
    os_vPatchInit();

	/* Debug */
	DBG_vUartInit(DBG_E_UART_1, DBG_E_UART_BAUD_RATE_115200);
	vAHI_UartSetRTSCTS(DBG_E_UART_1, FALSE);
	DBG_vPrintf(RTR_TRACE, "\n%d RTR  < vAppMain()", NODE_sData.u32Timer);

	/* Initialise and turn on LEDs */
	vLedInitRfd();
	vLedControl(0, TRUE);
	vLedControl(1, TRUE);

	/* Initialise buttons */
	vButtonInitRfd();

	/* Watchdog ? */
	#if NODE_FUNC_WATCHDOG
	{
		DBG_vPrintf(RTR_TRACE, " Stack low water mark = %08x", &stack_low_water_mark);
		*(volatile uint32 *)0x020010e0 = ((uint32)&stack_low_water_mark) | 0x8000000;

		if ((*(volatile uint32 *)0x02000004) & 0x80 )
		{
			DBG_vPrintf(RTR_TRACE, " Watchdog timer has reset device!");
			vAHI_WatchdogStop();
			while(1);
		}
   	}
   	#else
   	{
	    /* Don't use watchdog */
    	vAHI_WatchdogStop();
   	}
   	#endif

	/* Initialise application API */
	u32AppApiInit(NULL, NULL, NULL, NULL, NULL, NULL);

	/* Start OS */
    OS_vStart(RTR_vInit, RTR_vUncInt);

	/* Turn off LEDs */
	vLedControl(0, FALSE);
	vLedControl(1, FALSE);

	/* Idle task commences on exit from OS start call */
    while (TRUE)
    {
		/* Watchdog ? */
		#if NODE_FUNC_WATCHDOG
		{
			/* Restart watchdog */
        	vAHI_WatchdogRestart();
        }
        #endif

        /* Manage power */
        PWRM_vManagePower();
    }
}

/****************************************************************************
 *
 * NAME: vAppRegisterPWRMCallbacks
 *
 * DESCRIPTION:
 * Stack called function for registering PWRM callbacks.
 *
 * RETURNS:
 * Never returns.
 *
 ****************************************************************************/
PUBLIC void vAppRegisterPWRMCallbacks(void)
{
	/* Debug */
    DBG_vUartInit(DBG_E_UART_1, DBG_E_UART_BAUD_RATE_115200);
    DBG_vPrintf(RTR_TRACE, "\n%d RTR  < vAppRegisterPWRMCallbacks()", NODE_sData.u32Timer);

    /* Register the PWRM callbacks */
    PWRM_vRegisterPreSleepCallback(RTR_PreSleep);
    PWRM_vRegisterWakeupCallback(RTR_Wakeup);
}

/****************************************************************************
 *
 * NAME: RTR_PreSleep
 *
 * DESCRIPTION:
 * PreSleep callback
 *
 * RETURNS:
 * Never returns.
 *
 ****************************************************************************/
PWRM_CALLBACK(RTR_PreSleep)
{
	DBG_vPrintf(RTR_TRACE, "\n%d RTR  < RTR_PreSleep()", NODE_sData.u32Timer);
}

/****************************************************************************
 *
 * NAME: RTR_Wakeup
 *
 * DESCRIPTION:
 * Wakeup callback
 *
 * RETURNS:
 * Never returns.
 *
 ****************************************************************************/
PWRM_CALLBACK(RTR_Wakeup)
{
	DBG_vReset();
	DBG_vPrintf(RTR_TRACE, "\n%d RTR  < RTR_Wakeup()", NODE_sData.u32Timer);
}

/****************************************************************************
 *
 * NAME: RTR_ZpsEventTask
 *
 * DESCRIPTION:
 * Called when ZPS Events are raised
 * Main state machine for the router
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
OS_TASK(RTR_ZpsEventsTask)
{
	ZPS_tsAfEvent sStackEvent;

	/* Initialise event - in case there isn't one */
	sStackEvent.eType = ZPS_EVENT_NONE;

	/* Try to collect message */
	if (OS_E_OK != OS_eCollectMessage(APP_msgZpsEvents, &sStackEvent))
	{
		/* No event to process */
		;
	}

	/* Debug */
	DBG_vPrintf(NODE_ZPSE_TRACE, "\n%d ZPSE < ZPSE_ZpsEventTask(%d, ZPS_EVENT_", NODE_sData.u32Timer, NODE_sData.eNwkState);

	switch (sStackEvent.eType)
	{
		case ZPS_EVENT_NONE:
		{
			/* Debug */
			DBG_vPrintf(NODE_ZPSE_TRACE, "NONE)");

			/* No state ? */
			if (NODE_sData.eNwkState == NODE_NWKSTATE_NONE)
			{
				/* Start stack */
				ZPS_teStatus eStatus = ZPS_eAplZdoStartStack();
				DBG_vPrintf(NODE_ZPS_TRACE, "\n%d ZPS  > ZPS_eAplZdoStartStack()=%d", NODE_sData.u32Timer, eStatus);

				/* Failed to start stack */
				if (ZPS_E_SUCCESS != eStatus)
				{
					/* Try again in a short while */
					OS_eStartSWTimer(RTR_ZpsEventsTimer, TICK_TIME_MS(1000), NULL);
				}
				else
				{
					/* Go to discover state */
					NODE_sData.eNwkState = NODE_NWKSTATE_DISCOVER;
				}
			}
			else if (NODE_sData.eNwkState == NODE_NWKSTATE_RESCAN)
			{
				/* Debug */
				DBG_vPrintf(NODE_ZPS_TRACE, "\n%d ZPS  > ZPS_eAplZdoDiscoverNetworks(%x)", NODE_sData.u32Timer, ZPS_psAplAibGetAib()->apsChannelMask);
				/* Restart scan with configured channel mask */
				ZPS_eAplZdoDiscoverNetworks(ZPS_psAplAibGetAib()->apsChannelMask);
				/* Go to discover state */
				NODE_sData.eNwkState = NODE_NWKSTATE_DISCOVER;
			}
		} break;
		case ZPS_EVENT_APS_DATA_INDICATION:			DBG_vPrintf(NODE_ZPSE_TRACE, "APS_DATA_INDICATION)"); break;
		case ZPS_EVENT_NWK_JOINED_AS_ROUTER:
		{
			/* Fill in node data now that we have formed the network */
			NODE_sData.u16Address = NODE_sData.psPib->u16ShortAddr_ReadOnly;
			NODE_sData.u16PanId	 = NODE_sData.psPib->u16PanId_ReadOnly;
			NODE_sData.u64ExtPanId = NODE_sData.psNib->sPersist.u64ExtPanId;

			/* Network is up */
			NODE_sData.eNwkState = NODE_NWKSTATE_UP;

			/* Note MinBe as default */
			NODE_sData.u8DefPibMinBe = NODE_sData.psPib->u8MinBe_ReadOnly;

			/* Debug */
			DBG_vPrintf(NODE_ZPSE_TRACE, "NWK_JOINED_AS_ROUTER ExtAddr=%x:%x NwkAddr=%x ExtPan=%x:%x Pan=%x MinBe=%d)",
				(uint32) (NODE_sData.u64Address >> 32),
				(uint32) (NODE_sData.u64Address &  0xffffffff),
				NODE_sData.u16Address,
				(uint32) (NODE_sData.u64ExtPanId >> 32),
				(uint32) (NODE_sData.u64ExtPanId &  0xffffffff),
				NODE_sData.u16PanId,
				NODE_sData.u8DefPibMinBe);

			/* Turn on joining */
			ZPS_eAplZdoPermitJoining(0xff);

			/* Wuart not running ? */
			if (WUART_sData.u8State == WUART_STATE_NONE)
			{
				/* Put wuart into idle state */
				WUART_vState(WUART_STATE_IDLE);
			}
		} break;
		case ZPS_EVENT_NWK_JOINED_AS_ENDDEVICE:		DBG_vPrintf(NODE_ZPSE_TRACE, "NWK_JOINED_AS_ENDDEVICE)"); break;
		case ZPS_EVENT_NWK_STARTED:					DBG_vPrintf(NODE_ZPSE_TRACE, "NWK_STARTED)"); break;
		case ZPS_EVENT_NWK_FAILED_TO_START:			DBG_vPrintf(NODE_ZPSE_TRACE, "NWK_FAILED_TO_START)"); break;
		case ZPS_EVENT_NWK_FAILED_TO_JOIN:
		{
			/* Debug */
			DBG_vPrintf(NODE_ZPSE_TRACE, "NWK_FAILED_TO_JOIN)");

			/* Return to none state */
			NODE_sData.eNwkState = NODE_NWKSTATE_NONE;
			/* Try start again in a short while */
			OS_eStartSWTimer(RTR_ZpsEventsTimer, TICK_TIME_MS(1000), NULL);
		} break;
		case ZPS_EVENT_NWK_NEW_NODE_HAS_JOINED:
		{
			/* Debug */
			DBG_vPrintf(NODE_ZPSE_TRACE, "NWK_NEW_NODE_HAS_JOINED Lng=%x:%x Sht=%x)",
				(uint32) (sStackEvent.uEvent.sNwkJoinIndicationEvent.u64ExtAddr >> 32),
				(uint32) (sStackEvent.uEvent.sNwkJoinIndicationEvent.u64ExtAddr &  0xffffffff),
				sStackEvent.uEvent.sNwkJoinIndicationEvent.u16NwkAddr);
		} break;
		case ZPS_EVENT_NWK_DISCOVERY_COMPLETE:
		{
			uint8 u8Nwk;
			ZPS_teStatus eStatus;
			ZPS_tsNwkNetworkDescr *psNwkDescr;
			ZPS_tsNwkNetworkDescr *psNwkSelected = NULL;

			/* Debug */
			DBG_vPrintf(NODE_ZPSE_TRACE, "NWK_DISCOVERY_COMPLETE)");

			/* Successful discovery ? */
			if (ZPS_E_SUCCESS	== sStackEvent.uEvent.sNwkDiscoveryEvent.eStatus)
			{
				uint64 u64ExtPanId;

				DBG_vPrintf(NODE_ZPSE_TRACE, " success nwks=%d unscanned=%x", sStackEvent.uEvent.sNwkDiscoveryEvent.u8NetworkCount, sStackEvent.uEvent.sNwkDiscoveryEvent.u32UnscannedChannels);

				/* Set extended PAN ID from profile id and cluster id so we can discover an appropriate network */
				u64ExtPanId  = (uint64) JENNIC_WUART_PROFILE_ID << 48;
				u64ExtPanId += (uint64) JENNIC_WUART_APP_CLUSTER_ID << 32;
				/* Loop through found network descriptors */
				for (u8Nwk = 0; u8Nwk < sStackEvent.uEvent.sNwkDiscoveryEvent.u8NetworkCount; u8Nwk++)
				{
					/* Get pointer to network descriptor */
					psNwkDescr = &sStackEvent.uEvent.sNwkDiscoveryEvent.asNwkDescriptors[u8Nwk];
					/* Is this a suitable network permitting joining ? */
					if ((psNwkDescr->u64ExtPanId & 0xffffffff00000000ULL) == u64ExtPanId &&
					     psNwkDescr->u8PermitJoining                      != 0)
					{
						/* Not already got a selected network ? */
						if (psNwkSelected == NULL)
						{
							/* Use this network */
							psNwkSelected = psNwkDescr;
						}
						/* Is this network 'better' than the currently selected network ? */
						else if (psNwkDescr->u8RouterCapacity > psNwkSelected->u8RouterCapacity)
						{
							/* Use this network instead */
							psNwkSelected = psNwkDescr;
						}
                    }
					/* Debug */
					DBG_vPrintf(NODE_ZPSE_TRACE, "\n\tExt PAN ID = %x:%x",
							(uint32) (psNwkDescr->u64ExtPanId >> 32),
							(uint32) (psNwkDescr->u64ExtPanId &  0xffffffff));
					DBG_vPrintf(NODE_ZPSE_TRACE, "\n\tChannel = %d",
							psNwkDescr->u8LogicalChan);
					DBG_vPrintf(NODE_ZPSE_TRACE, "\n\tStack Profile = %d",
							psNwkDescr->u8StackProfile);
					DBG_vPrintf(NODE_ZPSE_TRACE, "\n\tZigbee Version = %d",
							psNwkDescr->u8ZigBeeVersion);
					DBG_vPrintf(NODE_ZPSE_TRACE, "\n\tPermit Joining = %d",
							psNwkDescr->u8PermitJoining);
					DBG_vPrintf(NODE_ZPSE_TRACE, "\n\tRouter Capacity = %d",
							psNwkDescr->u8RouterCapacity);
					DBG_vPrintf(NODE_ZPSE_TRACE, "\n\tEnd Device Capacity = %d",
							psNwkDescr->u8EndDeviceCapacity);
				}

				/* Try to join network */
				eStatus = ZPS_eAplZdoJoinNetwork(psNwkSelected);
				/* Debug */
				DBG_vPrintf(NODE_ZPS_TRACE, "\n%d ZPS  > ZPS_eAplZdoJoinNetwork()=%d", NODE_sData.u32Timer, eStatus);
				/* Success ? */
				if (ZPS_E_SUCCESS == eStatus)
				{
					/* Now attempting to join */
					NODE_sData.eNwkState = NODE_NWKSTATE_JOIN;
				}
				else
				{
					/* Rescan in a short while */
					DBG_vPrintf(NODE_ZPSE_TRACE, " rescan");
					NODE_sData.eNwkState = NODE_NWKSTATE_RESCAN;
					OS_eStartSWTimer(RTR_ZpsEventsTimer, TICK_TIME_MS(1000), NULL);
				}
			}
			/* Error in discovery ? */
			else
			{
				DBG_vPrintf(NODE_ZPSE_TRACE, " error=%d\n", sStackEvent.uEvent.sNwkDiscoveryEvent.eStatus);
			}

			/* Didn't find a suitable network and didn't fail to join ? */
			if (NODE_sData.eNwkState != NODE_NWKSTATE_JOIN &&
				NODE_sData.eNwkState != NODE_NWKSTATE_RESCAN)
			{
				/* Got some unscanned channels left ? */
				if (0 != sStackEvent.uEvent.sNwkDiscoveryEvent.u32UnscannedChannels)
				{
					/* Debug */
					DBG_vPrintf(NODE_ZPS_TRACE, "\n%d ZPS  > ZPS_eAplZdoDiscoverNetworks(%x)", NODE_sData.u32Timer, sStackEvent.uEvent.sNwkDiscoveryEvent.u32UnscannedChannels);
					/* Continue to look for networks in unscanned channels */
					ZPS_eAplZdoDiscoverNetworks(sStackEvent.uEvent.sNwkDiscoveryEvent.u32UnscannedChannels);
					/* Go to discover state */
					NODE_sData.eNwkState = NODE_NWKSTATE_DISCOVER;
				}
				else
				{
					/* Rescan in a short while */
					DBG_vPrintf(NODE_ZPSE_TRACE, " rescan");
					NODE_sData.eNwkState = NODE_NWKSTATE_RESCAN;
					OS_eStartSWTimer(RTR_ZpsEventsTimer, TICK_TIME_MS(1000), NULL);
				}
			}
		} break;
		case ZPS_EVENT_NWK_LEAVE_INDICATION:			DBG_vPrintf(NODE_ZPSE_TRACE, "NWK_LEAVE_INDICATION)"); break;
		case ZPS_EVENT_NWK_LEAVE_CONFIRM:				DBG_vPrintf(NODE_ZPSE_TRACE, "NWK_LEAVE_CONFIRM)"); break;
		case ZPS_EVENT_NWK_STATUS_INDICATION:			DBG_vPrintf(NODE_ZPSE_TRACE, "NWK_STATUS_INDICATION)"); break;
		case ZPS_EVENT_NWK_ROUTE_DISCOVERY_CONFIRM:		DBG_vPrintf(NODE_ZPSE_TRACE, "NWK_ROUTE_DISCOVERY_CONFIRM)"); break;
		case ZPS_EVENT_APS_DATA_CONFIRM:				DBG_vPrintf(NODE_ZPSE_TRACE, "APS_DATA_CONFIRM)"); break;
		case ZPS_EVENT_ERROR:							DBG_vPrintf(NODE_ZPSE_TRACE, "ERROR)"); break;
		case ZPS_EVENT_NWK_POLL_CONFIRM:				DBG_vPrintf(NODE_ZPSE_TRACE, "NWK_POLL_CONFIRM)"); break;
		case ZPS_EVENT_APS_DATA_ACK:					DBG_vPrintf(NODE_ZPSE_TRACE, "APS_DATA_ACK)"); break;
		default:										DBG_vPrintf(NODE_ZPSE_TRACE, "UNKNOWN)"); break;
	}
}

/****************************************************************************
 *
 * NAME: RTR_AppEpEventsTask
 *
 * DESCRIPTION:
 * Receives data sent to the application endpoint
 *
 ****************************************************************************/
OS_TASK(RTR_AppEpEventsTask)
{
	ZPS_tsAfEvent sAppEpEvent;

	/* Initialise event - in case there isn't one */
	sAppEpEvent.eType = ZPS_EVENT_NONE;

	/* Try to collect message */
	if (OS_E_OK != OS_eCollectMessage(APP_EP_Events, &sAppEpEvent))
	{
		/* No event to process */
		;
	}

	/* Debug */
	DBG_vPrintf(NODE_DATA_TRACE, "\n%d DATA < RTR_AppEpEventsTask(%d, ZPS_EVENT_", NODE_sData.u32Timer, NODE_sData.eNwkState);

	switch (sAppEpEvent.eType)
	{
		/* Data indication ? */
		case ZPS_EVENT_APS_DATA_INDICATION:
		{
			uint8 *pu8APduPayload;
			uint16 u16APduSize = 0;

			/* Get APDU payload size */
			u16APduSize = PDUM_u16APduInstanceGetPayloadSize(sAppEpEvent.uEvent.sApsDataIndEvent.hAPduInst);
			/* Get APDU payload pointer */
			pu8APduPayload = (uint8 *) PDUM_pvAPduInstanceGetPayload(sAppEpEvent.uEvent.sApsDataIndEvent.hAPduInst);

			/* Debug */
			DBG_vPrintf(NODE_DATA_TRACE, "APS_DATA_INDICATION, src=%x len=%d pay=%s",
				sAppEpEvent.uEvent.sApsDataIndEvent.uSrcAddress.u16Addr,   /* DANGER: might not be a short address ! */
				u16APduSize,
				pu8APduPayload);

			/* Pass data to wuart for handling */
			WUART_vRx(sAppEpEvent.uEvent.sApsDataIndEvent.uSrcAddress.u16Addr, u16APduSize, pu8APduPayload);

			/* Free APDU */
			PDUM_eAPduFreeAPduInstance(sAppEpEvent.uEvent.sApsDataIndEvent.hAPduInst);
		} break;

		/* Data confirm ? */
		case ZPS_EVENT_APS_DATA_CONFIRM:
		{
			/* Debug */
			DBG_vPrintf(NODE_DATA_TRACE, "APS_DATA_CONFIRM");
		} break;

		/* Others ? */
		default:
		{
			DBG_vPrintf(NODE_DATA_TRACE, "%d", sAppEpEvent.eType);
		}
	}
}

/****************************************************************************
 *
 * NAME: RTR_TickTimerTask
 *
 * DESCRIPTION:
 * Called regularly off a timer
 *
 ****************************************************************************/
OS_TASK(RTR_TickTimerTask)
{
	/* Tick node */
	NODE_vTick();

	/* Call the wuart tick function */
	WUART_vTick();

	/* Continue timer for next iteration */
    OS_eContinueSWTimer(RTR_TickTimer, NODE_TICKTIMER, NULL);
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: RTR_vInit
 *
 * DESCRIPTION:
 * Initialises Zigbee stack, hardware and application.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void RTR_vInit(void)
{
	/* Debug */
	DBG_vPrintf(RTR_TRACE, "\n%d RTR  < RTR_vInit()", NODE_sData.u32Timer);

	/* Initialise node */
	NODE_vInit();

	/* Initialise WUART */
	WUART_vInit();

	/* Start tick timer */
	OS_eStartSWTimer(RTR_TickTimer, TICK_TIME_MS(1), NULL);
}

/****************************************************************************
 *
 * NAME: RTR_vUncInt
 *
 * DESCRIPTION:
 * Catches any unexpected interrupts
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void RTR_vUncInt(void)
{
	register uint32 u32PICSR, u32PICMR;

	asm volatile ("l.mfspr %0,r0,0x4800" :"=r"(u32PICMR) : );
	asm volatile ("l.mfspr %0,r0,0x4802" :"=r"(u32PICSR) : );

	DBG_vPrintf(RTR_TRACE, "\n%d RTR  < RTR_vUncInt() picmr=%x picsr=%x", u32PICMR, u32PICSR);

	while (1);
}

/* Exception handlers */

OS_ISR(APP_isrBusErrorException)
{
    DBG_vPrintf(TRUE, "Bus error\n");
    DBG_vDumpStack();
    while(1);
}

OS_ISR(APP_isrAlignmentException)
{
    DBG_vPrintf(TRUE, "Align error\n");
    DBG_vDumpStack();
    while(1);
}

OS_ISR(APP_isrIllegalInstruction)
{
    DBG_vPrintf(TRUE, "Illegal error\n");
    DBG_vDumpStack();
    while(1);
}

OS_ISR(APP_isrStackOverflowException)
{
    DBG_vPrintf(TRUE, "StackOverflow error\n");
    DBG_vDumpStack();
    while(1);
}

OS_ISR(APP_isrUnimplementedModuleException)
{
    DBG_vPrintf(TRUE, "Unimplemented error\n");
    DBG_vDumpStack();
    while(1);
}


