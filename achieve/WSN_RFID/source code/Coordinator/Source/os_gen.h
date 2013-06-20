/****************************************************************************
 *
 *                 THIS IS A GENERATED FILE. DO NOT EDIT!
 *
 * MODULE:         OS
 *
 * COMPONENT:      os_gen.h
 *
 * DATE:           Fri May 18 04:19:20 2012
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
#define mutexPdmMedia ((OS_thMutex)&os_Mutex_mutexPdmMedia)

/* Message Handles */
#define zps_msgMlmeDcfmInd ((OS_thMessage)&os_Message_zps_msgMlmeDcfmInd)
#define zps_msgTimeEvents ((OS_thMessage)&os_Message_zps_msgTimeEvents)
#define zps_msgMcpsDcfmInd ((OS_thMessage)&os_Message_zps_msgMcpsDcfmInd)

/* Module ZBPro_WUART_Coordinator */

/* Task Handles */
#define CRD_ZpsEventsTask ((OS_thTask)&os_Task_CRD_ZpsEventsTask)
#define CRD_TickTimerTask ((OS_thTask)&os_Task_CRD_TickTimerTask)
#define CRD_AppEpEventsTask ((OS_thTask)&os_Task_CRD_AppEpEventsTask)

/* Message Handles */
#define APP_msgZpsEvents ((OS_thMessage)&os_Message_APP_msgZpsEvents)
#define APP_EP_Events ((OS_thMessage)&os_Message_APP_EP_Events)

/* Timer Handles */
#define TickCounter ((OS_thHWCounter)&os_HWCounter_TickCounter)
#define CRD_ZpsEventsTimer ((OS_thSWTimer)&os_SWTimer_TickCounter_CRD_ZpsEventsTimer)
#define CRD_TickTimer ((OS_thSWTimer)&os_SWTimer_TickCounter_CRD_TickTimer)

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
PUBLIC OS_thMutex os_Mutex_mutexPdmMedia;

/* Message Handles */
PUBLIC OS_thMessage os_Message_zps_msgMlmeDcfmInd;
PUBLIC OS_thMessage os_Message_zps_msgTimeEvents;
PUBLIC OS_thMessage os_Message_zps_msgMcpsDcfmInd;

/* Task Handles */
PUBLIC OS_thTask os_Task_CRD_ZpsEventsTask;
PUBLIC OS_thTask os_Task_CRD_TickTimerTask;
PUBLIC OS_thTask os_Task_CRD_AppEpEventsTask;

/* Message Handles */
PUBLIC OS_thMessage os_Message_APP_msgZpsEvents;
PUBLIC OS_thMessage os_Message_APP_EP_Events;

/* Timer Handles */
PUBLIC OS_thHWCounter os_HWCounter_TickCounter;
PUBLIC OS_thSWTimer os_SWTimer_TickCounter_CRD_ZpsEventsTimer;
PUBLIC OS_thSWTimer os_SWTimer_TickCounter_CRD_TickTimer;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/


PUBLIC bool os_bTICK_bSet(uint32 );
PUBLIC uint32 os_u32TICK_u32Get(void);
PUBLIC void os_vTICK_vEnable(void);
PUBLIC void os_vTICK_vDisable(void);

#endif
