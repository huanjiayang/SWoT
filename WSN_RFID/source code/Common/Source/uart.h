#ifndef  UART_H_INCLUDED
#define  UART_H_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

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
/* Debug trace defines */
#define UART_TRACE 				FALSE	/**< Uart */

/* Compile options */
#define UART_STATS	    		TRUE	/**< Compile uart statistics */
#define UART_PRINTF  			FALSE	/**< Compile printf support */
#define UART_CHIP_JN5148 		TRUE	/**< Compile for JN5148 */
#define UART_JENOS        		TRUE	/**< Compile for JenOs */

/* Disabled / Enabled flags */
#define UART_ABLE_UARTIO  		0x01	/**< Disabled by UartIO module */
#define UART_ABLE_WUART_PAIR	0x02	/**< Disabled by Wuart module not paired */

/* Standard characters */
#define CHAR_XON				0x11
#define CHAR_XOFF				0x13
#define CHAR_LF					'\n'
#define CHAR_CR					'\r'

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Function Prototypes                                  ***/
/****************************************************************************/
PUBLIC 	bool_t 		UART_bOpen (		/**< Open UART */
		uint8 	        u8Uart,			/**< Uart to open */
		uint32    	   u32BaudRate,		/**< Baud rate */
		bool_t   		 bEvenParity,	/**< Even parity */
		bool_t   		 bEnableParity,	/**< Enable parity */
		uint8      		u8WordLength,	/**< Word length, one of:\n
										 	 E_AHI_UART_WORD_LEN_5\n
										 	 E_AHI_UART_WORD_LEN_6\n
										 	 E_AHI_UART_WORD_LEN_7\n
										 	 E_AHI_UART_WORD_LEN_8 */
		bool_t   		 bOneStopBit,	/**< One stop bit */
		QUEUE_tsData   *psTxQueue,		/**< Transmit queue pointer */
		QUEUE_tsData   *psRxQueue);		/**< Receive queue pointer */

PUBLIC 	bool_t 		UART_bGetChar (		/**< Get character received by UART */
		uint8  	   		u8Uart,			/**< Uart to get character from */
		char  	  	   *pcChar);		/**< Character received pointer */

PUBLIC bool_t 		UART_bPutChar (		/**< Put character for transmission by UART */
		uint8  	   		u8Uart,			/**< Uart to get character from */
		char  	    	 cChar);		/**< Character to transmit */

PUBLIC bool_t 		UART_bWaitEmptyTx 	(uint8 u8Uart);
PUBLIC bool_t 		UART_bOpened 		(uint8 u8Uart);
PUBLIC bool_t 		UART_bRxEnable		(uint8 u8Uart, uint8 u8Enable);
PUBLIC bool_t 		UART_bRxDisable		(uint8 u8Uart, uint8 u8Disable);
PUBLIC bool_t 		UART_bRxEnabled 	(uint8 u8Uart);
PUBLIC bool_t 		UART_bTxEnable		(uint8 u8Uart, uint8 u8Enable);
PUBLIC bool_t 		UART_bTxDisable		(uint8 u8Uart, uint8 u8Disable);
PUBLIC bool_t 		UART_bTxEnabled		(uint8 u8Uart);
PUBLIC void   		UART_vStartTx		(uint8 u8Uart);

#if UART_PRINTF
PUBLIC 	bool_t 		UART_bPrintf (		/**< Format and print to UART */
		uint8 			u8Uart,			/**< Uart to output to */
		const char     *pcFormat,		/**< Formatting string */
		...);
PUBLIC 	bool_t UART_bPrintf_ap (
		uint8 			u8Uart,		/**< Uart to output to */
		const char     *pcFormat,	/**< Formatting string */
		va_list         ap);
#endif

#if UART_STATS
PUBLIC uint32 		UART_u32Txed 		(uint8 u8Uart);
PUBLIC uint32 		UART_u32Rxed 		(uint8 u8Uart);
PUBLIC uint32 		UART_u32TxDisabled 	(uint8 u8Uart);
PUBLIC uint32 		UART_u32RxDisabled 	(uint8 u8Uart);
#endif

#if defined __cplusplus
}
#endif

#endif  /* UART_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/


