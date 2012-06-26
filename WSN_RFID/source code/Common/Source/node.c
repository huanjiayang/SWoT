/*****************************************************************************/
/*!
 *\MODULE              Node (Application Layer)
 *
 *\COMPONENT           $HeadURL: https://www.collabnet.nxp.com/svn/lprf_apps/Application_Notes/JN-AN-1134-ZigBee-PRO-Serial-Cable-Replacement/Tags/Release_1v3-Public/Common/Source/node.c $
 *
 *\VERSION			   $Revision: 7114 $
 *
 *\REVISION            $Id: node.c 7114 2011-03-08 16:02:26Z nxp29761 $
 *
 *\DATED               $Date: 2011-03-08 16:02:26 +0000 (Tue, 08 Mar 2011) $
 *
 *\AUTHOR              $Author: nxp29761 $
 *
 *\DESCRIPTION         Node (Application Layer) - implementation.
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
/* Standard includes */
#include <string.h>

/* Stack includes */
#include <AppApi.h>
#include <Button.h>
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
#include <zps_nwk_nib.h>
#include <zps_nwk_pub.h>
#include <zps_tsv.h>

/* Application includes */
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

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
PUBLIC NODE_tsData	NODE_sData;						  /**< Node information */

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: NODE_vInit
 *
 * DESCRIPTION:
 * Initialises Zigbee stack, hardware and application.
 *
 ****************************************************************************/
PUBLIC void NODE_vInit(void)
{
    uint32 u32RandomSeed;

	/* Debug */
	DBG_vPrintf(NODE_TRACE, "\n%d NODE < NODE_vInit()", NODE_sData.u32Timer);

    /* Seed the pseudo random number generator using the hardware random gen */
    *((volatile uint32 *)0x02001010) |= (1 << 30); /* clear ready flag */
    vAHI_StartRandomNumberGenerator(TRUE /* single shot */, FALSE /* no ints */);
    while (!bAHI_RndNumPoll());
    u32RandomSeed = ((uint32)u16AHI_ReadRandomNumber()) << 16;

    *((volatile uint32 *)0x02001010) |= (1 << 30); /* clear ready flag */
    vAHI_StartRandomNumberGenerator(TRUE /* single shot */, FALSE /* no ints */);
    while (!bAHI_RndNumPoll());
    u32RandomSeed |= u16AHI_ReadRandomNumber();

	RND_vInit(u32RandomSeed);
    DBG_vPrintf(NODE_TRACE, " RND=%08x", u32RandomSeed);

    /* Initialise JenOS modules */
    PWRM_vInit(E_AHI_SLEEP_OSCON_RAMON);
	DBG_vPrintf(NODE_TRACE, " PWRM");

    /* Initialise persistent data */
    PDM_vInit(7, 1, 64*1024, NULL, mutexPdmMedia, NULL, NULL);
	DBG_vPrintf(NODE_TRACE, " PDM");
	/* Delete persistent data - always start over for now */
    PDM_vDelete();
	DBG_vPrintf(NODE_TRACE, "deleted");

    PDUM_vInit();
	DBG_vPrintf(NODE_TRACE, " PDUM");

    /* Initialise ZigbeePro stack */
    ZPS_eAplAfInit();
    DBG_vPrintf(NODE_TRACE, " ZPS");

	/* No state yet */
	NODE_sData.eNwkState = NODE_NWKSTATE_NONE;

	/* Initialise non-zero node structure members */
	NODE_sData.u64Address   = *(uint64 *) pvAppApiGetMacAddrLocation();	/**< Address of node */
	NODE_sData.u16Address   = 0xffff;									/**< Short address of node */
	NODE_sData.u16Parent    = 0xffff;									/**< Short address of node's parent */

	/* Initialise node structure pointer members */
	NODE_sData.pvMac = (void *) pvAppApiGetMacHandle();
	NODE_sData.psPib = MAC_psPibGetHandle(NODE_sData.pvMac);
	NODE_sData.pvNwk = ZPS_pvNwkGetHandle();
	NODE_sData.psNib = ZPS_psNwkNibGetHandle(NODE_sData.pvNwk);

	/* Note initial MinBe */
	NODE_sData.u8DefPibMinBe = NODE_sData.psPib->u8MinBe_ReadOnly;
}

/****************************************************************************
 *
 * NAME: NODE_vTick
 *
 * DESCRIPTION:
 * Called regularly off a timer
 *
 ****************************************************************************/
PUBLIC void NODE_vTick (void)
{
	/* Increment node tick counters */
	NODE_sData.u32Timer++;
	NODE_sData.u32TimerSync++;

	/* Debug this tick ? */
	if ((NODE_sData.u32Timer & NODE_TICKTRACE) == 0)
	{
		/* Debug */
		DBG_vPrintf(NODE_TRACE, "\n%d NODE < NODE_TickTimerTask()", NODE_sData.u32Timer);
	}

}

/****************************************************************************
 *
 * NAME: NODE_vTx
 *
 * DESCRIPTION:
 * Transmit data over the radio
 *
 ****************************************************************************/
PUBLIC void NODE_vTx(uint64 u64DstAddr, uint16 u16Size, uint8 *pu8Payload)
{
    PDUM_thAPduInstance hAPduInstTx = PDUM_INVALID_HANDLE;
    uint8   u8Status;
    uint16 u16APduSize;
    uint16 u16DstAddr;
	uint8 *pu8APduPayload;

	/* Debug */
	DBG_vPrintf(NODE_DATA_TRACE, "\n%d DATA < NODE_vTx(%x:%x %d %s)",
		NODE_sData.u32Timer,
		(uint32)(u64DstAddr >> 32),
		(uint32)(u64DstAddr & 0xffffffff),
		u16Size,
		pu8Payload);

	/* Valid address ? */
	if (u64DstAddr != NODE_ADDRESS_NULL)
	{
		/* Not allocated APDU ? */
		if (PDUM_INVALID_HANDLE == hAPduInstTx)
		{
			/* Try to allocate APDU */
			hAPduInstTx = PDUM_hAPduAllocateAPduInstance(apduAppEpTxData);
			/* Debug */
			DBG_vPrintf(NODE_DATA_TRACE, " allocated apdu");
		}

		/* Not allocated ? */
		if (PDUM_INVALID_HANDLE == hAPduInstTx)
		{
			/* Debug */
			DBG_vPrintf(NODE_DATA_TRACE, " no apdu");
		}
		/* Allocated */
		else
		{
			/* Get APDU size */
			u16APduSize = PDUM_u16APduGetSize(apduAppEpTxData);
			/* Too small ? */
			if (u16APduSize < u16Size)
			{
				/* Free APDU */
				PDUM_eAPduFreeAPduInstance(hAPduInstTx);
				/* Debug */
				DBG_vPrintf(NODE_DATA_TRACE, " apdu too small");
			}
			else
			{
				/* Get APDU payload pointer */
				pu8APduPayload = (uint8 *) PDUM_pvAPduInstanceGetPayload(hAPduInstTx);
				/* Copy data into the APDU */
				memcpy(pu8APduPayload, pu8Payload, u16Size);
				/* Set final size of message */
				PDUM_eAPduInstanceSetPayloadSize(hAPduInstTx, u16Size);

				/* Lower MinBe while we send this for speed */
				MAC_vPibSetMinBe(NODE_sData.pvMac, 3);

				/* IEEE address ? */
				if (u64DstAddr > (uint64) 0xffff)
				{
					/* Unicast to IEEE address data */
					u8Status = ZPS_eAplAfUnicastIeeeDataReq(
						hAPduInstTx,
						JENNIC_WUART_APP_CLUSTER_ID,
						WUART_ROUTER_APP_ENDPOINT,		/* Both endpoints should be the same */
						WUART_COORDINATOR_APP_ENDPOINT,  /* Both endpoints should be the same */
						u64DstAddr,
						ZPS_E_APL_AF_UNSECURE,
						0 /* default radius */,
						&NODE_sData.u8SeqTx);
					/* Debug */
					DBG_vPrintf(NODE_DATA_TRACE, "\n%d DATA > ZPS_eAplAfUnicastDataReq(seq=%d dst=%x:%x len=%d)=%d",
						NODE_sData.u32Timer,
						NODE_sData.u8SeqTx,
						(uint32)(u64DstAddr >> 32),
						(uint32)(u64DstAddr & 0xffffffff),
						u16Size,
						u8Status);
				}
				else
				{
					/* Use short address */
					u16DstAddr = (uint16) u64DstAddr;
					/* Broadcast ? */
					if (u16DstAddr >= 0xfff0)
					{
						/* Broadcast data */
						u8Status = ZPS_eAplAfBroadcastDataReq(
							hAPduInstTx,
							JENNIC_WUART_APP_CLUSTER_ID,
							WUART_ROUTER_APP_ENDPOINT,		/* Both endpoints should be the same */
							WUART_COORDINATOR_APP_ENDPOINT,  /* Both endpoints should be the same */
							u16DstAddr,
							ZPS_E_APL_AF_UNSECURE,
							0 /* default radius */,
							&NODE_sData.u8SeqTx);
						/* Debug */
						DBG_vPrintf(NODE_DATA_TRACE, "\n%d DATA > ZPS_eAplAfBroadcastDataReq(seq=%d dst=%x len=%d)=%d", NODE_sData.u32Timer, NODE_sData.u8SeqTx, u16DstAddr, u16Size, u8Status);
					}
					/* Unicast ? */
					else
					{
						/* Unicast data */
						u8Status = ZPS_eAplAfUnicastDataReq(
							hAPduInstTx,
							JENNIC_WUART_APP_CLUSTER_ID,
							WUART_ROUTER_APP_ENDPOINT,		/* Both endpoints should be the same */
							WUART_COORDINATOR_APP_ENDPOINT,  /* Both endpoints should be the same */
							u16DstAddr,
							ZPS_E_APL_AF_UNSECURE,
							0 /* default radius */,
							&NODE_sData.u8SeqTx);
						/* Debug */
						DBG_vPrintf(NODE_DATA_TRACE, "\n%d DATA > ZPS_eAplAfUnicastDataReq(seq=%d dst=%x len=%d)=%d", NODE_sData.u32Timer, NODE_sData.u8SeqTx, u16DstAddr, u16Size, u8Status);
					}
				}

				/* Restore default MinBe now we've sent */
				MAC_vPibSetMinBe(NODE_sData.pvMac, NODE_sData.u8DefPibMinBe);

				/* Payload looks like a string ? */
				if (pu8APduPayload[u16Size-1] == '\0')
				{
					/* Debug */
					DBG_vPrintf(NODE_DATA_TRACE, " pay='%s'", pu8APduPayload);
				}

				/* Increment sequence number */
				NODE_sData.u8SeqTx++;
			}
		}
	}
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

