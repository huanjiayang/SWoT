/* Jennic includes */
#include <jendefs.h>
#include <AppHardwareApi.h>
#include <dbg.h>
#include <os.h>

/* Generated includes */
#include "os_gen.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define UART_ADDRESS_0  	0x02003000UL 	   /**< UART register addresses */
#define UART_ADDRESS_1  	0x02004000UL 	   /**< UART register addresses */
#define UART_OFFSET_AFC		0x0000002CUL	   /**< UART AFC register offset */

#define UART_OFFSET_MCR 	0x00000010UL	  /**< UART MCR register offset */

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/** UART structure */
typedef struct
{
	QUEUE_tsData *psTxQueue;		/**< Transmit structure */
	QUEUE_tsData *psRxQueue;		/**< Receive structure */
	uint32    	 u32Address;		/**< Address of UART in memory */
#if UART_STATS
	uint32	  	 u32Txed;			/**< Number of transmitted characters */
	uint32	  	 u32Rxed;			/**< Number of received characters */
	uint32	  	 u32TxDisabled;    	/**< Number of times transmit disabled */
	uint32	  	 u32RxDisabled;		/**< Number of times receive disabled */
#endif
	uint8      	  u8TxDisable;		/**< Transmit disabled flags */
	uint8      	  u8RxDisable;		/**< Transmit disabled flags */
	bool_t	       bOpened;			/**< Port has been opened */
	bool_t         bTxInt;			/**< Tx interrupt is expected */
} 	tsUart;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void     UART_vInterrupt		(uint32, uint32);
PRIVATE void     UART_vRts  			(uint8, bool_t);

#if 0
PRIVATE void     UART_vAfc  			(uint8, bool_t);
#endif
#if UART_PRINTF
PRIVATE bool_t   UART_bNum2String		(uint8, uint32, uint8, uint8, char, bool_t, char, bool_t);
PRIVATE bool_t   UART_bPad2Min			(uint8, uint8, uint8, char);
#endif

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE tsUart	asUart[2];				 					 /**< Uart data */


/****************************************************************************/
/***        Public Functions                                              ***/
/****************************************************************************/

/****************************************************************************/
/**
 * UART_bOpen - Opens UART.
 *
 * This function opens and configures a UART for input and output. Parameters
 * to this function allow the baud rate, parity, word length and stop bits to
 * be set.
 */
/****************************************************************************/
PUBLIC 	bool_t 		UART_bOpen (
		uint8 	   	    u8Uart,			/**< Uart to open */
		uint32    	   u32BaudRate,		/**< Baud rate */
		bool_t   	     bEvenParity,	/**< Even parity */
		bool_t   	     bEnableParity,	/**< Enable parity */
		uint8      	    u8WordLength,	/**< Word length, one of:\n
										 	 E_AHI_UART_WORD_LEN_5\n
										 	 E_AHI_UART_WORD_LEN_6\n
										 	 E_AHI_UART_WORD_LEN_7\n
										 	 E_AHI_UART_WORD_LEN_8 */
		QUEUE_tsData   *psTxQueue,		/**< Transmit queue pointer */
		QUEUE_tsData   *psRxQueue)		/**< Receive queue pointer */

{
	/* Valid uart ? */
		if (u8Uart < 2)
		{
			/* Uart 0 ? */
			if      (u8Uart == 0)
			{
				/* Set address in memory of UART */
				asUart[u8Uart].u32Address = UART_ADDRESS_0;
			}
			/* Uart 1 ? */
			else if (u8Uart == 1)
			{
				asUart[u8Uart].u32Address = UART_ADDRESS_1;
			}

			/* Note the queue pointers */
			asUart[u8Uart].psTxQueue = psTxQueue;
			asUart[u8Uart].psRxQueue = psRxQueue;

			/* Start not expected transmit interrupt */
			asUart[u8Uart].bTxInt = FALSE;

			/* Start with transmit and receive enabled */
			asUart[u8Uart].u8TxDisable = 0;
			asUart[u8Uart].u8RxDisable = 0;

			/* Start with port unopened */
			asUart[u8Uart].bOpened = FALSE;

			/* UART operating in 2-wire mode */
			vAHI_UartSetRTSCTS();
			/* Enable UART */
			vAHI_UartEnable(u8Uart);
			/* Reset UART */
			vAHI_UartReset(u8Uart, TRUE, TRUE);
			vAHI_UartReset(u8Uart, FALSE, FALSE);
			/* Note we've opened the UART */
			asUart[u8Uart].bOpened = TRUE;

			/* Set baud rate */
			vAHI_UartSetBaudRate(u8Uart, E_AHI_UART_RATE_115200);
			/* Set control */
			vAHI_UartSetControl(u8Uart, bEvenParity, bEnableParity, u8WordLength, bOneStopBit, (asUart[u8Uart].u8RxDisable == 0) ? TRUE : FALSE);


			/* Set UART interrupts */
					vAHI_UartSetInterrupt(u8Uart,
										  FALSE, 		 				 /* ModemStatus */
										  FALSE,						  /* RxLineStatus */
										  TRUE,							  /* TxFifoEmpty */
										  TRUE,							  /* RxData */
										  E_AHI_UART_FIFO_LEVEL_14);

					return TRUE;		   /** \retval TRUE Valid UART specified. */
				}

				return FALSE;			   /** \retval FALSE Invalid UART specified. */
			}
		}

}

