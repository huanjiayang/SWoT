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
//PRIVATE void vUART_SetBuadRate(uint32 u32BaudRate);

#if UART == E_AHI_UART_0
PRIVATE void vUART_HandleUart0Interrupt(uint32 u32Device, uint32 u32ItemBitmap);
#else
PRIVATE void vUART_HandleUart1Interrupt(uint32 u32Device, uint32 u32ItemBitmap);
#endif

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
PUBLIC void vUART_Init(void)
{
    /* Enable UART 0 */
    vAHI_UartEnable(UART);

    vAHI_UartReset(UART, TRUE, TRUE);
    vAHI_UartReset(UART, FALSE, FALSE);

    /* Register function that will handle UART interrupts */
    #if UART == E_AHI_UART_0
        vAHI_Uart0RegisterCallback(vUART_HandleUart0Interrupt);
    #else
        vAHI_Uart1RegisterCallback(vUART_HandleUart1Interrupt);
    #endif

    /* Set the clock divisor register to give required buad */
    vAHI_UartSetBaudRate(UART, E_AHI_UART_RATE_115200);

    vAHI_UartSetControl(UART, FALSE, FALSE, E_AHI_UART_WORD_LEN_8, TRUE, FALSE);
    vAHI_UartSetInterrupt(UART, FALSE, FALSE, TRUE, TRUE, E_AHI_UART_FIFO_LEVEL_1);
}

/****************************************************************************
 *
 * NAME: vUART_SetBuadRate
 *
 * DESCRIPTION:
 *
 * PARAMETERS: Name        RW  Usage
 *
 * RETURNS:
 *
 ****************************************************************************/
//PRIVATE void vUART_SetBuadRate(uint32 u32BaudRate)
//{
//    uint16 u16Divisor;
//    uint32 u32Remainder;
//
//    /* Divisor = 16MHz / (16 x baud rate) */
//    u16Divisor = (uint16)(16000000UL / (16UL * u32BaudRate));
//
//    /* Correct for rounding errors */
//    u32Remainder = (uint32)(16000000UL % (16UL * u32BaudRate));
//
//    if (u32Remainder >= ((16UL * u32BaudRate) / 2))
//    {
//        u16Divisor += 1;
//    }
//
//	vAHI_UartSetBaudDivisor(UART, u16Divisor);
//}

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
    if (u8AHI_UartReadLineStatus(UART) & E_AHI_UART_LS_THRE)
    {
        if(!bSerialQ_Empty(TX_QUEUE))
        {
            vAHI_UartWriteData(UART, u8SerialQ_RemoveItem(TX_QUEUE));
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
        vAHI_UartWriteData(UART, u8SerialQ_RemoveItem(TX_QUEUE));
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

#if UART == E_AHI_UART_0
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
PRIVATE void vUART_HandleUart0Interrupt(uint32 u32Device, uint32 u32ItemBitmap)
{
    if (u32Device == E_AHI_DEVICE_UART0)
    {
        if ((u32ItemBitmap & 0x000000FF) == E_AHI_UART_INT_RXDATA)
        {
            vUART_RxCharISR(u8AHI_UartReadData(E_AHI_UART_0));
        }
        else if (u32ItemBitmap == E_AHI_UART_INT_TX)
        {
            vUART_TxCharISR();
        }
    }
}
#else
/****************************************************************************
 *
 * NAME: vUART_HandleUart1Interrupt
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
PRIVATE void vUART_HandleUart1Interrupt(uint32 u32Device, uint32 u32ItemBitmap)
{
    if (u32Device == E_AHI_DEVICE_UART1)
    {
        if ((u32ItemBitmap & 0x000000FF) == E_AHI_UART_INT_RXDATA)
        {
            vUART_RxCharISR(u8AHI_UartReadData(E_AHI_UART_1));
        }
        else if (u32ItemBitmap == E_AHI_UART_INT_TX)
        {
            vUART_TxCharISR();
        }
    }
}
#endif
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
