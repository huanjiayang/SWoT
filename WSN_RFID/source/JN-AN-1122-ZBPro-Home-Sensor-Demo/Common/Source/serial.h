
#ifndef  SERIAL_H_INCLUDED
#define  SERIAL_H_INCLUDED

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
#define CR_CHAR	  0x0DU
#define LF_CHAR	  0x0AU

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/** Serial configuration */
#define Serial_UART 	E_AHI_UART_1			/**< Uart to use */
#define Serial_BAUD		115200					/**< Baud rate */
#define Serial_EVEN		FALSE					/**< Even parity */
#define Serial_PARITY	FALSE					/**< Enable parity */
#define Serial_WORDLEN	E_AHI_UART_WORD_LEN_8	/**< Word length */
#define Serial_ONESTOP	TRUE					/**< One stop bit */


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void vSerial_Init(void);
PUBLIC void vSerial_TxChar(uint8 u8Chr);
PUBLIC void vSerial_TxString(const uint8 *ps);
PUBLIC int16 i16Serial_RxChar(void);
PUBLIC void vSerialRxString(uint8 *ps);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* SERIAL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/


