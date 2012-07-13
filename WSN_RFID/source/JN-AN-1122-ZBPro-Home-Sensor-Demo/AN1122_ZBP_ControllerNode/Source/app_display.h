/*****************************************************************************
 *
 * MODULE:             Operating System
 *
 * COMPONENT:          OS.c
 *
 * AUTHOR:             MRW
 *
 * DESCRIPTION:        Provide basic operating system functionality for managing
 *                     tasks, interrupts, etc.
 *
 * $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Application_Notes/JN-AN-1122-ZBPro-Home-Sensor-Demo/Tags/ZBPRO-HOME-SENSOR-DEMO_1v7_RC4/AN1122_ZBP_ControllerNode/Source/app_display.h $
 *
 * $Revision: 22328 $
 *
 * $LastChangedBy: mwild $
 *
 * $LastChangedDate: 2009-11-23 18:30:12 +0000 (Mon, 23 Nov 2009) $
 *
 * $Id: app_display.h 22328 2009-11-23 18:30:12Z mwild $
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

#ifndef APP_DISPLAY_H_
#define APP_DISPLAY_H_

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum
{
    APP_E_DISPLAY_STATE_INIT,
    APP_E_DISPLAY_STATE_SPLASH,
    APP_E_DISPLAY_STATE_NETWORK_STARTUP,
    APP_E_DISPLAY_STATE_NETWORK,
    APP_E_DISPLAY_STATE_NODE
} APP_teDisplayState;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void APP_vDisplayInitialise(void);
PUBLIC void APP_vDisplayCycleNode(void);
PUBLIC void APP_vDisplayUpdate(void);
PUBLIC uint8 APP_u8GetCurrentNode(void);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern APP_teDisplayState app_eDisplayState;
extern APP_teSensor app_eCurrentSensor;

/****************************************************************************/
/***        Inlined Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: APP_vDisplaySetState
 *
 * DESCRIPTION:
 * Sets the display state
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
static inline void APP_vDisplaySetState(APP_teDisplayState eDisplayState)
{
    app_eDisplayState = eDisplayState;
}

/****************************************************************************
 *
 * NAME: APP_vDisplaySetSensor
 *
 * DESCRIPTION:
 * Sets the displayed sensor
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
static inline void APP_vDisplaySetSensor(APP_teSensor eSensor)
{
    app_eCurrentSensor = eSensor;
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_DISPLAY_H_*/
