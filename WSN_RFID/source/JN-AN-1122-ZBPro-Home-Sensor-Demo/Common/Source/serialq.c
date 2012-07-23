

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <AppHardwareApi.h>
#include <os.h>
#include "os_gen.h"

//#include "config.h"
#include "serialq.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define CIRCBUFF_PTR_MASK   0x03FFU
#define MAX_CIRCBUFF_SIZE   1024
#define NBR_QUEUES          2

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct
{
    uint16 u16Head;
    uint16 u16Tail;
    uint8  u8Buff[MAX_CIRCBUFF_SIZE];
} tsCircBuff;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE tsCircBuff sRxQueue, sTxQueue;
PRIVATE const tsCircBuff *apsQueueList[NBR_QUEUES] = { &sRxQueue, &sTxQueue };

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
PRIVATE void vSerialQ_Flush(eQueueRef eQueue);

/****************************************************************************
 *
 * NAME: vSerialQ_Init
 *
 * DESCRIPTION:
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PUBLIC void vSerialQ_Init(void)
{
    vSerialQ_Flush(RX_QUEUE);
    vSerialQ_Flush(TX_QUEUE);
}

/****************************************************************************
 *
 * NAME: vSerialQ_AddItem
 *
 * DESCRIPTION:
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PUBLIC void vSerialQ_AddItem(eQueueRef eQueue, uint8 u8Item)
{
    tsCircBuff *psQueue;
    uint16 u16NextLocation;

    psQueue = (tsCircBuff *)apsQueueList[eQueue]; /* Set pointer to the requested queue */

    u16NextLocation = (psQueue->u16Head + 1) & CIRCBUFF_PTR_MASK;

    if (u16NextLocation != psQueue->u16Tail)
    {
        /* Space available in buffer so add data */
        psQueue->u8Buff[psQueue->u16Head] = u8Item;
        psQueue->u16Head = u16NextLocation;
    }
}

/****************************************************************************
 *
 * NAME: u8SerialQ_RemoveItem
 *
 * DESCRIPTION:
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PUBLIC uint8 u8SerialQ_RemoveItem(eQueueRef eQueue)
{
    uint8 u8Item = 0;
    tsCircBuff *psQueue;

    psQueue = (tsCircBuff *)apsQueueList[eQueue]; /* Set pointer to the requested queue */

    if (psQueue->u16Tail != psQueue->u16Head)
    {
        /* Data available on queue so remove a single item */
        u8Item = psQueue->u8Buff[psQueue->u16Tail];
        psQueue->u16Tail = (psQueue->u16Tail + 1) & CIRCBUFF_PTR_MASK;
    }
    return(u8Item);
}

/****************************************************************************
 *
 * NAME: bSerialQ_Empty
 *
 * DESCRIPTION:
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PUBLIC bool_t bSerialQ_Empty(eQueueRef eQueue)
{
    bool_t bResult = FALSE;
    tsCircBuff *psQueue;

    psQueue = (tsCircBuff *)apsQueueList[eQueue];

    if (psQueue->u16Tail == psQueue->u16Head)
    {
        bResult = TRUE;
    }
    return(bResult);
}

/****************************************************************************
 *
 * NAME: bSerialQ_Full
 *
 * DESCRIPTION:
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PUBLIC bool_t bSerialQ_Full(eQueueRef eQueue)
{
    bool_t bResult = FALSE;
    tsCircBuff *psQueue;
    uint16 u16NextLocation;

    psQueue = (tsCircBuff *)apsQueueList[eQueue];

    u16NextLocation = (psQueue->u16Head + 1) & CIRCBUFF_PTR_MASK;

    if (u16NextLocation == psQueue->u16Tail)
    {
	    bResult = TRUE;
    }
    return(bResult);
}

/****************************************************************************
 *
 * NAME: vSerialQ_Flush
 *
 * DESCRIPTION:
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vSerialQ_Flush(eQueueRef eQueue)
{
    tsCircBuff *psQueue;

    psQueue = (tsCircBuff *)apsQueueList[eQueue]; /* Set pointer to the requested queue */

    psQueue->u16Head  = 0;
    psQueue->u16Tail  = 0;
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
