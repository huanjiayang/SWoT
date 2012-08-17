/****************************************************************************
 *
 * MODULE:             Wireless UART
 *
 * COMPONENT:          serial.c,v
 *
 * VERSION:
 *
 * REVISION:           1.3
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
 * serial.c,v
 * Revision 1.3  2006/11/09 12:57:10
 * Updated copyright information in file header
 *
 * Revision 1.2  2006/11/06 15:42:10
 * Updated to use interrupt driven comms
 *
 * Revision 1.1  2006/08/24 14:59:26
 * Initial version
 *
 *
 *
 * LAST MODIFIED BY:
 *                     $Modtime: $
 *
 *


////****************************************************************************/
///***        Include files                                                 ***/
///****************************************************************************/
#include <jendefs.h>
#include <AppHardwareApi.h>

#include "uart.h"
#include "serialq.h"
#include "serial.h"
#include "dbg.h"
#include "app_home_sensor_demo.h"
#include "app_log.h"
#include "app_display.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define TX_STRING_END_CHAR   '\0'    /* Strings to be transmitted must end with NULL */
#define RX_STRING_END_CHAR   CR_CHAR /* Input string must end with carriage return */

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
    vUART_Init(Serial_UART, Serial_BAUD, Serial_EVEN, Serial_PARITY, Serial_WORDLEN, Serial_ONESTOP);
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
 *
 *
 *
 ****************************************************************************/
PUBLIC void vSerial_TxOutput(uint8 u8Char[])
{
	uint8 i;
	if (!bSerialQ_Full(TX_QUEUE)) {
		for (i = 0; i < vStrArrlen(u8Char); i++) {
			vSerialQ_AddItem(TX_QUEUE, u8Char[i]);
			vUART_StartTx(); /* Start the tx process if it has stalled */
		}
	}
}

/****************************************************************************/
PUBLIC void vSerial_TxChar(string u8Char)
{
	uint8 i;
	if (!bSerialQ_Full(TX_QUEUE)) {
		for (i = 0; i < vStrlen(u8Char); i++) {
			vSerialQ_AddItem(TX_QUEUE, u8Char[i]);
			vUART_StartTx(); /* Start the tx process if it has stalled */
		}
	}
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
PUBLIC void Rfid_Rawread(void)
{
//	uint16 u16Char[] = {0x8001, 0x0000, 0x5358, 0x0021, 0x0000,
//			0x0008, 0x0001, 0x0013, 0x0000, 0x000F, 0x00FB};
	uint16 u16Char[]= {0x8001, 0x7685};
	uint8 hi_temp, lo_temp;

	hi_temp = u16Char[1];
	lo_temp = u16Char[1]>>8;
	DBG_vPrintf(TRACE_APP, "%x %x",lo_temp, hi_temp );
	vAHI_UartWriteData(Serial_UART, 0x76);
	vAHI_UartWriteData(Serial_UART, 0x85);
	string src;
//	int i;
//	for (i=0; i < 11; i++)
//	{
//		uint8 hi_temp, lo_temp;
//		hi_temp = u16Char[i] ;
//		lo_temp = u16Char[i]>>8;
//		vAHI_UartWriteData(Serial_UART, lo_temp);
//		vAHI_UartWriteData(Serial_UART, hi_temp);
//	}

	src = "Source_0";
	vSerial_TxChar(src);
   /* Start the tx process if it has stalled */
}




PUBLIC void Rfid_Getprotocol(void)
{
	uint16 u16Char[] = {0x8001, 0x0000, 0x0000, 0x5358, 0x0012, 0x0000,
						0x0008, 0x0001, 0x0079};
	uint8 hi_temp, lo_temp;
		uint8 i;
		for (i=0 ; i<8; i++)
		{

			hi_temp = u16Char[i];
			lo_temp = u16Char[i]>>8;
			vAHI_UartWriteData(Serial_UART, lo_temp);
			vAHI_UartWriteData(Serial_UART, hi_temp);
		}
	DBG_vPrintf(TRACE_APP, "Get protocol command sent\n" );

}
/****************************************************************************/
int vStrArrlen(uint8 s[])
{
    int len = 0;

    while (s[len] != '\0')
        len++;
    return len;


}

/****************************************************************************/
int vStrlen(char *s)
{
	   int c = 0;

	   while(*(s+c))
	      c++;

	   return c;
}


PUBLIC void vSerial_SensorData()
{
	APP_tsLogNodeHistory *psNodeHistory;
	uint8 u8Node;
	uint8 u8CurTemp, u8CurHum, u8CurLight, Addr1, Addr2;
	uint16 u16ShtAddr;
//	uint8 seqno;
//	if(seq != 0)
//	{
//
//	}

	// string Output;
	uint8 Output[6];
//	uint8 i=0;
	if ((APP_u8ControllerNodeNumDataSensors()) != 0)
	{
		for (u8Node = 0; u8Node < APP_u8ControllerNodeNumDataSensors(); u8Node++)
		{
			psNodeHistory = APP_psLogGetSensorNodeHistory(u8Node);
			u8CurTemp = psNodeHistory->asNodeSensorData[APP_E_SENSOR_TEMP].u8NowValue;
			u8CurHum =  psNodeHistory->asNodeSensorData[APP_E_SENSOR_HTS].u8NowValue;
			u8CurLight = psNodeHistory->asNodeSensorData[APP_E_SENSOR_ALS].u8NowValue;
			u16ShtAddr = psNodeHistory->u16ShortAddr;
			Addr1 = u16ShtAddr >>8;
			Addr2 = u16ShtAddr;
//			vValToDec(Output, u8CurTemp, " ");
//			while (Output[i] != '\0')
//			{
//			vSerialQ_AddItem(TX_QUEUE, (uint8)Output);
//			vUART_StartTx(); /* Start the tx process if it has stalled */
//			i++;
//			}
			Output[0] = Addr1;
			Output[1] = Addr2;
			Output[2] = u8CurTemp;
			Output[3] = u8CurHum;
			Output[4] = u8CurLight;
			Output[5] = 0xaa;
			DBG_vPrintf(TRACE_APP, "Array Len: %d", vStrArrlen(Output));
			vSerial_TxOutput(Output);
			DBG_vPrintf(TRACE_APP, "Node: %x%x Temp: %d Humidity: %d Light: %d\n", Addr1, Addr2, u8CurTemp, u8CurHum, u8CurLight);

		}
	}
}

PUBLIC void vSerial_QData(void)
{
	uint8 data[100];
	uint8 i = 0;
	while(!bSerialQ_Empty(RX_QUEUE))
       {
		data[i] = u8SerialQ_RemoveItem(RX_QUEUE);
		i++;
       }
//	vSerial_TxOutput(data);
	for (i=0; i < vStrArrlen(data); i++)
	{
		DBG_vPrintf(TRACE_APP,"%x", data[i]);
	}
}

//PUBLIC 	void UART_bPrintf_ap (
//		uint8 			u8Uart,		/**< Uart to output to */
//		const char     *pcFormat,	/**< Formatting string */
//		va_list         ap)
//{
//	/* Uart has been opened ? */
//	if (u8Uart < 2 && asUart[u8Uart].bOpened)
//	{
//		char *bp = (char *)pcFormat;
//		char c;
//		char *p, *t;
//		int32 i;
//		uint8 u8Min, u8Len;
//		char   cPad;
//		char   cSign;
//		bool_t bPrefix;
//		bool_t bLeft;
//		uint64 u64Address;
//
//		while ((c = *bp++)) {
//			if (c != '%') {
//				UART_bPutChar(u8Uart, c);
//				continue;
//			}
//
//			/* Flags ? */
//			bPrefix = FALSE;
//			bLeft   = FALSE;
//			cSign   = 0;
//			cPad   = ' ';
//			while (*bp == '#' || *bp == '-' || *bp == '+' || *bp == ':' || *bp == ' ' || *bp == '0' || *bp == '_')
//			{
//				switch (*bp)
//				{
//				case '#': bPrefix = TRUE; break; /* Number base flag  */
//				case '-': bLeft   = TRUE; break; /* left align flag  */
//				case '+': cSign   = '+';  break; /* sign on positive number flag  */
//				case ' ': cSign   = ' ';  break; /* space on positive number flag  */
//				case ':': cSign   = ':';  break; /* colons in mac address flag */
//				case '0': cPad    = '0';  break; /* pad with 0 flag  */
//				case '_': cPad    = '_';  break; /* pad with underscore flag */
//				}
//				bp++;
//			}
//
//			/* Minimum width ? */
//			u8Min = 0;
//			if (*bp == '*')
//			{
//				u8Min = (uint8) va_arg(ap, uint32);
//				bp++;
//			}
//			else
//			{
//				while (*bp >= '0' && *bp <= '9')
//				{
//					u8Min=u8Min*10+(*bp-'0');
//					bp++;
//				}
//			}
//
//			switch ((c = *bp++)) {
//
//			/* %d - show a decimal value */
//			case 'd':
//				UART_bNum2String(u8Uart, va_arg(ap, uint32), 10, u8Min, cPad, bLeft, 0, bPrefix);
//				break;
//
//			/* %x - show a value in hex */
//			case 'x':
//				UART_bNum2String(u8Uart, va_arg(ap, uint32), 16, u8Min, cPad, bLeft, 0, bPrefix);
//				break;
//
//			/* %m - show mac address in hex */
//			case 'm':
//				if (cSign != ':') cSign = 0;
//				u64Address = va_arg(ap, uint64);
//				UART_bNum2String(u8Uart, (uint32)(u64Address >> 32),        16, 8, '0', FALSE, cSign, bPrefix);
//				if (cSign != 0) UART_bPutChar(u8Uart, cSign);
//				UART_bNum2String(u8Uart, (uint32)(u64Address & 0xffffffff), 16, 8, '0', FALSE, cSign, FALSE);
//				break;
//
//			/* %b - show a value in binary */
//			case 'b':
//				UART_bNum2String(u8Uart, va_arg(ap, uint32), 2, u8Min, cPad, bLeft, cSign, bPrefix);
//				break;
//
//			case 'i':
//				i = va_arg(ap, int32);
//				if(i < 0){
//					cSign = '-';
//					UART_bNum2String(u8Uart, (~i)+1, 10, u8Min, cPad, bLeft, cSign, bPrefix);
//				} else {
//					if (cSign == ':') cSign = 0;
//					UART_bNum2String(u8Uart, i, 10, u8Min, cPad, bLeft, cSign, bPrefix);
//				}
//				break;
//
//			/* %c - show a character */
//			case 'c':
//				UART_bPutChar(u8Uart, va_arg(ap, int));
//				break;
//
//			/* %s - show a string */
//			case 's':
//				p = va_arg(ap, char *);
//				t = p;
//				u8Len = 0;
//				while (*t++) ++u8Len;
//				if (!bLeft && u8Min>u8Len) UART_bPad2Min(u8Uart, 0, u8Min-u8Len, cPad);
//				do {
//					UART_bPutChar(u8Uart, *p++);
//				} while (*p);
//				if (bLeft && u8Min>u8Len) UART_bPad2Min(u8Uart, u8Len, u8Min, cPad);
//				break;
//
//			/* %% - show a % character */
//			case '%':
//				UART_bPutChar(u8Uart, '%');
//				break;
//
//			/* %something else not handled ! */
//			default:
//				UART_bPutChar(u8Uart, '?');
//				UART_bPutChar(u8Uart, c);
//				break;
//			}
//		}
//
//		/* If interrupts are not being serviced start transmitting */
//		if (asUart[u8Uart].bTxInt == FALSE) UART_vStartTx(u8Uart);
//
//		return TRUE;
//	}
//	else
//	{
//		return FALSE;
//	}
//}
//#endif

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
