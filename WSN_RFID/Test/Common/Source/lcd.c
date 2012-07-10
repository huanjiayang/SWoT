/****************************************************************************/
/*!
 *\MODULE              LCD (Application Layer)
 *
 *\COMPONENT           $HeadURL: https://www.collabnet.nxp.com/svn/lprf_apps/Application_Notes/JN-AN-1134-ZigBee-PRO-Serial-Cable-Replacement/Tags/Release_1v3-Public/Common/Source/lcd.c $
 *
 *\VERSION			   $Revision: 5797 $
 *
 *\REVISION            $Id: lcd.c 5797 2010-04-20 12:26:04Z mlook $
 *
 *\DATED               $Date: 2010-04-20 13:26:04 +0100 (Tue, 20 Apr 2010) $
 *
 *\AUTHOR              $Author: mlook $
 *
 *\DESCRIPTION         LCD (Application Layer) - implementation.
 */
/****************************************************************************
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
 * Copyright Jennic Ltd 2010. All rights reserved
 *
 ****************************************************************************/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
/* Standard includes */
#include <string.h>
/* Jennic includes */
#include <jendefs.h>
#include <LcdDriver.h>
/* Application includes */
#include "lcd.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define LCD_HEX		 FALSE		/**< Display in hex or decimal */
#define COL_TEXT 		40		/**< Column to display text field */
#define COL_LEFT 	 	 0		/**< Column to display first value field */
#define COL_RIGHT 		92		/**< Column to display second value field */
#define MAX_TEXT 		22		/**< Maximum characters in text field */

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/** Row definition */
typedef struct
{
    char   sText[MAX_TEXT+1];	/**< Text for row */
    uint32 u32Left;				/**< Left value for row */
    uint32 u32Right;			/**< Right value for row */
    uint8   u8WidLeft;			/**< Left value number of digits */
    uint8   u8WidRight;			/**< Right value number of digits */
} tsRow;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
#if LCD_HEX
PRIVATE void LCD_vWriteHex(uint32, uint8, uint8, uint8);
#else
PRIVATE void LCD_vWriteDec(uint32, uint8, uint8, uint8);
#endif

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE tsRow asRow[8];						   /**< Data for page, (8 rows) */

/****************************************************************************/
/**
 * <b>LCD_vInit</b> &mdash; Initialises display
 *
 ****************************************************************************/
PUBLIC void LCD_vInit(void)
{
	vLcdResetDefault();
	memset(asRow, 0, sizeof(asRow));
}

/****************************************************************************/
/**
 * <b>LCD_vText</b> &mdash; Set text for display row
 *
 ****************************************************************************/
PUBLIC void LCD_vText(uint8 u8Row, 		/**< Row to set text for */
					  char  sText) 	/**< Text to display */
{
	if (u8Row < 8)
	{
		strncpy (asRow[u8Row].sText, sText, MAX_TEXT);
	}
}

/****************************************************************************/
/**
 * <b>LCD_vLeft</b> &mdash; Set left value for display row
 *
 ****************************************************************************/
PUBLIC void LCD_vLeft(uint8   u8Row, 	/**< Row to set left value for */
					  uint32 u32Val, 	/**< Value to display */
					  uint8   u8Wid)	/**< Digits to display */
{
	if (u8Row < 8)
	{
		asRow[u8Row].u32Left = u32Val;
		if (u8Wid > 6) 	asRow[u8Row].u8WidLeft = 6;
		else			asRow[u8Row].u8WidLeft = u8Wid;
	}
}

/****************************************************************************/
/**
 * <b>LCD_vRight</b> &mdash; Set right value for display row
 *
 ****************************************************************************/
PUBLIC void LCD_vRight(uint8  u8Row, 	/**< Row to set right value for */
					  uint32 u32Val, 	/**< Value to display */
					  uint8   u8Wid)	/**< Digits to display */
{
	if (u8Row < 8)
	{
		asRow[u8Row].u32Right = u32Val;
		if (u8Wid > 6) 	asRow[u8Row].u8WidRight = 6;
		else			asRow[u8Row].u8WidRight = u8Wid;
	}
}

/****************************************************************************/
/**
 * <b>LCD_vDraw</b> &mdash; Draw the display
 *
 ****************************************************************************/
PUBLIC void LCD_vDraw(void)
{
	uint8 u8Row;

	/* Clear LCD */
	vLcdClear();
	/* Loop through LCD lines */
	for (u8Row = 0; u8Row < 8; u8Row++)
	{
		/* Something to Display ? */
		if (asRow[u8Row].sText[0] != '\0')
		{
			/* Ensure termination of line */
			asRow[u8Row].sText[MAX_TEXT] = '\0';
			/* Write line to Display */
			vLcdWriteText(asRow[u8Row].sText, u8Row, COL_TEXT);
		}
		/* Got a value 0 to Display ? */
		if (asRow[u8Row].u8WidLeft != 0)
		{
			/* Write out value */
			#if LCD_HEX
			LCD_vWriteHex(asRow[u8Row].u32Left, asRow[u8Row].u8WidLeft, u8Row, COL_LEFT);
			#else
			LCD_vWriteDec(asRow[u8Row].u32Left, asRow[u8Row].u8WidLeft, u8Row, COL_LEFT);
			#endif
		}
		/* Got a value 1 to Display ? */
		if (asRow[u8Row].u8WidRight != 0)
		{
			#if LCD_HEX
			/* Write out value */
			LCD_vWriteHex(asRow[u8Row].u32Right, asRow[u8Row].u8WidRight, u8Row, COL_RIGHT);
			#else
			/* Write out value */
			LCD_vWriteDec(asRow[u8Row].u32Right, asRow[u8Row].u8WidRight, u8Row, COL_RIGHT);
			#endif
		}
	}
	/* Refresh LCD */
	vLcdRefreshAll();
}

/****************************************************************************/
/***        Private Functions                                             ***/
/****************************************************************************/

#if LCD_HEX
/****************************************************************************/
/**
 * <b>LCD_vWriteHex</b> &mdash; Write a hex value to the display
 *
 ****************************************************************************/
PRIVATE void LCD_vWriteHex(uint32 u32Value, 	/**< Value to display */
						   uint8   u8Digits, 	/**< Hex digits to display */
						   uint8   u8Row, 		/**< Row to display on */
						   uint8   u8Column)	/**< Column to display at */
{
	uint8  u8Pos;
	char    sHex[7];
	char    sChar[17] = "0123456789ABCDEF";

	/* Sanity check digits */
	if (u8Digits > 6) u8Digits = 6;
	/* Do hex conversion */
	for (u8Pos = 0; u8Pos < u8Digits; u8Pos++)
	{
		sHex[u8Pos] = sChar[(u32Value >> ((u8Digits-u8Pos-1)*4)) &0xF];
		sHex[u8Pos+1] = '\0';
	}
	/* Write out string */
	vLcdWriteText(sHex, u8Row, u8Column);
}

#else
/****************************************************************************/
/**
 * <b>LCD_vWriteDec</b> &mdash; Write a dec value to the display
 *
 ****************************************************************************/
PRIVATE void LCD_vWriteDec(uint32 u32Value, 	/**< Value to display */
						   uint8   u8Digits, 	/**< Hex digits to display */
						   uint8   u8Row, 		/**< Row to display on */
						   uint8   u8Column)	/**< Column to display at */
{
	uint8  u8Pos;
	char    sDec[7];

	/* Sanity check digits */
	if (u8Digits > 6) u8Digits = 6;
	sDec[u8Digits] = '\0';
	/* Do hex conversion */
	for (u8Pos = 0; u8Pos < u8Digits; u8Pos++)
	{
		sDec[u8Digits-u8Pos-1] = (u32Value % 10) + '0';
		u32Value /= 10;
	}
	/* Write out string */
	vLcdWriteText(sDec, u8Row, u8Column);
}
#endif

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
