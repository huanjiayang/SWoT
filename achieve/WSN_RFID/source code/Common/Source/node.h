/*****************************************************************************/
/*!
 *\MODULE              Node (Application Layer)
 *
 *\COMPONENT           $HeadURL: https://www.collabnet.nxp.com/svn/lprf_apps/Application_Notes/JN-AN-1134-ZigBee-PRO-Serial-Cable-Replacement/Tags/Release_1v3-Public/Common/Source/node.h $
 *
 *\VERSION			   $Revision: 5797 $
 *
 *\REVISION            $Id: node.h 5797 2010-04-20 12:26:04Z mlook $
 *
 *\DATED               $Date: 2010-04-20 13:26:04 +0100 (Tue, 20 Apr 2010) $
 *
 *\AUTHOR              $Author: mlook $
 *
 *\DESCRIPTION         Node (Application Layer) - public interface.
 *
 * This module provides functionality common to all nodes.
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

#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
/* Jennic includes */
#include <jendefs.h>
#include <mac_pib.h>
#include <zps_nwk_nib.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/* Trace debug defines */
#define NODE_TRACE			FALSE					/**< Trace node */
#define NODE_ZPS_TRACE		FALSE					/**< Trace ZigBee stack */
#define NODE_ZPSE_TRACE		FALSE					/**< Trace ZigBee stack events */
#define NODE_DATA_TRACE		FALSE					/**< Trace ZigBee stack end point events */

/* Functionality defines */
#define NODE_FUNC_WATCHDOG	TRUE					/**< Use watchdog */

/* General defines */
#define NODE_ADDRESS_NULL	0xffffffffffffffffULL	/**< Null address */
#define NODE_PAYLOAD		80						/**< Payload size */

/* Timer defines */
#define NODE_TICKTIMER  	TICK_TIME_MS(10) 		/**< Tick timer interval */
#define NODE_TICKTRACE  	0x3ff			  	   	/**< Tick timer trace mask */

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/** Network state type */
typedef enum
{
	NODE_NWKSTATE_NONE,				/**< None */
	NODE_NWKSTATE_CREATE,			/**< Creating network (coordinator) */
	NODE_NWKSTATE_RESCAN,			/**< Rescan for network */
	NODE_NWKSTATE_DISCOVER,			/**< Discover network */
	NODE_NWKSTATE_JOIN,				/**< Join network */
	NODE_NWKSTATE_UP				/**< Network is up */
} NODE_teNwkState;

/** Node information structure */
typedef struct
{
	uint64			u64ExtPanId;	/**< Extended PAN ID */
	uint64 	  		u64Address;		/**< Address of node */
	uint64	  		u64Parent;		/**< Address of node's parent */
	uint32 	  		u32Timer;		/**< Internal timer */
	uint32 	  		u32TimerSync;	/**< Synchronised timer */
	uint16    		u16Address;		/**< Address of node */
	uint16    		u16Parent;		/**< Address of node's parent */
	uint16 	  		u16PanId;		/**< PAN ID of network */
	uint16 	  		u16Depth;		/**< Depth in network */
	uint16 	 		u16MaxDepth;	/**< Maximum depth of network */
	uint8      	 	 u8Channel;		/**< Channel of network */
	uint8			 u8SeqTx;		/**< Transmit sequence number */
	uint8			 u8DefPibMinBe;	/**< Default minimum be */
	bool_t	      	  bHighPower;	/**< High Power module flag */
	NODE_teNwkState   eNwkState;	/**< Network state */
	void 	  		*pvMac;			/**< MAC pointer */
	MAC_Pib_s 		*psPib;			/**< MAC Pib pointer */
	void 	  		*pvNwk;			/**< NWK pointer */
	ZPS_tsNwkNib 	*psNib;			/**< NWK Nib pointer */
} NODE_tsData;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void NODE_vInit(void);
PUBLIC void NODE_vTick(void);
PUBLIC void NODE_vTx(uint64, uint16, uint8 *);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern PUBLIC NODE_tsData	NODE_sData;		/**< Node information */

#endif /* NODE_H_INCLUDED */
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
