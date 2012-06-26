/****************************************************************************/
/*!
 *\MODULE              Queue
 *
 *\COMPONENT           $HeadURL: https://www.collabnet.nxp.com/svn/lprf_apps/Application_Notes/JN-AN-1134-ZigBee-PRO-Serial-Cable-Replacement/Tags/Release_1v3-Public/Common/Source/queue.c $
 *
 *\VERSION			   $Revision: 7114 $
 *
 *\REVISION            $Id: queue.c 7114 2011-03-08 16:02:26Z nxp29761 $
 *
 *\DATED               $Date: 2011-03-08 16:02:26 +0000 (Tue, 08 Mar 2011) $
 *
 *\AUTHOR              $Author: nxp29761 $
 *
 *\DESCRIPTION         Queue - implementation.
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

/* Application includes */
#include "queue.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define QUEUE_RATE_CHAR	FALSE	/**< Calculate rate between ^ and ~ chars or
									 over duration of queue not being empty */

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
/***        Local Constants                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Public Functions                                              ***/
/****************************************************************************/

/****************************************************************************/
/**
 * <b>QUEUE_bInit</b> &mdash; Initialise a queue.
 */
/****************************************************************************/
PUBLIC bool_t QUEUE_bInit (QUEUE_tsData *psQueue,	    /**< Queue to initialise */
					       uint16 		u16Size,	    /**< Size of queue */
					       uint16 		u16Low,		    /**< Low level of free space threshold */
					       uint16 		u16High,		/**< High level of free space threshold */
					       uint8 	   *pu8Data)		/**< Queue buffer */

{
	bool_t bReturn = FALSE;

	/* Valid tx/rx structure ? */
    if (psQueue != (QUEUE_tsData *) NULL)
	{
		/* If queue is null zero the size */
		if (pu8Data == (uint8 *) NULL) u16Size = 0;

		/* Limit the queue sizes to a power of two */
		if      (u16Size >= 0x8000) u16Size = 0x8000;
		else if (u16Size >= 0x4000) u16Size = 0x4000;
		else if (u16Size >= 0x2000) u16Size = 0x2000;
		else if (u16Size >= 0x1000) u16Size = 0x1000;
		else if (u16Size >= 0x0800) u16Size = 0x0800;
		else if (u16Size >= 0x0400) u16Size = 0x0400;
		else if (u16Size >= 0x0200) u16Size = 0x0200;
		else if (u16Size >= 0x0100) u16Size = 0x0100;
		else if (u16Size >= 0x0080) u16Size = 0x0080;
		else if (u16Size >= 0x0040) u16Size = 0x0040;
		else if (u16Size >= 0x0020) u16Size = 0x0020;
		else if (u16Size >= 0x0010) u16Size = 0x0010;
		else 						u16Size = 0;

		/* Sensible size ? */
		if (u16Size != 0)
		{
			/* Set mask and queue pointer */
			psQueue->u16Mask  = u16Size-1;
			psQueue->pu8Data = pu8Data;
			/* Limit low space to a quarter of the queue size */
			if (u16Low > (u16Size >> 2)) u16Low = (u16Size >> 2);
			/* Limit high space to a half of the queue size */
			if (u16High > (u16Size >> 1)) u16High = (u16Size >> 1);
			/* Set the low and high space levels */
			psQueue->u16Low  = u16Low;
			psQueue->u16High = u16High;
			/* Success */
			bReturn = TRUE;
		}
		/* No queue */
		else
		{
			/* Zero mask and queue pointer */
			psQueue->u16Mask  = 0;
			psQueue->pu8Data = (uint8 *) NULL;
			psQueue->u16Low   = 0;
			psQueue->u16High  = 0;
		}

		/* Initialise remaining data */
		psQueue->u16Head      = 0;
		psQueue->u16Tail      = 0;
		psQueue->bLowState    = FALSE;
		psQueue->bLowReached  = FALSE;
		psQueue->bHighReached = FALSE;
	}

	return bReturn;
}

/****************************************************************************/
/**
 * <b>QUEUE_bFlush</b> &mdash; Initialise a queue.
 */
/****************************************************************************/
PUBLIC bool_t QUEUE_bFlush (QUEUE_tsData *psQueue) /**< Queue to initialise */

{
	bool_t bReturn = FALSE;

	/* Valid tx/rx structure ? */
    if (psQueue != (QUEUE_tsData *) NULL)
	{
		/* Stats ? */
		#if QUEUE_STATS
		{
			/* Update stats to reflect removal of remaining data */
			psQueue->u32Got += QUEUE_u16Used(psQueue);
		}
		#endif

		/* Initialise remaining data */
		psQueue->u16Head = 0;
		psQueue->u16Tail = 0;

		/* Were we low on free space before flushing ? */
		if (psQueue->bLowState == TRUE)
		{
			/* We are no longer low on free space */
			psQueue->bLowState    = FALSE;
			psQueue->bLowReached  = FALSE;
			psQueue->bHighReached = FALSE;
		}

		/* Success */
		bReturn = TRUE;
	}

	return bReturn;
}

/****************************************************************************/
/**
 * <b>QUEUE_bPut</b> &mdash; Add a byte to a serial queue
 */
/****************************************************************************/
PUBLIC bool_t QUEUE_bPut(QUEUE_tsData *psQueue, 	/**< Queue to operate on. */
					  	 uint8   	   u8Item)		/**< Byte to add to queue */
{
    uint16 u16NextLocation;
    uint16 u16Tail;
    bool_t   bReturn = FALSE;

	/* Valid tx/rx structure ? */
    if (psQueue != (QUEUE_tsData *) NULL)
	{
		/* Get local copy of the tail - the remove function might use it */
		u16Tail = psQueue->u16Tail;
		/* Stats ? */
		#if QUEUE_STATS
		{
			#if QUEUE_RATE_CHAR
			/* Start character ? */
			if (u8Item == '^')
			#else
			/* Queue is currently empty ? */
			if (psQueue->u16Head == u16Tail)
			#endif
			{
				/* Start rate timer */
				psQueue->u32RateTimer = 0;
				/* Reset rate count */
				psQueue->u32RateCount = 0;
				/* Start timing */
				psQueue->bRate = TRUE;
			}
		}
		#endif
		/* Calculate next location after adding this byte */
		u16NextLocation = (psQueue->u16Head + 1) & psQueue->u16Mask;
		/* Got room to add the byte ? */
		if (u16NextLocation != u16Tail)
		{
			/* Space available in buffer so add data */
			psQueue->pu8Data[psQueue->u16Head] = u8Item;
			psQueue->u16Head = u16NextLocation;

			/* Not currently low on free space ? */
			if (psQueue->bLowState == FALSE)
			{
			    uint16 u16Free;

				/* Calculate the free characters */
				if (u16Tail > psQueue->u16Head) u16Free = u16Tail - psQueue->u16Head;
				else				            u16Free = psQueue->u16Mask + 1 + u16Tail - psQueue->u16Head;
				/* Did we just get low on free space ? */
				if (u16Free == psQueue->u16Low)
				{
					/* Set low state and flag changed */
					psQueue->bLowState    = TRUE;
					psQueue->bLowReached  = TRUE;
					psQueue->bHighReached = FALSE;
					/* Stats ? */
					#if QUEUE_STATS
					{
						/* Increment low counter */
						psQueue->u32Low++;
					}
					#endif
				}
			}

			/* Stats ? */
			#if QUEUE_STATS
			{
				/* Increment put counter */
				psQueue->u32Put++;
			}
			#endif

			/* Operation was successful */
			bReturn = TRUE;
		}
	}

	return bReturn;
}

/****************************************************************************/
/**
 * <b>QUEUE_bGet</b> &mdash; Remove a byte from a serial queue
 */
/****************************************************************************/
PUBLIC bool_t QUEUE_bGet(QUEUE_tsData *psQueue, 	/**< Queue to operate on. */
				       	 uint8  	 *pu8Item)		/**< Location for removed byte */
{
    uint16 u16Head;
    bool_t   bReturn = FALSE;

	/* Valid tx/rx structure ? */
    if (psQueue != (QUEUE_tsData *) NULL && pu8Item != (uint8 *) NULL)
	{
		/* Get local copy of the head - the add function updates it */
		u16Head = psQueue->u16Head;
		/* Data is in the queue ? */
		if (psQueue->u16Tail != u16Head)
		{
			/* Data available on queue so remove a single item */
			*pu8Item = psQueue->pu8Data[psQueue->u16Tail];
			psQueue->u16Tail = (psQueue->u16Tail + 1) & psQueue->u16Mask;

			/* Currently low on free space ? */
			if (psQueue->bLowState == TRUE)
			{
			    uint16 u16Free;

				/* Calculate the free characters */
				if (psQueue->u16Tail > u16Head) u16Free = psQueue->u16Tail - u16Head;
				else				   			u16Free = psQueue->u16Mask + 1 + psQueue->u16Tail - u16Head;
				/* Did we just get a high amount of free space ? */
				if (u16Free == psQueue->u16High)
				{
					/* Set low state and flag changed */
					psQueue->bLowState    = FALSE;
					psQueue->bLowReached  = FALSE;
					psQueue->bHighReached = TRUE;
				}
			}

			/* Stats ? */
			#if QUEUE_STATS
			{
				/* Increment got counter */
				psQueue->u32Got++;
				/* Increment rate count */
				psQueue->u32RateCount++;

				#if QUEUE_RATE_CHAR
				/* End character ? */
				if (*pu8Item == '~')
				#else
				/* Queue is now empty ? */
				if (u16Head == psQueue->u16Tail)
				#endif
				{
					/* Stop timing */
					psQueue->bRate = FALSE;
				}
			}
			#endif

			/* Operation was successful */
			bReturn = TRUE;
		}
	}

    return bReturn;
}

/****************************************************************************/
/**
 * <b>QUEUE_bEmpty</b> &mdash; Returns if specified queue is empty.
 */
/****************************************************************************/
PUBLIC bool_t QUEUE_bEmpty(QUEUE_tsData *psQueue)	/**< Queue to operate on. */
{
    return (psQueue->u16Tail == psQueue->u16Head ? TRUE : FALSE);
}

/****************************************************************************/
/**
 * <b>QUEUE_bEmpty</b> &mdash; Returns if specified queue is empty.
 */
/****************************************************************************/
PUBLIC bool_t QUEUE_bEmpty_Int(QUEUE_tsData *psQueue)	/**< Queue to operate on. */
{
    return (psQueue->u16Tail == psQueue->u16Head ? TRUE : FALSE);
}

/****************************************************************************/
/**
 * <b>QUEUE_bFull</b> &mdash; Returns if specified queue is full.
 */
/****************************************************************************/
PUBLIC bool_t QUEUE_bFull(QUEUE_tsData *psQueue)	/**< Queue to operate on. */
{
    return (((psQueue->u16Head + 1) & psQueue->u16Mask) == psQueue->u16Tail ? TRUE : FALSE);
}

/****************************************************************************/
/**
 * <b>QUEUE_bLowState</b> &mdash; Returns if specified queue is low on free space.
 */
/****************************************************************************/
PUBLIC bool_t QUEUE_bLowState(QUEUE_tsData *psQueue)	/**< Queue to operate on. */
{
    return (psQueue->bLowState);
}

/****************************************************************************/
/**
 * <b>QUEUE_bHighState</b> &mdash; Returns if specified queue is high on free space.
 */
/****************************************************************************/
PUBLIC bool_t QUEUE_bHighState(QUEUE_tsData *psQueue)	/**< Queue to operate on. */
{
    return (! psQueue->bLowState);
}

/****************************************************************************/
/**
 * <b>QUEUE_bLowReached</b> &mdash; Returns if specified queue low level has been reached
 */
/****************************************************************************/
PUBLIC bool_t QUEUE_bLowReached(QUEUE_tsData *psQueue)	/**< Queue to operate on. */
{
	bool_t bReturn = psQueue->bLowReached;
	psQueue->bLowReached = FALSE;
    return bReturn;
}

/****************************************************************************/
/**
 * <b>QUEUE_bHighReached</b> &mdash; Returns if specified queue high level has been reached
 */
/****************************************************************************/
PUBLIC bool_t QUEUE_bHighReached(QUEUE_tsData *psQueue)	/**< Queue to operate on. */
{
	bool_t bReturn = psQueue->bHighReached;
	psQueue->bHighReached = FALSE;
    return bReturn;
}

/****************************************************************************/
/**
 * <b>QUEUE_u16Used</b> &mdash; Returns number of used characters in specified queue
 */
/****************************************************************************/
PUBLIC uint16 QUEUE_u16Used(QUEUE_tsData *psQueue)  /**< Queue to operate on. */
{
    uint16 u16Head;
    uint16 u16Tail;

	/* Get local tail and head values, we don't want them changing under us
	   while we calculate the count */
	u16Tail = psQueue->u16Tail;
	u16Head = psQueue->u16Head;

	/* Calculate the used characters into the head value */
	if (u16Head >= u16Tail) u16Head -= u16Tail;
	else					u16Head =  psQueue->u16Mask + 1 + u16Head - u16Tail;

    return(u16Head); /** \return Number of characters in queue */
}

/****************************************************************************/
/**
 * <b>QUEUE_u16Free</b> &mdash; Returns number of free characters in specified queue
 */
/****************************************************************************/
PUBLIC uint16 QUEUE_u16Free(QUEUE_tsData *psQueue)   /**< Queue to operate on. */
{
    uint16 u16Head;
    uint16 u16Free;

	/* Get local tail and head values, we don't want them changing under us
	   while we calculate the count */
	u16Free = psQueue->u16Tail;
	u16Head = psQueue->u16Head;

	/* Calculate the used characters into the head value */
	if (u16Free > u16Head) u16Free -= u16Head;
	else				   u16Free =  psQueue->u16Mask + 1 + u16Free - u16Head;

	/* Decrement - there is always a byte we never actually use
	   as its easier to have head == tail imply an empty queue */
	u16Free--;

    return(u16Free); /** \return Amount of free space in queue */
}

#if QUEUE_STATS
/****************************************************************************/
/**
 * <b>QUEUE_u32Put</b> &mdash; Returns number of characters added to queue
 */
/****************************************************************************/
PUBLIC uint32 QUEUE_u32Put(QUEUE_tsData *psQueue)
{
	return ((psQueue != (QUEUE_tsData *) NULL) ? psQueue->u32Put : 0);
}
#endif /* QUEUE_STATS */

#if QUEUE_STATS
/****************************************************************************/
/**
 * <b>QUEUE_u32Got</b> &mdash; Returns number of characters removed from queue
 */
/****************************************************************************/
PUBLIC uint32 QUEUE_u32Got(QUEUE_tsData *psQueue)
{
	return ((psQueue != (QUEUE_tsData *) NULL) ? psQueue->u32Got : 0);
}
#endif /* QUEUE_STATS */

#if QUEUE_STATS
/****************************************************************************/
/**
 * <b>QUEUE_u32Low</b> &mdash; Returns number of times queue went low
 */
/****************************************************************************/
PUBLIC uint32 QUEUE_u32Low(QUEUE_tsData *psQueue)
{
	return ((psQueue != (QUEUE_tsData *) NULL) ? psQueue->u32Low : 0);
}
#endif /* QUEUE_STATS */

#if QUEUE_STATS
/****************************************************************************/
/**
 * <b>QUEUE_u32Rate</b> &mdash; Returns rate of queue throughput bytes per second
 */
/****************************************************************************/
PUBLIC uint32 QUEUE_u32Rate(QUEUE_tsData *psQueue)
{
	uint32 u32Rate = 0;

	/* Valid pointer */
	if (psQueue != (QUEUE_tsData *) NULL)
	{
		/* Timer not zero ? */
		if (psQueue->u32RateTimer != 0)
		{
			/* Calculate rate */
			u32Rate = (psQueue->u32RateCount * 100) / psQueue->u32RateTimer;
		}
	}

	return u32Rate;
}
#endif /* QUEUE_STATS */

#if QUEUE_STATS
/****************************************************************************/
/**
 * <b>QUEUE_vTick</b> &mdash; Called regularly to time queue throughput
 */
/****************************************************************************/
PUBLIC void QUEUE_vTick(QUEUE_tsData *psQueue)
{
	/* Valid pointer */
	if (psQueue != (QUEUE_tsData *) NULL)
	{
		/* Timing the rate - increment rate timer */
		if (psQueue->bRate) psQueue->u32RateTimer++;
	}
}
#endif /* QUEUE_STATS */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
