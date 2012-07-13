/*****************************************************************************
 *
 * MODULE:             Home Sensor Demo
 *
 * COMPONENT:          app_log.c
 *
 * AUTHOR:             MRW
 *
 * DESCRIPTION:        Log sensor data received from remote wireless sensors
 *
 * $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Application_Notes/JN-AN-1122-ZBPro-Home-Sensor-Demo/Tags/ZBPRO-HOME-SENSOR-DEMO_1v7_RC4/AN1122_ZBP_ControllerNode/Source/app_log.c $
 *
 * $Revision: 27712 $
 *
 * $LastChangedBy: fbhai $
 *
 * $LastChangedDate: 2010-11-24 17:02:20 +0000 (Wed, 24 Nov 2010) $
 *
 * $Id: app_log.c 27712 2010-11-24 17:02:20Z fbhai $
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

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "os.h"
#include "os_gen.h"
#include "pdum_nwk.h"
#include "pdum_apl.h"
#include "pdum_gen.h"
#include "dbg.h"
#include "pdm.h"
#include "zps_gen.h"
#include "zps_apl_af.h"
#include "app_home_sensor_demo.h"
#include "app_display.h"
#include "app_timer_driver.h"
#include "app_controller_node.h"
#include "app_log.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifndef TRACE_APP_LOG
#define TRACE_APP_LOG   FALSE
#endif

#define LOG_TIME    APP_TIME_MS(1000)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct
{
    uint16 u16ShortAddr;
    uint64 u64IeeeAddress;
} tsDataNode;

typedef struct
{
    uint8 s_u8NumDataSensors;
    tsDataNode s_asDataNodeAddr[APP_HSDEMO_NUM_SENSORNODES];
} tsLogNodes;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PUBLIC PDM_tsRecordDescriptor s_sLogPDDesc;

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE APP_tsLogNodeHistory s_asNodeHistoryData[APP_HSDEMO_NUM_SENSORNODES];
PRIVATE uint8 s_u8CurrentTime = 0;

/* This structure is used to contain any data which the module requires to
 * persist in flash.
 */
PRIVATE tsLogNodes s_sLogNodes;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: APP_vLogInitialise
 *
 * DESCRIPTION:
 * Initialises the logging module
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vLogInitialise(void)
{
    /* Initialise device counter */
    s_sLogNodes.s_u8NumDataSensors = 0;

    /* Restore any device list previously saved to flash */
    PDM_eLoadRecord(&s_sLogPDDesc, "APP_LOG", &s_sLogNodes, sizeof(s_sLogNodes),FALSE);
}

/****************************************************************************
 *
 * NAME: APP_vLogStart
 *
 * DESCRIPTION:
 * Starts logging sensor data
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vLogStart(void)
{
    OS_eStartSWTimer(APP_tmrLogData, LOG_TIME, NULL);
}

/****************************************************************************
 *
 * NAME: APP_tsLogNodeHistory
 *
 * DESCRIPTION:
 *
 * PARAMETERS:      Name            RW  Usage
 *                  u8SensorNode    R   Handle of the PDU to be sent
 *
 *
 * RETURNS:
 * Current channel number of radio
 *
 ****************************************************************************/
PUBLIC  APP_tsLogNodeHistory *APP_psLogGetSensorNodeHistory(uint8 u8SensorNode)
{
    return &s_asNodeHistoryData[u8SensorNode];
}

/****************************************************************************
 *
 * NAME: APP_u8LogGetDataStartPos
 *
 * DESCRIPTION:
 *
 *
 * RETURNS:
 * Start of logged data
 *
 ****************************************************************************/
PUBLIC uint8 APP_u8LogGetDataStartPos(void)
{
    /*
     *  s_u8CurrentTime will always be 1 place in front of last logged data
     * this position will be the oldest logged data and therfore the start
     * position for the graph data
     */
    return s_u8CurrentTime;
}

/****************************************************************************
 *
 * NAME: APP_GetSensorNodeId
 *
 * DESCRIPTION:
 * Returns the node id given a short address
 *
 * RETURNS:
 * Node Id
 *
 ****************************************************************************/
PUBLIC uint8 APP_u8GetSensorNodeId(uint16 u16ShortAddr, uint64 u64ExtendedAddress)
{
    uint8 n;

    for (n = 0; n < APP_u8ControllerNodeNumDataSensors(); n++)
    {
        if (s_sLogNodes.s_asDataNodeAddr[n].u16ShortAddr == u16ShortAddr)
        {
            return n;
        }
        else if(s_sLogNodes.s_asDataNodeAddr[n].u64IeeeAddress == u64ExtendedAddress && u64ExtendedAddress != 0)
        {
            /* Device has rejoined with a different address */
            s_sLogNodes.s_asDataNodeAddr[n].u16ShortAddr = u16ShortAddr;
            return n;
        }
    }

    return NOT_FOUND;
}

/****************************************************************************
 *
 * NAME: APP_u16GetSensorNodeAddr
 *
 * DESCRIPTION:
 * Returns the network address for a given node id
 *
 * PARAMETERS:      Name      RW  Usage
 *                  u8Node    R   ID of the node whose address is required
 *
 * RETURNS:
 * Node Id
 *
 ****************************************************************************/
PUBLIC uint16 APP_u16GetSensorNodeAddr(uint8 u8Node)
{
    return s_sLogNodes.s_asDataNodeAddr[u8Node].u16ShortAddr;
}

/****************************************************************************
 *
 * NAME: APP_u8ControllerNodeNumDataSensors
 *
 * DESCRIPTION:
 * Returns the number of sensors that are in data comm. with the coordinator
 *
 * RETURNS:
 * Current number of data sensor nodes
 *
 ****************************************************************************/
PUBLIC uint8 APP_u8ControllerNodeNumDataSensors(void)
{
    return s_sLogNodes.s_u8NumDataSensors;
}

/****************************************************************************
 *
 * NAME: APP_LogDataTask
 *
 * DESCRIPTION:
 *
 *
 * RETURNS:
 * Current channel number of radio
 *
 ****************************************************************************/
OS_TASK(APP_taskLogData)
{
    ZPS_tsAfEvent sHSDEndPointEv;
    sHSDEndPointEv.eType = ZPS_EVENT_NONE;

    if (OS_E_OK == OS_eCollectMessage(APP_msgSensorEvent, &sHSDEndPointEv))
    {
        if (ZPS_EVENT_APS_DATA_INDICATION == sHSDEndPointEv.eType)
        {
            APP_tsSensorData sSensorData;

            /* basic check on correct app functionality - if controller does not regularly receive
             * sensor data frames then something must be wrong (assume that there are some sensors)
             * APP_bAppHealthy is a flag which is used in app_start to kick the watchdog.
             */
            APP_bAppHealthy =  TRUE;

            if (0 != PDUM_u16APduInstanceReadNBO(sHSDEndPointEv.uEvent.sApsDataIndEvent.hAPduInst,
                                                   0,
                                                   "blbbbb",
                                                   &sSensorData))
            {
                DBG_vPrintf(TRACE_APP_LOG, "len = %d, mac = %016llx, light = %x, temp = %x, humid = %x, switch = %x\n",
                        sSensorData.u8Length,
                        sSensorData.u64ExtAddr,
                        sSensorData.u8LightValue,
                        sSensorData.u8TempValue,
                        sSensorData.u8HumidityValue,
                        sSensorData.u8Switch);

                uint8 u8Node = APP_u8GetSensorNodeId(sHSDEndPointEv.uEvent.sApsDataIndEvent.uSrcAddress.u16Addr, sSensorData.u64ExtAddr);
                if (NOT_FOUND == u8Node)
                {
                    if (APP_HSDEMO_NUM_SENSORNODES > s_sLogNodes.s_u8NumDataSensors)
                    {
                        /* add node's address into list of data nodes */
                        s_sLogNodes.s_asDataNodeAddr[s_sLogNodes.s_u8NumDataSensors].u16ShortAddr
                        = sHSDEndPointEv.uEvent.sApsDataIndEvent.uSrcAddress.u16Addr;
                        s_sLogNodes.s_asDataNodeAddr[s_sLogNodes.s_u8NumDataSensors].u64IeeeAddress =
                            sSensorData.u64ExtAddr;
                        u8Node = s_sLogNodes.s_u8NumDataSensors++;

                        if (APP_HSDEMO_NUM_SENSORNODES == s_sLogNodes.s_u8NumDataSensors)
                        {
                            /* Reached max number of sensor nodes so turn off permit joining.
                             * Note that additional sensor nodes may still join through child routers,
                             * however frames from them will be ignored.
                             */
                            APP_vSetPermitJoining(FALSE);
                        }

                        PDM_vSaveRecord(&s_sLogPDDesc);
                        DBG_vPrintf(TRACE_APP_LOG, "Num data sensors = %d\n", s_sLogNodes.s_u8NumDataSensors);
                    }
                }

                DBG_vPrintf(TRACE_APP_LOG, "APP - Data rxed node = %d, addr = %04x\n", u8Node, sHSDEndPointEv.uEvent.sApsDataIndEvent.uSrcAddress.u16Addr);
                s_asNodeHistoryData[u8Node].asNodeSensorData[APP_E_SENSOR_TEMP].u8NowValue
                = sSensorData.u8TempValue;
                s_asNodeHistoryData[u8Node].asNodeSensorData[APP_E_SENSOR_HTS].u8NowValue
                = sSensorData.u8HumidityValue;
                s_asNodeHistoryData[u8Node].asNodeSensorData[APP_E_SENSOR_ALS].u8NowValue
                = sSensorData.u8LightValue;
                s_asNodeHistoryData[u8Node].u8SwitchOn
                = sSensorData.u8Switch;
                s_asNodeHistoryData[u8Node].u16ShortAddr
                = sHSDEndPointEv.uEvent.sApsDataIndEvent.uSrcAddress.u16Addr;

                /* reset missed frames counter, whcih should count only consecutive missed frames */
                if (s_asNodeHistoryData[u8Node].u8FramesMissed)
                {
                    s_asNodeHistoryData[u8Node].u8FramesMissed = 0;
                }
            }
            else
            {
                DBG_vPrintf(TRACE_APP_LOG, "APP - APDU read failed to read any data.\n");
            }
            PDUM_eAPduFreeAPduInstance(sHSDEndPointEv.uEvent.sApsDataIndEvent.hAPduInst);
        }
        else
        {
            DBG_vPrintf(TRACE_APP_LOG, "Unexpected HSD endpoint event %d\n", sHSDEndPointEv.eType);
        }
    }
    else
    {
        /* must be a timer expiry causing activation */
        PRIVATE uint32 s_u32Count = 0;

        OS_eContinueSWTimer(APP_tmrLogData, LOG_TIME, NULL);
        DBG_vPrintf(TRACE_APP_LOG, "Logging data %d\n", s_u32Count++);

        /* if there are no sensor nodes connected then app is healthy if no data
         * is being received, so ensure the watchdog is kicked */
        if (0 == s_sLogNodes.s_u8NumDataSensors)
        {
            APP_bAppHealthy = TRUE;
        }

        /*
         * Copy the current value into the log and increment the current time
         */
        uint8 u8Node;
        for (u8Node = 0; u8Node < s_sLogNodes.s_u8NumDataSensors; u8Node++)
        {
            s_asNodeHistoryData[u8Node].asNodeSensorData[APP_E_SENSOR_TEMP].au8GraphData[s_u8CurrentTime]
                    = s_asNodeHistoryData[u8Node].asNodeSensorData[APP_E_SENSOR_TEMP].u8NowValue >> 2;
            s_asNodeHistoryData[u8Node].asNodeSensorData[APP_E_SENSOR_HTS].au8GraphData[s_u8CurrentTime]
                    = s_asNodeHistoryData[u8Node].asNodeSensorData[APP_E_SENSOR_HTS].u8NowValue >> 3;
            s_asNodeHistoryData[u8Node].asNodeSensorData[APP_E_SENSOR_ALS].au8GraphData[s_u8CurrentTime]
                    = s_asNodeHistoryData[u8Node].asNodeSensorData[APP_E_SENSOR_ALS].u8NowValue * 2;

            if (s_asNodeHistoryData[u8Node].u8FramesMissed < 0xff)
            {
                s_asNodeHistoryData[u8Node].u8FramesMissed++;
            }
        }

        s_u8CurrentTime++;
        if (APP_LOG_HISTORY_LEN == s_u8CurrentTime)
        {
            s_u8CurrentTime = 0;
        }

        /* update the display to show the newly logged data */
        APP_vDisplayUpdate();
    }
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
