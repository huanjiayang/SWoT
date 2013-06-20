/*****************************************************************************
 *
 * MODULE:             Home Sensor Demo Application
 *
 * COMPONENT:          app_controller_node.h
 *
 * AUTHOR:             MRW
 *
 * DESCRIPTION:        Global definitions for the home sensor demo application
 *
 * $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Application_Notes/JN-AN-1122-ZBPro-Home-Sensor-Demo/Tags/ZBPRO-HOME-SENSOR-DEMO_1v7_RC4/AN1122_ZBP_ControllerNode/Source/app_controller_node.h $
 *
 * $Revision: 22328 $
 *
 * $LastChangedBy: mwild $
 *
 * $LastChangedDate: 2009-11-23 18:30:12 +0000 (Mon, 23 Nov 2009) $
 *
 * $Id: app_controller_node.h 22328 2009-11-23 18:30:12Z mwild $
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

#ifndef APP_COORD_H_
#define APP_COORD_H_

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void APP_vInitialiseControllerNode(void);
PUBLIC uint8 APP_u8ControllerNodeGetCurRadioChan(void);
PUBLIC uint8 APP_u8ControllerNodeNumAssociatedSensors(void);
PUBLIC uint8 APP_u8ControllerNodeNumDataSensors(void);
PUBLIC bool_t APP_bGetPermitJoining(void);
PUBLIC void APP_vSetPermitJoining(bool_t bPermit);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

extern bool_t APP_bAppHealthy;

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_COORD_H_*/
