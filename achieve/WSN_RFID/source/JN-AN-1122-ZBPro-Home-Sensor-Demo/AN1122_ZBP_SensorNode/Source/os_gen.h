/****************************************************************************
 *
 *                 THIS IS A GENERATED FILE. DO NOT EDIT!
 *
 * MODULE:         OS
 *
 * COMPONENT:      os_gen.h
 *
 * DATE:           Thu Jul 12 11:01:22 2012
 *
 * AUTHOR:         Jennic RTOS Configuration Tool
 *
 * DESCRIPTION:    RTOS Application Configuration
 *
 ****************************************************************************
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

#ifndef _OS_GEN_H
#define _OS_GEN_H

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/* Module ZBPro */

/* Task Handles */
#define zps_taskZPS ((OS_thTask)&os_Task_zps_taskZPS)

/* Mutex Handles */
#define mutexZPS ((OS_thMutex)&os_Mutex_mutexZPS)
#define mutexPDUM ((OS_thMutex)&os_Mutex_mutexPDUM)
#define mutexMAC ((OS_thMutex)&os_Mutex_mutexMAC)
#define mutexMEDIA ((OS_thMutex)&os_Mutex_mutexMEDIA)

/* Message Handles */
#define zps_msgMlmeDcfmInd ((OS_thMessage)&os_Message_zps_msgMlmeDcfmInd)
#define zps_msgTimeEvents ((OS_thMessage)&os_Message_zps_msgTimeEvents)
#define zps_msgMcpsDcfmInd ((OS_thMessage)&os_Message_zps_msgMcpsDcfmInd)

/* Module ZBPro_Home_Sensor_Demo */

/* Task Handles */
#define APP_taskSampleSensors ((OS_thTask)&os_Task_APP_taskSampleSensors)
#define APP_taskSensorNode ((OS_thTask)&os_Task_APP_taskSensorNode)
#define APP_taskScanButtons ((OS_thTask)&os_Task_APP_taskScanButtons)
#define APP_taskLedControl ((OS_thTask)&os_Task_APP_taskLedControl)
#define APP_taskSensorEvent ((OS_thTask)&os_Task_APP_taskSensorEvent)

/* Message Handles */
#define APP_msgZpsEvents ((OS_thMessage)&os_Message_APP_msgZpsEvents)
#define APP_msgLedEvent ((OS_thMessage)&os_Message_APP_msgLedEvent)
#define APP_msgSensorEvent ((OS_thMessage)&os_Message_APP_msgSensorEvent)

/* Timer Handles */
#define APP_cntrTickTimer ((OS_thHWCounter)&os_HWCounter_APP_cntrTickTimer)
#define APP_tmrSampleSensors ((OS_thSWTimer)&os_SWTimer_APP_cntrTickTimer_APP_tmrSampleSensors)
#define APP_tmrRestart ((OS_thSWTimer)&os_SWTimer_APP_cntrTickTimer_APP_tmrRestart)
#define APP_tmrButtonsScan ((OS_thSWTimer)&os_SWTimer_APP_cntrTickTimer_APP_tmrButtonsScan)

/* Module Exceptions */

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/


/* Task Handles */
PUBLIC OS_thTask os_Task_zps_taskZPS;

/* Mutex Handles */
PUBLIC OS_thMutex os_Mutex_mutexZPS;
PUBLIC OS_thMutex os_Mutex_mutexPDUM;
PUBLIC OS_thMutex os_Mutex_mutexMAC;
PUBLIC OS_thMutex os_Mutex_mutexMEDIA;

/* Message Handles */
PUBLIC OS_thMessage os_Message_zps_msgMlmeDcfmInd;
PUBLIC OS_thMessage os_Message_zps_msgTimeEvents;
PUBLIC OS_thMessage os_Message_zps_msgMcpsDcfmInd;

/* Task Handles */
PUBLIC OS_thTask os_Task_APP_taskSampleSensors;
PUBLIC OS_thTask os_Task_APP_taskSensorNode;
PUBLIC OS_thTask os_Task_APP_taskScanButtons;
PUBLIC OS_thTask os_Task_APP_taskLedControl;
PUBLIC OS_thTask os_Task_APP_taskSensorEvent;

/* Message Handles */
PUBLIC OS_thMessage os_Message_APP_msgZpsEvents;
PUBLIC OS_thMessage os_Message_APP_msgLedEvent;
PUBLIC OS_thMessage os_Message_APP_msgSensorEvent;

/* Timer Handles */
PUBLIC OS_thHWCounter os_HWCounter_APP_cntrTickTimer;
PUBLIC OS_thSWTimer os_SWTimer_APP_cntrTickTimer_APP_tmrSampleSensors;
PUBLIC OS_thSWTimer os_SWTimer_APP_cntrTickTimer_APP_tmrRestart;
PUBLIC OS_thSWTimer os_SWTimer_APP_cntrTickTimer_APP_tmrButtonsScan;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/


PUBLIC bool os_bAPP_cbSetTickTimerCompare(uint32 );
PUBLIC uint32 os_u32APP_cbGetTickTimer(void);
PUBLIC void os_vAPP_cbEnableTickTimer(void);
PUBLIC void os_vAPP_cbDisableTickTimer(void);

#endif
