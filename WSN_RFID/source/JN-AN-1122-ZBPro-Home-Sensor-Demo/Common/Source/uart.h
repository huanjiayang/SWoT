#ifndef  UART_H_INCLUDED
#define  UART_H_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <jendefs.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC bool_t vUART_Init(uint8 u8Uart,	/**< Uart to open */
						uint32    	   u32BaudRate,		/**< Baud rate */
						bool_t   	     bEvenParity,	/**< Even parity */
						bool_t   	     bEnableParity,	/**< Enable parity */
						uint8      	    u8WordLength,	/**< Word length, one of:\n
											 	 E_AHI_UART_WORD_LEN_5\n
											 	 E_AHI_UART_WORD_LEN_6\n
											 	 E_AHI_UART_WORD_LEN_7\n
											 	 E_AHI_UART_WORD_LEN_8 */
						bool_t   	     bOneStopBit);	/**< One stop bit */
PUBLIC void vUART_StartTx(void);
PUBLIC void vUART_RxCharISR(uint8 u8RxChar);
PUBLIC void vUART_TxCharISR(uint8 u8Uart);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* UART_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/


