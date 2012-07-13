/*****************************************************************************
 *
 * MODULE:             Home Sensor Demo Application
 *
 * COMPONENT:          app_sample.h
 *
 * AUTHOR:             MRW
 *
 * DESCRIPTION:        Sample the sensors on a sensor node and send the values
 *                     to the controller node
 *
 * $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Application_Notes/JN-AN-1122-ZBPro-Home-Sensor-Demo/Tags/ZBPRO-HOME-SENSOR-DEMO_1v7_RC4/AN1122_ZBP_SensorNode_SED/Source/app_sample_SED.h $
 *
 * $Revision: 15615 $
 *
 * $LastChangedBy: mwild $
 *
 * $LastChangedDate: 2009-07-02 11:32:33 +0100 (Thu, 02 Jul 2009) $
 *
 * $Id: app_sample_SED.h 15615 2009-07-02 10:32:33Z mwild $
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

#ifndef APP_SAMPLE_H_
#define APP_SAMPLE_H_

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define SAMPLE_TIME     APP_TIME_MS(1000)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void APP_vInitialiseSample(void);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_SAMPLE_H_*/
