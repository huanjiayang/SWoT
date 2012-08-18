/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <AppHardwareApi.h>
#include <os.h>
#include "os_gen.h"


#include "serial.h"
#include "serialq.h"
#include "uart.h"
#include "ledcontrol.h"
#include "app_led.h"
#include "dbg.h"


/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
PRIVATE bool_t	Uart_Opened[2];


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
    if (u8AHI_UartReadLineStatus(Serial_UART) & E_AHI_UART_LS_THRE)
    {
        if(!bSerialQ_Empty(TX_QUEUE))
        {
            vAHI_UartWriteData(Serial_UART, u8SerialQ_RemoveItem(TX_QUEUE));
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
PUBLIC void vUART_TxCharISR(uint8 u8Uart)
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
//	/* Have we opened UART0 ? */
//	if (Uart_Opened[0])
//	{
		uint8 u8IntStatus;

		u8IntStatus = u8AHI_UartReadInterruptStatus(E_AHI_UART_0);

		/* Pass on to full interrupt routine */
		UART_vInterrupt(E_AHI_DEVICE_UART0, ((u8IntStatus >> 1) & 0x7));
//	}
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
//	if (Uart_Opened[1])
//	{
		uint8 u8IntStatus;

		u8IntStatus = u8AHI_UartReadInterruptStatus(E_AHI_UART_1);


		/* Pass on to full interrupt routine */
		UART_vInterrupt(E_AHI_DEVICE_UART1, (u8IntStatus >> 1) & 0x7);

//	}
}
#endif


/****************************************************************************
 *
 * NAME: UART_vUartInterrupt
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
	uint8 u8Uart = 0xFF;
	uint8 rxChar;
	APP_vLedsInitialise();
	/* Which uart ? */

	if (u32Device == E_AHI_DEVICE_UART0) u8Uart = E_AHI_UART_0;
	if (u32Device == E_AHI_DEVICE_UART1) u8Uart = E_AHI_UART_1;

		if ((u32ItemBitmap & 0xFF) == E_AHI_UART_INT_RXDATA ||
			(u32ItemBitmap & 0xFF) == E_AHI_UART_INT_TIMEOUT)
	{
		while (u8AHI_UartReadLineStatus(u8Uart) & E_AHI_UART_LS_DR)
			{
//				APP_vLedSet(1, TRUE);
			rxChar= u8AHI_UartReadData(u8Uart);
			vUART_RxCharISR(rxChar);
			DBG_vPrintf(TRACE_APP, "%x",  rxChar);


			}
//		vAHI_UartWriteData(u8Uart, rxChar);
//		DBG_vPrintf(TRACE_APP, "%x",  rxChar);
	}
        else if ((u32ItemBitmap & 0xFF) == E_AHI_UART_INT_TX)
      {
        	APP_vLedSet(1, TRUE);
            vUART_TxCharISR(u8Uart);

        }
}



/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
