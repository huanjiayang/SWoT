/*****************************************************************************
 *
 * MODULE:             Home Sensor Demo
 *
 * COMPONENT:          app_buttons.h
 *
 * AUTHOR:             MRW
 *
 * DESCRIPTION:        Monitor the state of the buttons on the DK2 development
 *                     board, ensuring they are debounced.
 *
 * $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Application_Notes/JN-AN-1122-ZBPro-Home-Sensor-Demo/Tags/ZBPRO-HOME-SENSOR-DEMO_1v7_RC4/AN1122_ZBP_ControllerNode/Source/app_buttons.h $
 *
 * $Revision: 15615 $
 *
 * $LastChangedBy: mwild $
 *
 * $LastChangedDate: 2009-07-02 11:32:33 +0100 (Thu, 02 Jul 2009) $
 *
 * $Id: app_buttons.h 15615 2009-07-02 10:32:33Z mwild $
 *
 *****************************************************************************
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
 * Copyright Jennic Ltd. 2009 All rights reserved
 *
 ****************************************************************************/

#ifndef APP_BUTTONS_H
#define APP_BUTTONS_H

#define APP_CONTROLLER

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define APP_BUTTON_DECLARE(dio) \
    APP_tsButtonDescriptor app_sButton_##name = { NULL, dio, 0 };

#define APP_BUTTONS_BUTTON_1        (9)
#define APP_BUTTONS_BUTTON_2        (10)
#define APP_BUTTONS_BUTTON_3        (11)
#define APP_BUTTONS_BUTTON_4        (20)

#ifdef APP_CONTROLLER
#define APP_BUTTONS_NUM             (4UL)
#define APP_BUTTONS_DIO_MASK ( \
    (1 << APP_BUTTONS_BUTTON_1) | \
    (1 << APP_BUTTONS_BUTTON_2) | \
    (1 << APP_BUTTONS_BUTTON_3) | \
    (1 << APP_BUTTONS_BUTTON_4) )
#else
#define APP_BUTTONS_NUM             (2UL)
#define APP_BUTTONS_DIO_MASK ( \
    (1 << APP_BUTTONS_BUTTON_1) | \
    (1 << APP_BUTTONS_BUTTON_2))
#endif

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum
{
    APP_E_BUTTONS_BUTTON_1 = 0,
    APP_E_BUTTONS_BUTTON_2,
    APP_E_BUTTONS_BUTTON_3,
    APP_E_BUTTONS_BUTTON_4
} APP_teButtons;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC bool_t APP_bButtonInitialise(void);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_BUTTONS_H*/
