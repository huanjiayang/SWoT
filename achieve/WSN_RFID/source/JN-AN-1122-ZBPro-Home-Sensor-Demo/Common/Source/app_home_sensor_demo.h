/*****************************************************************************
 *
 * MODULE:             Home Sensor Demo Application
 *
 * COMPONENT:          app_home_sensor_demo.h
 *
 * AUTHOR:             MRW
 *
 * DESCRIPTION:        Global definitions for the home sensor demo application
 *
 * $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Application_Notes/JN-AN-1122-ZBPro-Home-Sensor-Demo/Tags/ZBPRO-HOME-SENSOR-DEMO_1v7_RC4/Common/Source/app_home_sensor_demo.h $
 *
 * $Revision: 15615 $
 *
 * $LastChangedBy: mwild $
 *
 * $LastChangedDate: 2009-07-02 11:32:33 +0100 (Thu, 02 Jul 2009) $
 *
 * $Id: app_home_sensor_demo.h 15615 2009-07-02 10:32:33Z mwild $
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

#ifndef APP_HOME_SENSOR_DEMO_H_
#define APP_HOME_SENSOR_DEMO_H_

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define APP_HSDEMO_NUM_SENSORNODES                  4
#define APP_HSDEMO_NUM_SENSORNODES_POSITIONS        4
#define APP_HSDEMO_NUM_SENSOR_TYPES                 3

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum
{
    APP_E_SENSOR_TEMP = 0,
    APP_E_SENSOR_HTS,
    APP_E_SENSOR_ALS,
} APP_teSensor;

typedef enum
{
    APP_E_EVENT_NONE = 0,
    APP_E_EVENT_BUTTON_UP,
    APP_E_EVENT_BUTTON_DOWN
} APP_teEventType;

typedef struct
{
    uint8 u8Button;
} APP_teEventButton;

typedef struct
{
    APP_teEventType eType;
    union
    {
        APP_teEventButton sButton;
    };
} APP_tsEvent;

typedef struct
{
    uint8 u8Length;
    uint64 u64ExtAddr;
    uint8 u8LightValue;
    uint8 u8TempValue;
    uint8 u8HumidityValue;
    uint8 u8Switch;
} APP_tsSensorData;

#define APDU_SENSORDATA_LENGTH      0
#define APDU_SENSORDATA_ADDR        1
#define APDU_SENSORDATA_LIGHT       9
#define APDU_SENSORDATA_TEMP        10
#define APDU_SENSORDATA_HUMIDITY    11
#define APDU_SENSORDATA_SWITCH      12

#define APDU_LEDCTRL_LENGTH     0
#define APDU_LEDCTRL_ADDR       1
#define APDU_LEDCTRL_STATE      9

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_HOME_SENSOR_DEMO_H_*/
