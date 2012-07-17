/*****************************************************************************
 *
 * MODULE:             Home Sensor Demo
 *
 * COMPONENT:          app_sensor_node.c
 *
 * AUTHOR:             MRW
 *
 * DESCRIPTION:        Sensor node for the home sensor demo
 *
 * $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Application_Notes/JN-AN-1122-ZBPro-Home-Sensor-Demo/Tags/ZBPRO-HOME-SENSOR-DEMO_1v7_RC4/AN1122_ZBP_SensorNode/Source/app_sensor_node.c $
 *
 * $Revision: 29654 $
 *
 * $LastChangedBy: nxp29741 $
 *
 * $LastChangedDate: 2011-03-01 14:54:11 +0000 (Tue, 01 Mar 2011) $
 *
 * $Id: app_sensor_node.c 29654 2011-03-01 14:54:11Z nxp29741 $
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
#include <string.h>
#include "os.h"
#include "os_gen.h"
#include "pdum_apl.h"
#include "pdum_gen.h"
#include "pdm.h"
#include "dbg.h"
#include "zps_gen.h"
#include "zps_nwk_sap.h"
#include "zps_nwk_nib.h"
#include "zps_nwk_pub.h"
#include "zps_apl_af.h"
#include "zps_apl_zdo.h"
#include "zps_apl_aib.h"
#include "zps_apl_zdp.h"
#include "AppHardwareApi.h"
#include "app_led.h"
#include "app_buttons.h"
#include "app_home_sensor_demo.h"
#include "app_sample.h"
#include "app_timer_driver.h"
#include "pwrm.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifndef TRACE_APP
#define TRACE_APP   FALSE
#endif

#define RESTART_TIME    APP_TIME_MS(1000)
#define MAX_SINGLE_CHANNEL_RETRIES      2

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum
{
    E_STARTUP,
    E_NETWORK_DISCOVERY,
    E_NETWORK_JOIN,
    E_MONITOR_SENSORS,
} teState;

typedef struct
{
    teState eState;
} tsDevice;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void vHandleStartupEvent(ZPS_tsAfEvent sStackEvent);
PRIVATE void vHandleNetworkDiscoveryEvent(ZPS_tsAfEvent sStackEvent);
PRIVATE void vHandleNetworkJoinEvent(ZPS_tsAfEvent sStackEvent);
PRIVATE void vHandleMonitorSensorsEvent(ZPS_tsAfEvent sStackEvent);
PUBLIC void vClearDiscNT(void);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
PUBLIC PDM_tsRecordDescriptor s_sDevicePDDesc;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE tsDevice s_sDevice;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: APP_vInitialiseSensorNode
 *
 * DESCRIPTION:
 * Initialises the application
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vInitialiseSensorNode(void)
{
    bool_t bDeleteRecords;

    /* Initialise buttons; if a button is held down as the device is reset, delete the device
     * context from flash
     */
    bDeleteRecords = APP_bButtonInitialise();
    if (bDeleteRecords)
    {

        DBG_vPrintf(TRACE_APP, "APP: Deleting all records from flash\n");
        PDM_vDelete();
    }

    /* Restore any application data previously saved to flash */
    s_sDevice.eState = E_STARTUP;
    PDM_eLoadRecord(&s_sDevicePDDesc,
                    "APP_SENSOR",
                    &s_sDevice,
                    sizeof(s_sDevice),
                    FALSE);

    /* Initialise ZBPro stack */
    DBG_vPrintf(TRACE_APP, "APP: Call Af Init\r\n");
    ZPS_eAplAfInit();

    /* Always initialise any peripherals used by the app */
    APP_vInitialiseSample();
    APP_vLedsInitialise();

    /* If the device state has been restored from flash, re-start the stack
     *  and set the application running again */
    if (E_MONITOR_SENSORS == s_sDevice.eState)
    {
        ZPS_teStatus eStatus = ZPS_eAplZdoStartStack();
        DBG_vPrintf(TRACE_APP, "APP: Re-starting Stack....\r\n");
        if (ZPS_E_SUCCESS == eStatus)
        {
            DBG_vPrintf(TRACE_APP, "APP: DONE\r\n");
        }
        else
        {
            DBG_vPrintf(TRACE_APP, "APP: ZPS_eZdoStartStack() failed error %d", eStatus);
        }

        ZPS_eAplZdoPermitJoining(0xff);
        OS_eStartSWTimer(APP_tmrSampleSensors, SAMPLE_TIME, NULL);
    }
    /* else perform any actions require on initial start-up */
    else
    {
        /* Turn on both LEDs to indicate device is joining network */
        APP_vLedSet(0, TRUE);
        APP_vLedSet(1, TRUE);

        s_sDevice.eState = E_STARTUP;
    }
}

/****************************************************************************
 *
 * NAME: APP_SensorNodeTask
 *
 * DESCRIPTION:
 * Main state machine for the home sensor demo coordinator
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
OS_TASK(APP_taskSensorNode)
{
    ZPS_tsAfEvent sStackEvent;

    DBG_vPrintf(TRACE_APP, "APP: Sensor Node Task\n");

    sStackEvent.eType = ZPS_EVENT_NONE;
    if (OS_E_OK != OS_eCollectMessage(APP_msgZpsEvents, &sStackEvent))
    {
        DBG_vPrintf(TRACE_APP, "APP: No event to process!\n");
    }

    switch (s_sDevice.eState)
    {
        case E_STARTUP:
            vHandleStartupEvent(sStackEvent);
            break;

        case E_NETWORK_DISCOVERY:
            vHandleNetworkDiscoveryEvent(sStackEvent);
            break;

        case E_NETWORK_JOIN:
            vHandleNetworkJoinEvent(sStackEvent);
            break;

        case E_MONITOR_SENSORS:
            vHandleMonitorSensorsEvent(sStackEvent);
            break;
        default:
            break;
    }
}

/****************************************************************************
 *
 * NAME: APP_u8GetSequenceNumber
 *
 * DESCRIPTION:
 * Returns the frame sequence number
 *
 * PARAMETERS: Name     RW  Usage
 *             bInc     R   Increments sequence number
 *
 * RETURNS:
 * Sequence number
 *
 ****************************************************************************/
PUBLIC uint8 APP_u8GetSequenceNumber(bool_t bInc)
{
    PRIVATE uint8 s_u8SeqNum = 0;

    if (bInc)
    {
        return s_u8SeqNum++;
    }
    return s_u8SeqNum;
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME: vHandleStartupEvent
 *
 * DESCRIPTION:
 * Handles stack events when the Controller is in its startup state
 *
 * PARAMETERS: Name         RW  Usage
 *             sStackEvent  R   Contains details of the stack event
 *
 * RETURNS:
 * Sequence number
 *
 ****************************************************************************/

PRIVATE void vHandleStartupEvent(ZPS_tsAfEvent sStackEvent)
{

    PRIVATE uint8 u8Channel = 11;

    /* Start the network stack as a router  */
    DBG_vPrintf(TRACE_APP, "APP: Starting ZPS\n");
    vClearDiscNT();

    /* Set channel to scan and start stack */
    ZPS_psAplAibGetAib()->apsChannelMask = 1 << u8Channel;

    ZPS_tsNwkNib * thisNib;

    void * thisNet = ZPS_pvAplZdoGetNwkHandle();
    thisNib = ZPS_psNwkNibGetHandle(thisNet);
    DBG_vPrintf(TRACE_APP, "APP: Starting ZPS SecLev=%d\n", thisNib->u8SecurityLevel);
    ZPS_teStatus eStatus = ZPS_eAplZdoStartStack();
    if (ZPS_E_SUCCESS == eStatus)
    {
        DBG_vPrintf(TRACE_APP, "APP: Stack started\n");
        DBG_vPrintf(TRACE_APP, "APP: Scan Channel: %d   Channel Mask: %08x\n", u8Channel, ZPS_psAplAibGetAib()->apsChannelMask);
        s_sDevice.eState = E_NETWORK_DISCOVERY;

        /* Move channel counter to next channel */
        u8Channel++;
        if (27 == u8Channel)
        {
          u8Channel = 11;
        }

    }
    else
    {
        DBG_vPrintf(TRACE_APP, "APP: ZPS_eAplZdoStartStack() failed error %d\n", eStatus);
        /* Try again ... */
        OS_eActivateTask(APP_taskSensorNode);
    }
}

/****************************************************************************
 *
 * NAME: vHandleNetworkDiscoveryEvent
 *
 * DESCRIPTION:
 * Handles stack events when the Controller is in its network discovery state
 *
 * PARAMETERS: Name         RW  Usage
 *             sStackEvent  R   Contains details of the stack event
 *
 * RETURNS:
 * Sequence number
 *
 ****************************************************************************/

PRIVATE void vHandleNetworkDiscoveryEvent(ZPS_tsAfEvent sStackEvent)
{


    static uint8 u8ChannelRetries = 0;

    /* wait for node to discover networks... */
    if (ZPS_EVENT_NONE != sStackEvent.eType)
    {
        if (ZPS_EVENT_NWK_DISCOVERY_COMPLETE == sStackEvent.eType)
        {
            DBG_vPrintf(TRACE_APP, "APP: Network discovery complete\n");

            if ((ZPS_E_SUCCESS == sStackEvent.uEvent.sNwkDiscoveryEvent.eStatus) ||
                  (ZPS_NWK_ENUM_NEIGHBOR_TABLE_FULL == sStackEvent.uEvent.sNwkDiscoveryEvent.eStatus))
            {
                DBG_vPrintf(TRACE_APP, "APP:    Found %d networks\n", sStackEvent.uEvent.sNwkDiscoveryEvent.u8NetworkCount);
                DBG_vPrintf(TRACE_APP, "APP:    Unscanned channels %08x\n", sStackEvent.uEvent.sNwkDiscoveryEvent.u32UnscannedChannels);
            }
            else
            {
                DBG_vPrintf(TRACE_APP, "APP: Network discovery failed with error %d\n",sStackEvent.uEvent.sNwkDiscoveryEvent.eStatus);
            }

            /* Networks were found */
            if ((0 != sStackEvent.uEvent.sNwkDiscoveryEvent.u8NetworkCount) &&
                (sStackEvent.uEvent.sNwkDiscoveryEvent.u8SelectedNetwork != 0xff))
            {

                ZPS_tsNwkNetworkDescr *psNwkDescr;
                ZPS_teStatus eStatus;

                psNwkDescr = &sStackEvent.uEvent.sNwkDiscoveryEvent.asNwkDescriptors[sStackEvent.uEvent.sNwkDiscoveryEvent.u8SelectedNetwork];

                eStatus = ZPS_eAplZdoJoinNetwork(psNwkDescr);
                if (ZPS_E_SUCCESS == eStatus)
                {
                   DBG_vPrintf(TRACE_APP, "APP: Joining network Ext PAN ID = %016llx\n", psNwkDescr->u64ExtPanId);
                   s_sDevice.eState = E_NETWORK_JOIN;
                    return;
                }
                else
                {
                    DBG_vPrintf(TRACE_APP, "APP:    Failed to join network reason = %02x\n", eStatus);
                    s_sDevice.eState = E_STARTUP;
                    OS_eActivateTask(APP_taskSensorNode);
                }
            }
            /* No Networks found */
            else
            {
                if (0 != sStackEvent.uEvent.sNwkDiscoveryEvent.u32UnscannedChannels)
                {
                    u8ChannelRetries++;

                    if( u8ChannelRetries < MAX_SINGLE_CHANNEL_RETRIES )
                    {
                        /* continue to look for networks on unscanned channels */
                        DBG_vPrintf(TRACE_APP, "APP:    Continue scanning ...\n");
                        vClearDiscNT();
                        ZPS_eAplZdoDiscoverNetworks(sStackEvent.uEvent.sNwkDiscoveryEvent.u32UnscannedChannels);
                    }
                    else
                    {
                        u8ChannelRetries = 0;

                        DBG_vPrintf(TRACE_APP, "APP:    Restart scan\n");
                        s_sDevice.eState = E_STARTUP;
                        OS_eActivateTask(APP_taskSensorNode);
                    }
                }
                else
                {
                    u8ChannelRetries = 0;
                    DBG_vPrintf(TRACE_APP, "APP:    Restart scan\n");
                    s_sDevice.eState = E_STARTUP;
                    OS_eActivateTask(APP_taskSensorNode);
                }
            }

        }
        else if (ZPS_EVENT_NWK_FAILED_TO_JOIN == sStackEvent.eType)
        {
            DBG_vPrintf(TRACE_APP, "APP: Failed to join network: error = %d\n", sStackEvent.uEvent.sNwkJoinFailedEvent.u8Status);
            s_sDevice.eState = E_STARTUP;
            OS_eActivateTask(APP_taskSensorNode);
        }
        else if (ZPS_EVENT_NWK_JOINED_AS_ROUTER == sStackEvent.eType)
        {
            /* Node has rejoined using Nwk Rejoin command, rather than than MAC association */
            DBG_vPrintf(TRACE_APP, "APP: Node rejoined network with Addr 0x%04x\n",
                        sStackEvent.uEvent.sNwkJoinedEvent.u16Addr);

            /* Turn off LEDs to indicate joining complete */
            APP_vLedSet(0, FALSE);
            APP_vLedSet(1, FALSE);

            /* start sample timer to begin sampling and sending data */
            OS_eStartSWTimer(APP_tmrSampleSensors, SAMPLE_TIME, NULL);
            s_sDevice.eState = E_MONITOR_SENSORS;
            ZPS_eAplZdoPermitJoining(0xff);
            PDM_vSaveRecord(&s_sDevicePDDesc);
        }
        else if (ZPS_EVENT_ERROR == sStackEvent.eType)
        {
            DBG_vPrintf(TRACE_APP, "APP: ZPS Error - %d\n", sStackEvent.uEvent.sAfErrorEvent.eError);

            if ( ZPS_ERROR_OS_MESSAGE_QUEUE_OVERRUN ==   sStackEvent.uEvent.sAfErrorEvent.eError)
            {
                DBG_vPrintf(TRACE_APP, "    Queue handle %d\n", sStackEvent.uEvent.sAfErrorEvent.uErrorData.sAfErrorOsMessageOverrun.hMessage);
            }

        }
        else
        {
            DBG_vPrintf(TRACE_APP, "APP: Unexpected event in E_NETWORK_DISCOVERY - %d\n", sStackEvent.eType);
        }
    }
}
/****************************************************************************
 *
 * NAME: vHandleNetworkJoinEvent
 *
 * DESCRIPTION:
 * Handles stack events when the Controller is in its network join state
 *
 * PARAMETERS: Name         RW  Usage
 *             sStackEvent  R   Contains details of the stack event
 *
 * RETURNS:
 * Sequence number
 *
 ****************************************************************************/
PRIVATE void vHandleNetworkJoinEvent(ZPS_tsAfEvent sStackEvent)
{
    uint64 u64ExtPANID;
    /* join router to discovered network */
    if (ZPS_EVENT_NONE != sStackEvent.eType)
    {
        if (ZPS_EVENT_NWK_JOINED_AS_ROUTER == sStackEvent.eType)
        {
            DBG_vPrintf(TRACE_APP, "APP: Node joined network with Addr 0x%04x\n",
                    sStackEvent.uEvent.sNwkJoinedEvent.u16Addr);

            // save the EPID for rejoins
            u64ExtPANID = ZPS_u64NwkNibGetEpid(ZPS_pvAplZdoGetNwkHandle());
            ZPS_eAplAibSetApsUseExtendedPanId(u64ExtPANID);

            /* Turn off LEDs to indicate joining complete */
            APP_vLedSet(0, FALSE);
            APP_vLedSet(1, FALSE);

            /* start sample timer to begin sampling and sending data */
            OS_eStartSWTimer(APP_tmrSampleSensors, SAMPLE_TIME, NULL);
            s_sDevice.eState = E_MONITOR_SENSORS;
            ZPS_eAplZdoPermitJoining(0xff);
            PDM_vSaveRecord(&s_sDevicePDDesc);
        }
        else if (ZPS_EVENT_NWK_FAILED_TO_JOIN == sStackEvent.eType)
        {
            DBG_vPrintf(TRACE_APP, "APP: Node failed to join network. Retrying ...\n");
            s_sDevice.eState = E_STARTUP;
            OS_eActivateTask(APP_taskSensorNode);
        }
        else
        {
            DBG_vPrintf(TRACE_APP, "APP: Unexpected event in E_NETWORK_JOIN - %d\n", sStackEvent.eType);
        }
    }
}

/****************************************************************************
 *
 * NAME: vHandleMonitorSensorsEvent
 *
 * DESCRIPTION:
 * Handles stack events when the Controller is in its monitor sensors state
 *
 * PARAMETERS: Name         RW  Usage
 *             sStackEvent  R   Contains details of the stack event
 *
 * RETURNS:
 * Sequence number
 *
 ****************************************************************************/
PRIVATE void vHandleMonitorSensorsEvent(ZPS_tsAfEvent sStackEvent)
{
    /*
     *  nothing to do other than allow sensor data to be sent
     *  to the controller node
     */
    DBG_vPrintf(TRACE_APP, "APP: Monitoring Sensors!\n");
    if (ZPS_EVENT_NONE != sStackEvent.eType)
    {
        switch (sStackEvent.eType)
        {
            case ZPS_EVENT_APS_DATA_INDICATION:
                DBG_vPrintf(TRACE_APP, "APP: Monitor Sensors ZPS_EVENT_AF_DATA_INDICATION\n");
                break;

            case ZPS_EVENT_NWK_NEW_NODE_HAS_JOINED:
                DBG_vPrintf(TRACE_APP, "APP: Monitor Sensors ZPS_EVENT_NEW_NODE_HAS_JOINED\n");
                break;

            case ZPS_EVENT_NWK_LEAVE_INDICATION:
                DBG_vPrintf(TRACE_APP, "APP: Monitor Sensors ZPS_EVENT_LEAVE_INDICATION\n");
                break;

            case ZPS_EVENT_NWK_LEAVE_CONFIRM:
                DBG_vPrintf(TRACE_APP, "APP: Monitor Sensors ZPS_EVENT_LEAVE_CONFIRM\n");
                break;

            case ZPS_EVENT_NWK_STATUS_INDICATION:
                DBG_vPrintf(TRACE_APP, "APP: Monitor Sensors ZPS_EVENT_NWK_STATUS_INDICATION\n");
                break;

            case ZPS_EVENT_NWK_ROUTE_DISCOVERY_CONFIRM:
                DBG_vPrintf(TRACE_APP, "APP: Monitor Sensors ZPS_EVENT_ROUTE_DISCOVERY_CFM\n");
                break;

            case ZPS_EVENT_APS_DATA_CONFIRM:
                DBG_vPrintf(TRACE_APP, "APP: ZPS_EVENT_APS_DATA_CONFIRM Status %d, Address 0x%04x\n",
                            sStackEvent.uEvent.sApsDataConfirmEvent.u8Status,
                            sStackEvent.uEvent.sApsDataConfirmEvent.u16DstAddr);
                break;

            case ZPS_EVENT_ERROR:
                DBG_vPrintf(TRACE_APP, "APP: Monitor Sensors ZPS_EVENT_ERROR\n");
                DBG_vPrintf(TRACE_APP, " Error Code %d\n",sStackEvent.uEvent.sAfErrorEvent.eError);

                if ( ZPS_ERROR_OS_MESSAGE_QUEUE_OVERRUN == sStackEvent.uEvent.sAfErrorEvent.eError)
                {
                    DBG_vPrintf(TRACE_APP, "    Queue handle %d\n",sStackEvent.uEvent.sAfErrorEvent.uErrorData.sAfErrorOsMessageOverrun.hMessage);
                }
                break;

            case ZPS_EVENT_NWK_FAILED_TO_JOIN:
                DBG_vPrintf(TRACE_APP, "APP: Node failed to join network. Retrying ...\n");
                s_sDevice.eState = E_STARTUP;
                OS_eActivateTask(APP_taskSensorNode);
                break;

            case ZPS_EVENT_NWK_POLL_CONFIRM:
                DBG_vPrintf(TRACE_APP, "APP: Monitor Sensors ZPS_EVENT_NWK_POLL_CONFIRM\n");
                break;

            default:
                DBG_vPrintf(TRACE_APP, "APP: Monitor Sensors unhandled event %d\n", sStackEvent.eType);
        }
    }
}

/****************************************************************************
 *
 * NAME: vClearDiscNT
 *
 * DESCRIPTION:
 * Handles the re-intialisation of the discovery table to 0
 *
 * PARAMETERS: None
 *
 *
 * RETURNS:
 * None
 *
 ****************************************************************************/

PUBLIC void vClearDiscNT(void)
{
    ZPS_tsNwkNib * thisNib;

    void * thisNet = ZPS_pvAplZdoGetNwkHandle();
    thisNib = ZPS_psNwkNibGetHandle(thisNet);

    memset(thisNib->sTbl.psNtDisc, 0, sizeof(ZPS_tsNwkDiscNtEntry) * thisNib->sTblSize.u8NtDisc);

}


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
