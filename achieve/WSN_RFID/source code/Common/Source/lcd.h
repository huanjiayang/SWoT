/****************************************************************************/
/*!
 *\MODULE              LCD (Application Layer)
 *
 *\COMPONENT           $HeadURL: https://www.collabnet.nxp.com/svn/lprf_apps/Application_Notes/JN-AN-1134-ZigBee-PRO-Serial-Cable-Replacement/Tags/Release_1v3-Public/Common/Source/lcd.h $
 *
 *\VERSION			   $Revision: 5797 $
 *
 *\REVISION            $Id: lcd.h 5797 2010-04-20 12:26:04Z mlook $
 *
 *\DATED               $Date: 2010-04-20 13:26:04 +0100 (Tue, 20 Apr 2010) $
 *
 *\AUTHOR              $Author: mlook $
 *
 *\DESCRIPTION         LCD (Application Layer) - public interface.
 *
 * This module contains general-purpose routines to maintain a single page
 * on the LCD screen.
 *
 * Each line can have a text string and two numeric values displayed. Functions
 * are provided to set each of these items independantly, and a draw function
 * is provided to draw the page.
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
#ifndef  LCD_H_INCLUDED
#define  LCD_H_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
/* Jennic includes */
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
PUBLIC void LCD_vInit(void);
PUBLIC void LCD_vText(uint8, char *);
PUBLIC void LCD_vLeft(uint8, uint32, uint8);
PUBLIC void LCD_vRight(uint8, uint32, uint8);
PUBLIC void LCD_vDraw(void);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* LCD_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/


