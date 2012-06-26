/****************************************************************************/
/*!
 *\MODULE              Queue
 *
 *\COMPONENT           $HeadURL: https://www.collabnet.nxp.com/svn/lprf_apps/Application_Notes/JN-AN-1134-ZigBee-PRO-Serial-Cable-Replacement/Tags/Release_1v3-Public/Common/Source/queue.h $
 *
 *\VERSION			   $Revision: 5797 $
 *
 *\REVISION            $Id: queue.h 5797 2010-04-20 12:26:04Z mlook $
 *
 *\DATED               $Date: 2010-04-20 13:26:04 +0100 (Tue, 20 Apr 2010) $
 *
 *\AUTHOR              $Author: mlook $
 *
 *\DESCRIPTION         Queue - public interface.
 *
 * This module provides circular queue functionality.
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

#ifndef  QUEUE_H_INCLUDED
#define  QUEUE_H_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
/* Jennic includes */
#include <jendefs.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/* Functionality defines */
#define QUEUE_STATS 	TRUE	/**< Maintain statistics for queues */

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/** Queue structure */
typedef struct
{
#if QUEUE_STATS
	uint32  u32Put;			/**< Number of characters put into queue */
	uint32  u32Got;			/**< Number of characters got from queue */
	uint32  u32Low;			/**< Number of times queue went low */
	uint32  u32RateCount;	/**< Count for rate */
	uint32  u32RateTimer;	/**< Timer for rate */
#endif
    uint16  u16Head; 		/**< Tx/Rx position in queue to add to */
    uint16  u16Tail; 		/**< Tx/Rx position in queue to remove from */
    uint16  u16Mask;		/**< Tx/Rx queue mask */
    uint16  u16Low;			/**< Low free space level */
    uint16  u16High;		/**< High free space level */
    bool_t	  bLowState;	/**< Reached low free space */
    bool_t	  bLowReached;	/**< Reached low free space */
    bool_t	  bHighReached;	/**< Reached high free space */
#if QUEUE_STATS
	bool_t    bRate;		/**< Run rate calculations */
#endif
    uint8  *pu8Data; 		/**< Tx/Rx queue */
} 	QUEUE_tsData;

/****************************************************************************/
/***        Exported Function Prototypes                                  ***/
/****************************************************************************/
PUBLIC bool_t QUEUE_bInit 		(QUEUE_tsData 	 *psQueue,
					    	 	 uint16 	 	 u16Size,
					    	 	 uint16 	 	 u16Low,
					    	 	 uint16 	 	 u16High,
					    	 	 uint8 		 	*pu8Data);
PUBLIC bool_t QUEUE_bFlush 		(QUEUE_tsData 	 *psQueue);
PUBLIC bool_t QUEUE_bPut		(QUEUE_tsData 	 *psQueue,
					  		 	 uint8   	  	  u8Item);
PUBLIC bool_t QUEUE_bGet		(QUEUE_tsData 	 *psQueue,
				         	 	 uint8  	 	*pu8Item);
PUBLIC bool_t QUEUE_bEmpty		(QUEUE_tsData 	 *psQueue);
PUBLIC bool_t QUEUE_bEmpty_Int	(QUEUE_tsData 	 *psQueue);
PUBLIC bool_t QUEUE_bFull		(QUEUE_tsData 	 *psQueue);
PUBLIC bool_t QUEUE_bLowState 	(QUEUE_tsData 	 *psQueue);
PUBLIC bool_t QUEUE_bHighState	(QUEUE_tsData 	 *psQueue);
PUBLIC bool_t QUEUE_bLowReached (QUEUE_tsData 	 *psQueue);
PUBLIC bool_t QUEUE_bHighReached(QUEUE_tsData 	 *psQueue);
PUBLIC uint16 QUEUE_u16Used		(QUEUE_tsData 	 *psQueue);
PUBLIC uint16 QUEUE_u16Free		(QUEUE_tsData 	 *psQueue);
#if QUEUE_STATS
PUBLIC uint32 QUEUE_u32Put  	(QUEUE_tsData *psQueue);
PUBLIC uint32 QUEUE_u32Got  	(QUEUE_tsData *psQueue);
PUBLIC uint32 QUEUE_u32Low  	(QUEUE_tsData *psQueue);
PUBLIC uint32 QUEUE_u32Rate 	(QUEUE_tsData *psQueue);
PUBLIC void   QUEUE_vTick   	(QUEUE_tsData *psQueue);
#endif


#if defined __cplusplus
}
#endif

#endif  /* QUEUE_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/


