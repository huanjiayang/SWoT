

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <AppHardwareApi.h>
#include <os.h>
#include "os_gen.h"

#include "uart.h"
#include "serialq.h"
#include "serial.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define TX_STRING_END_CHAR   '\0'    /* Strings to be transmitted must end with NULL */
#define RX_STRING_END_CHAR   CR_CHAR /* Input string must end with carriage return */


/****************************************************************************
 *
 * NAME: vSerial_Init
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
PUBLIC void vSerial_Init(void)
{
    /* Initialise the serial port and rx/tx queues */
    vSerialQ_Init();
    vUART_Init(Serial_UART,
			  Serial_BAUD,
			  Serial_EVEN,
			  Serial_PARITY,
			  Serial_WORDLEN,
			  Serial_ONESTOP);
}

/****************************************************************************
 *
 * NAME: vSerial_TxChar
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
PUBLIC void vSerial_TxChar(uint8 u8Chr)
{
    if(!bSerialQ_Full(TX_QUEUE))
    {
        vSerialQ_AddItem(TX_QUEUE, u8Chr);
        vUART_StartTx();      /* Start the tx process if it has stalled */
	}
}

/****************************************************************************
 *
 * NAME: vSerial_TxString
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
PUBLIC void vSerial_TxString( const uint8 *ps)
{
    const uint8 *pu8String;

	/* Copy the string to be transmitted to the transmit queue */

    for(pu8String = ps; (!bSerialQ_Full(TX_QUEUE) && (*pu8String != TX_STRING_END_CHAR)); pu8String++)
    {
        vSerialQ_AddItem(TX_QUEUE, *pu8String);  /* Add character to the transmit queue. */
    }
    vUART_StartTx();      /* Start the tx process if it has stalled */
}

/****************************************************************************
 *
 * NAME: i16Serial_RxChar
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
PUBLIC int16 i16Serial_RxChar(void)
{
    int16 i16Result = -1;

    if(!bSerialQ_Empty(RX_QUEUE))
	{
   	    i16Result = u8SerialQ_RemoveItem(RX_QUEUE);
	}
    return(i16Result);
}

/****************************************************************************
 *
 * NAME: vSerial_Init
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
PUBLIC void vSerialRxString(uint8 *ps)
{
    uint8 *pu8String;
	int16 i16Chr;

    /* Copy the received string from the receive queue */

    for(pu8String = ps; ((i16Chr = i16Serial_RxChar()) != (int16)RX_STRING_END_CHAR); pu8String++)
    {
        *pu8String = (uint8)i16Chr;
    }

    *pu8String = (uint8)'\0'; /* Append NULL character to the end of the string */
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
