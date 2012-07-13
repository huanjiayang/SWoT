/****************************************************************************
 *
 *                 THIS IS A GENERATED FILE. DO NOT EDIT!
 *
 * MODULE:         OS
 *
 * COMPONENT:      os_gen.c
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

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include <os.h>
#include "os_msg_types.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define OS_TASK_MAGIC                   0xdeadbeef
#define OS_MUTEX_MAGIC                  0xcafebabe
#define OS_MESSAGE_MAGIC                0x00ddba11
#define OS_HWCOUNTER_MAGIC              0x1A2B3C4D
#define OS_SWTIMER_MAGIC                0x4B3C2B1A

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef uint8  tIPL;
typedef uint8  tPriority;
#ifdef OS_64_TASKS
typedef uint64 tIPLMask;
typedef uint64 tPriorityMask;
#else
typedef uint32 tIPLMask;
typedef uint32 tPriorityMask;
#endif

typedef struct {
    uint32 u32Magic;
    tPriority u8BasePriority;
    tPriority u8DispatchPriority;
    uint16 u16ActivationCount;
    void (*pfvTaskFunction)(void);
} tsTask;

typedef struct {
    uint32 u32Magic;
    tPriority u8CeilingPriority;
    tIPL u8IPL;
    tPriority u8PrevPriority;
    tIPL u8PrevIPL;
} tsMutex;

typedef struct {
    uint32 u32Magic;
    void *pvData;
    uint32 u32In;
    uint32 u32Out;
    uint32 u32NumItems;
    uint32 u32Size;
    uint32 u32ElementSize;
    tsTask *psNotifyTask;
    tPriority u8CeilingPriority;
    tIPL u8IPL;
} tsMessage;

struct _tsHWCounter;
struct _tsSWTimer;
typedef struct _tsHWCounter tsHWCounter;
typedef struct _tsSWTimer tsSWTimer;

struct _tsHWCounter {
    uint32 u32Magic;
    tsSWTimer *psList; /* must be 2nd element of structure */
    bool (*pfvSetCallback)(uint32 u32Time);
    uint32 (*pfu32GetCallback)(void);
    void (*pfvEnableCallback)(void);
    void (*pfvDisableCallback)(void);
};

struct _tsSWTimer {
    uint32 u32Magic;
    tsSWTimer *psNext;  /* must be 2nd element of structure */
    tsHWCounter *psHWCounter;
    void *pvData;
    void *pvAction;
    uint32 u32MatchTime;
    uint8 eStatus;
    uint8 bCallback;
};

/****************************************************************************/
/***        Function Prototypes                                           ***/
/****************************************************************************/

/* Module ZBPro */
PUBLIC void os_vzps_taskZPS(void);
/* Module ZBPro_Home_Sensor_Demo */
PUBLIC void os_vAPP_taskSampleSensors(void);
PUBLIC void os_vAPP_taskSensorNode(void);
PUBLIC void os_vAPP_taskScanButtons(void);
PUBLIC void os_vAPP_taskLedControl(void);
PUBLIC void os_vAPP_taskSensorEvent(void);

PUBLIC bool os_bAPP_cbSetTickTimerCompare(uint32 );
PUBLIC uint32 os_u32APP_cbGetTickTimer(void);
PUBLIC void os_vAPP_cbEnableTickTimer(void);
PUBLIC void os_vAPP_cbDisableTickTimer(void);
/* Module Exceptions */

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/


/* Tasks */
tsTask os_Task_zps_taskZPS = { OS_TASK_MAGIC, 2, 2, 0, os_vzps_taskZPS };
tsTask os_Task_APP_taskSampleSensors = { OS_TASK_MAGIC, 4, 4, 0, os_vAPP_taskSampleSensors };
tsTask os_Task_APP_taskSensorNode = { OS_TASK_MAGIC, 3, 3, 1, os_vAPP_taskSensorNode };
tsTask os_Task_APP_taskScanButtons = { OS_TASK_MAGIC, 6, 6, 0, os_vAPP_taskScanButtons };
tsTask os_Task_APP_taskLedControl = { OS_TASK_MAGIC, 5, 5, 0, os_vAPP_taskLedControl };
tsTask os_Task_APP_taskSensorEvent = { OS_TASK_MAGIC, 1, 1, 0, os_vAPP_taskSensorEvent };

/* Mutexs */
tsMutex os_Mutex_mutexZPS = { OS_MUTEX_MAGIC, 6, 0, 0xff, 0xff };
tsMutex os_Mutex_mutexPDUM = { OS_MUTEX_MAGIC, 6, 0, 0xff, 0xff };
tsMutex os_Mutex_mutexMAC = { OS_MUTEX_MAGIC, 6, 7, 0xff, 0xff };
tsMutex os_Mutex_mutexMEDIA = { OS_MUTEX_MAGIC, 4, 0, 0xff, 0xff };

/* Messages */
PRIVATE MAC_MlmeDcfmInd_s s_aMessageData_zps_msgMlmeDcfmInd[9];
tsMessage os_Message_zps_msgMlmeDcfmInd = {
    OS_MESSAGE_MAGIC,
    s_aMessageData_zps_msgMlmeDcfmInd,
    0,
    0,
    0,
    9,
    sizeof(MAC_MlmeDcfmInd_s),
    &os_Task_zps_taskZPS,
    0,
    7
};

PRIVATE zps_tsTimeEvent s_aMessageData_zps_msgTimeEvents[8];
tsMessage os_Message_zps_msgTimeEvents = {
    OS_MESSAGE_MAGIC,
    s_aMessageData_zps_msgTimeEvents,
    0,
    0,
    0,
    8,
    sizeof(zps_tsTimeEvent),
    &os_Task_zps_taskZPS,
    0,
    7
};

PRIVATE MAC_tsMcpsVsDcfmInd s_aMessageData_zps_msgMcpsDcfmInd[8];
tsMessage os_Message_zps_msgMcpsDcfmInd = {
    OS_MESSAGE_MAGIC,
    s_aMessageData_zps_msgMcpsDcfmInd,
    0,
    0,
    0,
    8,
    sizeof(MAC_tsMcpsVsDcfmInd),
    &os_Task_zps_taskZPS,
    0,
    7
};


/* Messages */
PRIVATE ZPS_tsAfEvent s_aMessageData_APP_msgZpsEvents[1];
tsMessage os_Message_APP_msgZpsEvents = {
    OS_MESSAGE_MAGIC,
    s_aMessageData_APP_msgZpsEvents,
    0,
    0,
    0,
    1,
    sizeof(ZPS_tsAfEvent),
    &os_Task_APP_taskSensorNode,
    2,
    0
};

PRIVATE ZPS_tsAfEvent s_aMessageData_APP_msgLedEvent[1];
tsMessage os_Message_APP_msgLedEvent = {
    OS_MESSAGE_MAGIC,
    s_aMessageData_APP_msgLedEvent,
    0,
    0,
    0,
    1,
    sizeof(ZPS_tsAfEvent),
    &os_Task_APP_taskLedControl,
    2,
    0
};

PRIVATE ZPS_tsAfEvent s_aMessageData_APP_msgSensorEvent[1];
tsMessage os_Message_APP_msgSensorEvent = {
    OS_MESSAGE_MAGIC,
    s_aMessageData_APP_msgSensorEvent,
    0,
    0,
    0,
    1,
    sizeof(ZPS_tsAfEvent),
    &os_Task_APP_taskSensorEvent,
    2,
    0
};



/* Timers connected to 'APP_cntrTickTimer' */
tsHWCounter os_HWCounter_APP_cntrTickTimer = { OS_HWCOUNTER_MAGIC, NULL, os_bAPP_cbSetTickTimerCompare, os_u32APP_cbGetTickTimer, os_vAPP_cbEnableTickTimer, os_vAPP_cbDisableTickTimer };
tsSWTimer os_SWTimer_APP_cntrTickTimer_APP_tmrSampleSensors = { OS_SWTIMER_MAGIC, NULL, &os_HWCounter_APP_cntrTickTimer, NULL, &os_Task_APP_taskSampleSensors, 0, OS_E_SWTIMER_STOPPED, FALSE };
tsSWTimer os_SWTimer_APP_cntrTickTimer_APP_tmrRestart = { OS_SWTIMER_MAGIC, NULL, &os_HWCounter_APP_cntrTickTimer, NULL, &os_Task_APP_taskSensorNode, 0, OS_E_SWTIMER_STOPPED, FALSE };
tsSWTimer os_SWTimer_APP_cntrTickTimer_APP_tmrButtonsScan = { OS_SWTIMER_MAGIC, NULL, &os_HWCounter_APP_cntrTickTimer, NULL, &os_Task_APP_taskScanButtons, 0, OS_E_SWTIMER_STOPPED, FALSE };

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/* Task Table */
PRIVATE tsTask *s_asTasks[] = {
    &os_Task_APP_taskSensorEvent,
    &os_Task_zps_taskZPS,
    &os_Task_APP_taskSensorNode,
    &os_Task_APP_taskSampleSensors,
    &os_Task_APP_taskLedControl,
    &os_Task_APP_taskScanButtons,
};

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

extern void os_vStart(
    void (*)(void),
    void (*)(void),
    tPriorityMask ,
    tsTask **,
    uint8,
    uint32 *[9],
    uint8 *);
extern uint32 *os_OSMIUM_HwVectTable[9];
extern uint8 os_PIC_ChannelPriorities[16];

extern void os_vRestart(uint32 *[9], uint8 *);
extern void os_vPatchInit(void);

PUBLIC void OS_vStart(void (*prvInitFunction)(void), void (*prvUnclaimedISR)(void))
{
    /* Version compatibility check */
    asm(".extern OS_Version_1v1" : ) ;
    asm("b.addi r0,r0,OS_Version_1v1" : );
    os_vPatchInit();
    os_vStart(prvInitFunction, prvUnclaimedISR, 0x20000000, s_asTasks, 14, os_OSMIUM_HwVectTable, os_PIC_ChannelPriorities);
}

PUBLIC void OS_vRestart(void)
{
    os_vRestart(os_OSMIUM_HwVectTable, os_PIC_ChannelPriorities);
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
