/****************************************************************************
 *
 * MODULE:             Wireless UART
 *
 * COMPONENT:          uart.c,v
 *
 * VERSION:
 *
 * REVISION:           1.4
 *
 * DATED:              2006/11/09 12:57:10
 *
 * STATUS:             Exp
 *
 * AUTHOR:
 *
 * DESCRIPTION
 *
 * CHANGE HISTORY:
 *
 * uart.c,v
 * Revision 1.4  2006/11/09 12:57:10
 * Updated copyright information in file header
 *
 * Revision 1.3  2006/11/06 15:42:10
 * Updated to use interrupt driven comms
 *
 * Revision 1.2  2006/08/24 15:20:41
 * Fixed bug in baud rate calculation caused by a rounding error
 *
 * Revision 1.1  2006/08/24 14:58:48
 * Initial version
 *
 *
 *
 * LAST MODIFIED BY:
 *                     $Modtime: $
 *
 *
 ****************************************************************************
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
 * Copyright Jennic Ltd 2005, 2006, 2007. All rights reserved
 *
 ****************************************************************************/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <AppHardwareApi.h>

#include "config.h"
#include "serialq.h"
#include "uart.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
PRIVATE bool_t	Uart_Opened[2];
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
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
PRIVATE void UART_vInterrupt(uint32 u32Device, uint32 u32ItemBitmap);

/****************************************************************************
 *
 * NAME: vUART_Init
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
PUBLIC bool_t vUART_Init(uint8 u8Uart,	/**< Uart to open */
		uint32    	   u32BaudRate,		/**< Baud rate */
		bool_t   	     bEvenParity,	/**< Even parity */
		bool_t   	     bEnableParity,	/**< Enable parity */
		uint8      	    u8WordLength,	/**< Word length, one of:\n
										 	 E_AHI_UART_WORD_LEN_5\n
										 	 E_AHI_UART_WORD_LEN_6\n
										 	 E_AHI_UART_WORD_LEN_7\n
										 	 E_AHI_UART_WORD_LEN_8 */
		bool_t   	     bOneStopBit)	/**< One stop bit */
{
	uint16 	  u16Divisor;		/* Baud rate divisor */
	uint32 	  u32Remainder;		/* Baud rate remainder */

/* Valid uart ? */
	if (u8Uart < 2)
	{
		/* Calculate divisor for baud rate = 16MHz / (16 x baud rate) */
		u16Divisor   = (uint16)(16000000UL / (16UL * u32BaudRate));
		/* Correct for rounding errors */
		u32Remainder = (uint32)(16000000UL % (16UL * u32BaudRate));
		if (u32Remainder >= ((16UL * u32BaudRate) / 2)) u16Divisor += 1;

		/* Start with port unopened */
		Uart_Opened[u8Uart]= FALSE;
		vAHI_UartSetRTSCTS(u8Uart, FALSE);


		/* Enable UART 0 */
		vAHI_UartEnable(u8Uart);

		vAHI_UartReset(u8Uart, TRUE, TRUE);
		vAHI_UartReset(u8Uart, FALSE, FALSE);

		Uart_Opened[u8Uart]= TRUE;


		#if !UART_JENOS
		{
			/* Register function that will handle UART interrupts */
			if (u8Uart == E_AHI_UART_0) vAHI_Uart0RegisterCallback(UART_vInterrupt);
			if (u8Uart == E_AHI_UART_1) vAHI_Uart1RegisterCallback(UART_vInterrupt);
		}
		#endif
		/* Set baud rate */
		vAHI_UartSetBaudDivisor(u8Uart, u16Divisor);

		/* Set control */
		vAHI_UartSetControl(u8Uart, bEvenParity, bEnableParity, u8WordLength, bOneStopBit, FALSE);
		/* Set UART interrupts */
		vAHI_UartSetInterrupt(u8Uart,
							  FALSE, 		  				  /* ModemStatus */
							  FALSE,						  /* RxLineStatus */
							  TRUE,							  /* TxFifoEmpty */
							  TRUE,							  /* RxData */
							  E_AHI_UART_FIFO_LEVEL_1);

		return TRUE;
	}
	return FALSE;
}

/****************************************************************************
 *
 * NAME: vUART_StartTx
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
PUBLIC void vUART_StartTx(void)
{
    /* Has interrupt driven transmit stalled (tx fifo is empty) */
    if (u8AHI_UartReadLineStatus(u8Uart) & E_AHI_UART_LS_THRE)
    {
        if(!bSerialQ_Empty(TX_QUEUE))
        {
            vAHI_UartWriteData(u8Uart, u8SerialQ_RemoveItem(TX_QUEUE));
        }
    }
}

/****************************************************************************
 *
 * NAME: vUART_TxCharISR
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
PUBLIC void vUART_TxCharISR(void)
{
    if(!bSerialQ_Empty(TX_QUEUE))
	{
        vAHI_UartWriteData(u8Uart, u8SerialQ_RemoveItem(TX_QUEUE));
	}
}

/****************************************************************************
 *
 * NAME: vUART_RxCharISR
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
PUBLIC void vUART_RxCharISR(uint8 u8RxChar)
{
    vSerialQ_AddItem(RX_QUEUE, u8RxChar);
}

#if UART_JENOS
OS_ISR(UART_vIsr0)
{
	/* Have we opened UART0 ? */
	if (Uart_Opened[0])
	{
		uint8 u8IntStatus;

		u8IntStatus = u8AHI_UartReadInterruptStatus(E_AHI_UART_0);

		/* Pass on to full interrupt routine */
		UART_vInterrupt(E_AHI_DEVICE_UART0, ((u8IntStatus >> 1) & 0x7));
	}
}
#endif

/****************************************************************************/
/**
 * <b>UART_vIsr1</b> &mdash; UART 1 interrupt service routine.
 */
/****************************************************************************/
#if UART_JENOS
OS_ISR(UART_vIsr1)
{
	/* Have we opened UART1 ? */
	if (Uart_Opened[1])
	{
		uint8 u8IntStatus;

		u8IntStatus = u8AHI_UartReadInterruptStatus(E_AHI_UART_1);


		/* Pass on to full interrupt routine */
		UART_vInterrupt(E_AHI_DEVICE_UART1, ((u8IntStatus >> 1) & 0x7));

	}
}
#endif


/****************************************************************************
 *
 * NAME: vUART_HandleUart0Interrupt
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
PRIVATE void UART_vInterrupt(uint32 u32Device, uint32 u32ItemBitmap)
{
	if (u32Device == E_AHI_DEVICE_UART0) u8Uart = E_AHI_UART_0;
	if (u32Device == E_AHI_DEVICE_UART1) u8Uart = E_AHI_UART_1;

        if ((u32ItemBitmap & 0x000000FF) == E_AHI_UART_INT_RXDATA)
        {
            vUART_RxCharISR(u8AHI_UartReadData(u8Uart));
        }
        else if (u32ItemBitmap == E_AHI_UART_INT_TX)
        {
            vUART_TxCharISR();
        }
    }



/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
