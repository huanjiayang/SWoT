/*****************************************************************************
 *
 * MODULE:             Home Sensor Demo Application
 *
 * COMPONENT:          app_log.h
 *
 * AUTHOR:             MRW
 *
 * DESCRIPTION:        Log sensor data received from remote wireless sensors
 *
 * $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Application_Notes/JN-AN-1122-ZBPro-Home-Sensor-Demo/Tags/ZBPRO-HOME-SENSOR-DEMO_1v7_RC4/AN1122_ZBP_ControllerNode/Source/app_log.h $
 *
 * $Revision: 27476 $
 *
 * $LastChangedBy: fbhai $
 *
 * $LastChangedDate: 2010-11-08 11:43:26 +0000 (Mon, 08 Nov 2010) $
 *
 * $Id: app_log.h 27476 2010-11-08 11:43:26Z fbhai $
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
#ifndef APP_LOG_H_
#define APP_LOG_H_

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/* length of log history */
#define APP_LOG_HISTORY_LEN     (16UL)
#define NOT_FOUND               0xff

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/* Holds all stored data for a particular sensor for a node */
typedef struct
{
    uint8 u8NowValue;
    uint8 au8GraphData[APP_LOG_HISTORY_LEN];
} APP_tsLogSensorData;

/* Holds all stored data for a node */
typedef struct
{
    uint16 u16ShortAddr;
    uint8 u8FramesMissed;
    uint8 u8SwitchOn;
    APP_tsLogSensorData asNodeSensorData[APP_HSDEMO_NUM_SENSOR_TYPES];
} APP_tsLogNodeHistory;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void APP_vLogInitialise(void);
PUBLIC void APP_vLogStart(void);
PUBLIC APP_tsLogNodeHistory *APP_psLogGetSensorNodeHistory(uint8 u8SensorNode);
PUBLIC uint8 APP_u8LogGetDataStartPos(void);
PUBLIC uint8 APP_u8GetSensorNodeId(uint16 u16ShortAddr, uint64 u64IeeeAddress);
PUBLIC uint16 APP_u16GetSensorNodeAddr(uint8 u8Node);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_LOG_H_*/
