/*****************************************************************************
 *
 * MODULE:             Home Sensor Demo
 *
 * COMPONENT:          app_controller_node.c
 *
 * AUTHOR:             MRW
 *
 * DESCRIPTION:        Coordinator for the home sensor demo
 *
 * $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Application_Notes/JN-AN-1122-ZBPro-Home-Sensor-Demo/Tags/ZBPRO-HOME-SENSOR-DEMO_1v7_RC4/AN1122_ZBP_ControllerNode/Source/app_controller_node.c $
 *
 * $Revision: 28632 $
 *
 * $LastChangedBy: nxp29772 $
 *
 * $LastChangedDate: 2011-01-11 14:57:35 +0000 (Tue, 11 Jan 2011) $
 *
 * $Id: app_controller_node.c 28632 2011-01-11 14:57:35Z nxp29772 $
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
#include "dbg.h"
#include "os.h"
#include "os_gen.h"
#include "pdum_apl.h"
#include "pdum_gen.h"
#include "pwrm.h"
#include "pdm.h"

#include "zps_gen.h"
#include "zps_apl.h"
#include "zps_apl_af.h"
#include "zps_apl_aib.h"
#include "zps_apl_zdo.h"
#include "zps_apl_zdp.h"

#include "app_home_sensor_demo.h"
#include "app_controller_node.h"
#include "app_buttons.h"
#include "app_display.h"
#include "app_log.h"
#include "app_timer_driver.h"
#include "app_led.h"
#include "serialq.h"
#include "uart.h"
#include "serial.h"
#include "serialq.h"



/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifndef TRACE_APP
#define TRACE_APP FALSE
#endif

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum
{
    E_STARTUP,
    E_CONFIGURE_NETWORK,
    E_NETWORK_FORMATION,
    E_NETWORK_SCREEN,
    E_NODE_SCREEN,
} teCoordState;

typedef struct
{
    teCoordState eState;
    bool_t bPermitJoining;
    uint8 u8CurrentRadioChan;
} tsDevice;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void vHandleStartupEvent(ZPS_tsAfEvent sStackEvent);
PRIVATE void vHandleConfigureNetworkEvent(ZPS_tsAfEvent sStackEvent, APP_tsEvent sAppEvent);
PRIVATE void vHandleNetworkFormationEvent(ZPS_tsAfEvent sStackEvent, APP_tsEvent sAppEvent);
PRIVATE void vHandleNetworkScreenEvent(ZPS_tsAfEvent sStackEvent, APP_tsEvent sAppEvent);
PRIVATE void vHandleNodeScreenEvent(ZPS_tsAfEvent sStackEvent, APP_tsEvent sAppEvent);
PRIVATE void vCheckStackEvent(ZPS_tsAfEvent sStackEvent);
PRIVATE void vHandleLedControlEvent(bool_t bLedState);
PRIVATE void vSendLedData(PDUM_thAPduInstance hAPduInst);
//int vStrlen(char *s);

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
 * NAME: APP_vCoordInitialise
 *
 * DESCRIPTION:
 * Initialises the application
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vInitialiseControllerNode(void)
{
    uint32 u32ChannelMask;
    uint8 u8Chan;
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
	vSerial_Init();

    /* Restore any application data previously saved to flash */
    s_sDevice.eState = E_STARTUP;
    PDM_eLoadRecord(&s_sDevicePDDesc,
                    "APP_COORD",
                    &s_sDevice,
                    sizeof(s_sDevice),
                    TRUE);

    /* Initialise ZBPro stack */
    ZPS_eAplAfInit();



    /* Initialise logging module */
    APP_vLogInitialise();

    /* Always initialise any peripherals used by the application */
    APP_vDisplayInitialise();
    APP_vLedsInitialise();


    /* If the device state has been restored from flash, re-start the stack
     * and set the app running again */
    if (E_NETWORK_SCREEN == s_sDevice.eState
            || E_NODE_SCREEN == s_sDevice.eState)
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

        /* turn on joining */
        s_sDevice.bPermitJoining = TRUE;
        ZPS_eAplZdoPermitJoining(0xff);

        /* always start with network screen */
        s_sDevice.eState = E_NETWORK_SCREEN;
        APP_vDisplaySetState(APP_E_DISPLAY_STATE_NETWORK);
        APP_vDisplayUpdate();

        /* start logging now the network is up */
        APP_vLogStart();
    }
    else
        /* else perform any actions require on initial start-up */
    {
        /* Turn on both LEDs to indicate device is forming network */
        APP_vLedSet(0, TRUE);
        APP_vLedSet(1, TRUE);
        APP_vLedSet(2, TRUE);
        APP_vLedSet(3, TRUE);

        /* find the radio channel number from the channel bit mask */
        s_sDevice.u8CurrentRadioChan = 11;
        u32ChannelMask = ZPS_psAplAibGetAib()->apsChannelMask;
        for (u8Chan = 11; u8Chan < 27; u8Chan++)
        {
            if ((1UL << u8Chan) & u32ChannelMask)
            {
                s_sDevice.u8CurrentRadioChan = u8Chan;
                break;
            }
        }
        DBG_vPrintf(TRACE_APP, "APP: Channel mask = 0x%08x, chan = %d\n", u32ChannelMask, u8Chan);

        s_sDevice.bPermitJoining = FALSE;
        s_sDevice.eState = E_STARTUP;
    }
}

/****************************************************************************
 *
 * NAME: APP_u8CoordGetCurRadioChan
 *
 * DESCRIPTION:
 * Returns the current radio channel that the coordinator is using
 *
 * RETURNS:
 * Current channel number of radio
 *
 ****************************************************************************/
PUBLIC uint8 APP_u8ControllerNodeGetCurRadioChan(void)
{
    return s_sDevice.u8CurrentRadioChan;
}

/****************************************************************************
 *
 * NAME: APP_bGetPermitJoining
 *
 * DESCRIPTION:
 * Returns the permit joining state
 *
 * RETURNS:
 * The Permit joining state
 *
 ****************************************************************************/
PUBLIC bool_t APP_bGetPermitJoining(void)
{
    return s_sDevice.bPermitJoining;
}

/****************************************************************************
 *
 * NAME: APP_vSetPermitJoining
 *
 * DESCRIPTION:
 * Sets the permit joining state
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vSetPermitJoining(bool_t bPermit)
{
    s_sDevice.bPermitJoining = bPermit;
    if (bPermit)
    {
        ZPS_eAplZdoPermitJoining(0xff);
    }
    else
    {
        ZPS_eAplZdoPermitJoining(0x00);
    }
    APP_vDisplayUpdate();
}

/****************************************************************************
 *
 * NAME: APP_ControllerNodeTask
 *
 * DESCRIPTION:
 * Main state machine for the home sensor demo coordinator
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
OS_TASK(APP_taskControllerNode)
{
    APP_tsEvent sAppEvent;
    ZPS_tsAfEvent sStackEvent;

    sAppEvent.eType = APP_E_EVENT_NONE;
    sStackEvent.eType = ZPS_EVENT_NONE;

    /* check if any messages to collect */
    if (OS_E_OK != OS_eCollectMessage(APP_msgButtonEvent, &sAppEvent))
    {
        if (OS_E_OK != OS_eCollectMessage(APP_msgZpsEvents, &sStackEvent))
        {
            DBG_vPrintf(TRACE_APP, "APP: No event to process\n");
        }
    }

    switch (s_sDevice.eState)
    {
        case E_STARTUP:
            vHandleStartupEvent(sStackEvent);
            break;

        case E_CONFIGURE_NETWORK:
            vHandleConfigureNetworkEvent(sStackEvent, sAppEvent);
            break;

        case E_NETWORK_FORMATION:
            vHandleNetworkFormationEvent(sStackEvent, sAppEvent);
            break;

        case E_NETWORK_SCREEN:
            vHandleNetworkScreenEvent(sStackEvent, sAppEvent);
            break;

        case E_NODE_SCREEN:
            vHandleNodeScreenEvent(sStackEvent, sAppEvent);
            break;

        default:
            break;
    }
}

/****************************************************************************
 *
 * NAME: APP_taskTogglePermitJoining
 *
 * DESCRIPTION:
 * Toggles Permit Joining state
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
OS_TASK(APP_taskTogglePermitJoining)
{
    /* only allow toggle action if max number of sensors has not been reached */
    if (APP_u8ControllerNodeNumDataSensors() < APP_HSDEMO_NUM_SENSORNODES)
    {
        s_sDevice.bPermitJoining = !s_sDevice.bPermitJoining;
        ZPS_eAplZdoPermitJoining(s_sDevice.bPermitJoining ? 0xff : 0);
        APP_vDisplayUpdate();
        DBG_vPrintf(TRACE_APP, "APP: Joining set to %d\n", s_sDevice.bPermitJoining);
    }
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME: vHandleStartupEvent
 *
 * DESCRIPTION:
 * Handles stack events when the Controller is in its initial  state
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
    /*  display the startup splash screen */
    DBG_vPrintf(TRACE_APP, "APP: Application startup\n");

    APP_vDisplaySetState(APP_E_DISPLAY_STATE_SPLASH);
    APP_vDisplayUpdate();

    /* Stops Coordinator from sleeping ever - but does allow doze */
    PWRM_eStartActivity();

    s_sDevice.eState = E_CONFIGURE_NETWORK;
}

/****************************************************************************
 *
 * NAME: vHandleConfigureNetworkEvent
 *
 * DESCRIPTION:
 * Handles stack events when the Controller is in its network configuration state
 *
 * PARAMETERS: Name         RW  Usage
 *             sStackEvent  R   Contains details of the stack event
 *
 * RETURNS:
 * Sequence number
 *
 ****************************************************************************/
PRIVATE void vHandleConfigureNetworkEvent(ZPS_tsAfEvent sStackEvent, APP_tsEvent sAppEvent)
{
    /*
     * respond to button presses on the splash screen to configure the
     * network channel number.
     */
    if (APP_E_EVENT_BUTTON_UP == sAppEvent.eType)
    {
        if (APP_E_BUTTONS_BUTTON_2 == sAppEvent.sButton.u8Button)
        {
            /* perform action channel up action */
            if (26 > s_sDevice.u8CurrentRadioChan)
            {
                s_sDevice.u8CurrentRadioChan++;
                APP_vDisplayUpdate();
            }
        }
        else if (APP_E_BUTTONS_BUTTON_3 == sAppEvent.sButton.u8Button)
        {
            /* perform action channel down action */
            if (11 < s_sDevice.u8CurrentRadioChan)
            {
                s_sDevice.u8CurrentRadioChan--;
                APP_vDisplayUpdate();
            }
        }
        else if (APP_E_BUTTONS_BUTTON_4 == sAppEvent.sButton.u8Button)
        {
            /* Start ZBPro stack from a low priority task to allow the display
             * to be updated */
            APP_vDisplaySetState(APP_E_DISPLAY_STATE_NETWORK_STARTUP);
            APP_vDisplayUpdate();
            OS_eActivateTask(APP_StartZBProStack);
        }
    }
}

/****************************************************************************
 *
 * NAME: APP_StartZBProStack
 *
 * DESCRIPTION:
 * Handles starting the ZBPro stack from a low priority so that the display
 * can be updated.
 *
 * PARAMETERS: Name         RW  Usage
 *
 * RETURNS:
 *
 ****************************************************************************/

OS_TASK(APP_StartZBProStack)
{

    DBG_vPrintf(TRACE_APP, "APP: Starting ZPS\r\n");
    ZPS_psAplAibGetAib()->u32ApsChannelMask
    = s_sDevice.u8CurrentRadioChan;
    ZPS_teStatus eStatus = ZPS_eAplZdoStartStack();

    DBG_vPrintf(TRACE_APP, "APP: Stack started\r\n");

    if (ZPS_E_SUCCESS == eStatus)
    {
        s_sDevice.eState = E_NETWORK_FORMATION;
    }
    else
    {
        DBG_vPrintf(TRACE_APP, "APP: ZPS_eZdoStartStack() failed error %d", eStatus);
    }
}


/****************************************************************************
 *
 * NAME: vHandleNetworkFormationEvent
 *
 * DESCRIPTION:
 * Handles stack events when the Controller is in its network formation state
 *
 * PARAMETERS: Name         RW  Usage
 *             sStackEvent  R   Contains details of the stack event
 *
 * RETURNS:
 * Sequence number
 *
 ****************************************************************************/
PRIVATE void vHandleNetworkFormationEvent(ZPS_tsAfEvent sStackEvent, APP_tsEvent sAppEvent)
{
    /*wait for network stack to start up as a coordinator */
    if (ZPS_EVENT_NONE != sStackEvent.eType)
    {
        if (ZPS_EVENT_NWK_STARTED == sStackEvent.eType)
        {
            DBG_vPrintf(TRACE_APP, "APP: Network Started\r\n");

            /* Turn off LEDs to indicate network formation complete */
            APP_vLedSet(0, FALSE);
            APP_vLedSet(1, FALSE);
            APP_vLedSet(2, FALSE);
            APP_vLedSet(3, FALSE);

            /* turn on joining */
            s_sDevice.bPermitJoining = TRUE;
            ZPS_eAplZdoPermitJoining(0xff);

            /* start logging now the network is up */
            APP_vLogStart();

            APP_vDisplaySetState(APP_E_DISPLAY_STATE_NETWORK);
            APP_vDisplayUpdate();

            s_sDevice.eState = E_NETWORK_SCREEN;
            PDM_vSaveRecord(&s_sDevicePDDesc);
        }
    }
    else
    {
        if (APP_E_EVENT_NONE == sAppEvent.eType)
        {
            DBG_vPrintf(TRACE_APP, "APP: Unexpected event in E_NETWORK_FORMATION");
        }
    }
}

/****************************************************************************
 *
 * NAME: vHandleNetworkScreenEvent
 *
 * DESCRIPTION:
 * Handles stack events when the Controller is in its "network" display mode state
 *
 * PARAMETERS: Name         RW  Usage
 *             sStackEvent  R   Contains details of the stack event
 *
 * RETURNS:
 * Sequence number
 *
 ****************************************************************************/
PRIVATE void vHandleNetworkScreenEvent(ZPS_tsAfEvent sStackEvent, APP_tsEvent sAppEvent)
{
//	uint8 *u8Char;
//	string u8Char1;
//	uint8 *u8String;
    /* respond to button presses when in "network" display mode */
    if (APP_E_EVENT_BUTTON_DOWN == sAppEvent.eType)
    {
        if (APP_E_BUTTONS_BUTTON_1 == sAppEvent.sButton.u8Button)
        {
            DBG_vPrintf(TRACE_APP, "Starting join toggle timer\n");
            OS_eStopSWTimer(APP_tmrJoiningButton);
            OS_eStartSWTimer(APP_tmrJoiningButton, APP_TIME_MS(2000), NULL);
        }
    }

    if (APP_E_EVENT_BUTTON_UP == sAppEvent.eType)
    {
        if (APP_E_BUTTONS_BUTTON_1 == sAppEvent.sButton.u8Button)
        {
            if (OS_E_SWTIMER_RUNNING == OS_eGetSWTimerStatus(APP_tmrJoiningButton))
            {
                /*
                 * Button press was too short to trigger permit-join toggling
                 * therefore it was pressed in order to switch to the node
                 * display screen.
                 *
                 * Stop timer to prevent toggling from occurring.
                 */
                OS_eStopSWTimer(APP_tmrJoiningButton);

                /* If at least one sensor has joined, then it is valid to go
                 * to the node display screen
                 */
                if (0 < APP_u8ControllerNodeNumDataSensors())
                {
                    APP_vDisplaySetState(APP_E_DISPLAY_STATE_NODE);
                    APP_vDisplayUpdate();
                    s_sDevice.eState = E_NODE_SCREEN;
                }
            }
        }
        else if (APP_E_BUTTONS_BUTTON_2 == sAppEvent.sButton.u8Button)
        {
            /*
             * perform select temperature sensors action
             */
            APP_vDisplaySetSensor(APP_E_SENSOR_TEMP);
            APP_vDisplayUpdate();
            vSerial_QData();
        }
        else if (APP_E_BUTTONS_BUTTON_3 == sAppEvent.sButton.u8Button)
        {
            /* perform select humidity sensors action */
            APP_vDisplaySetSensor(APP_E_SENSOR_HTS);
            APP_vDisplayUpdate();
            vSerial_SensorData();
        }
        else if (APP_E_BUTTONS_BUTTON_4 == sAppEvent.sButton.u8Button)
        {
            /* perform select humidity sensors action */
            APP_vDisplaySetSensor(APP_E_SENSOR_ALS);
            APP_vDisplayUpdate();
            Rfid_Getprotocol();
        }
    }
    vCheckStackEvent(sStackEvent);
}

/****************************************************************************
 *
 * NAME: vHandleNodeScreenEvent
 *
 * DESCRIPTION:
 * Handles stack events when the Controller is in its "node" display mode state
 *
 * PARAMETERS: Name         RW  Usage
 *             sStackEvent  R   Contains details of the stack event
 *
 * RETURNS:
 * Sequence number
 *
 ****************************************************************************/
PRIVATE void vHandleNodeScreenEvent(ZPS_tsAfEvent sStackEvent, APP_tsEvent sAppEvent)
{
    /* respond to button presses when in "node" display mode */
    if (APP_E_EVENT_BUTTON_UP == sAppEvent.eType)
    {
        if (APP_E_BUTTONS_BUTTON_1 == sAppEvent.sButton.u8Button)
        {
            /* cycle through the sensor nodes */
            APP_vDisplayCycleNode();
            APP_vDisplayUpdate();
        }
        else if (APP_E_BUTTONS_BUTTON_2 == sAppEvent.sButton.u8Button)
        {
            /* Go to network screen */
            APP_vDisplaySetState(APP_E_DISPLAY_STATE_NETWORK);
            APP_vDisplayUpdate();
            s_sDevice.eState = E_NETWORK_SCREEN;
        }
        else if (APP_E_BUTTONS_BUTTON_3 == sAppEvent.sButton.u8Button)
        {
            /* Turn remote LED off on currently selected node */
            vHandleLedControlEvent(FALSE);
        }
        else if (APP_E_BUTTONS_BUTTON_4 == sAppEvent.sButton.u8Button)
        {
            /* Turn remote LED on on currently selected node */
            vHandleLedControlEvent(TRUE);
        }
    }
    vCheckStackEvent(sStackEvent);
}

/****************************************************************************
 *
 * NAME: vHandleLedControlEvent
 *
 * DESCRIPTION:
 * Check for and act upon any valid stack event. This function should be called
 * after node has formed network.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  sStackEvent     R   Contains details of stack event
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vCheckStackEvent(ZPS_tsAfEvent sStackEvent)
{
    if (ZPS_EVENT_NONE != sStackEvent.eType)
    {
        switch (sStackEvent.eType)
        {
            case ZPS_EVENT_APS_DATA_INDICATION:
                DBG_vPrintf(TRACE_APP, "APP: Monitor Sensors ZPS_EVENT_AF_DATA_INDICATION\n");
                break;

            case ZPS_EVENT_NWK_NEW_NODE_HAS_JOINED:
                DBG_vPrintf(TRACE_APP, "APP: ZPS_EVENT_NEW_NODE_HAS_JOINED, Nwk Addr=0x%04x\n",
                            sStackEvent.uEvent.sNwkJoinIndicationEvent.u16NwkAddr);
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
                DBG_vPrintf(TRACE_APP, "APP: Monitor Controller ZPS_EVENT_ERROR\n");
                DBG_vPrintf(TRACE_APP, "    Error Code %d\n", sStackEvent.uEvent.sAfErrorEvent.eError);
                if ( ZPS_ERROR_OS_MESSAGE_QUEUE_OVERRUN == sStackEvent.uEvent.sAfErrorEvent.eError)
                {
                        DBG_vPrintf(TRACE_APP, "    Queue handle %d\n", sStackEvent.uEvent.sAfErrorEvent.uErrorData.sAfErrorOsMessageOverrun.hMessage);
                }
                break;

            case ZPS_EVENT_NWK_POLL_CONFIRM:
                DBG_vPrintf(TRACE_APP, "APP: Monitor Sensors ZPS_EVENT_NEW_POLL_COMPLETE, status = %d\n",
                            sStackEvent.uEvent.sNwkPollConfirmEvent.u8Status );
                break;

            case ZPS_EVENT_NWK_JOINED_AS_ROUTER:
            case ZPS_EVENT_NWK_JOINED_AS_ENDDEVICE:
            case ZPS_EVENT_NWK_STARTED:
            case ZPS_EVENT_NWK_FAILED_TO_START:
            case ZPS_EVENT_NWK_FAILED_TO_JOIN:
            case ZPS_EVENT_NWK_DISCOVERY_COMPLETE:
                // Deliberate fall through
            default:
                DBG_vPrintf(TRACE_APP, "APP: Monitor Sensors unhandled event %d\n", sStackEvent.eType);
                break;
        }
    }
}

/****************************************************************************
 *
 * NAME: vHandleLedControlEvent
 *
 * DESCRIPTION:
 * Creates a frame to control a remote LED according to button press
 *
 * PARAMETERS:      Name        RW  Usage
 *                  bLedState   R   On/off signal for the remote LED
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vHandleLedControlEvent(bool_t bLedState)
{
    PRIVATE PDUM_thAPduInstance s_hAPduInst = PDUM_INVALID_HANDLE;

    /* allocate an APdu  for the frame */
    s_hAPduInst = PDUM_hAPduAllocateAPduInstance(apduLedControl);
    if (PDUM_INVALID_HANDLE == s_hAPduInst)
    {
        DBG_vPrintf(TRACE_APP, "APP: No APDUs left to send data\n");
        return;
    }

    /* Put the LED state data into the payload */
    PDUM_u16APduInstanceWriteNBO(s_hAPduInst,
                                 APDU_LEDCTRL_STATE,
                                 "b",
                                 bLedState);

    vSendLedData(s_hAPduInst);
}

/****************************************************************************
 *
 * NAME: vSendLedControl
 *
 * DESCRIPTION:
 * Sends a frame containing the LED control signal
 *
 * PARAMETERS:      Name        RW  Usage
 *                  hAPduInst   R   Handle of the PDU to be sent
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vSendLedData(PDUM_thAPduInstance hAPduInst)
{
    PRIVATE uint8 u8SeqNum = 0;

    uint64 u64ExtAddr;
    uint8 u8Status;
    uint16 u16NwkAddr;

    u16NwkAddr = APP_u16GetSensorNodeAddr(APP_u8GetCurrentNode());

    /* Get our extended address */
    u64ExtAddr = ZPS_u64NwkNibGetExtAddr(ZPS_pvAplZdoGetNwkHandle());

    /* place extended address into payload */
    (void) PDUM_u16APduInstanceWriteNBO(hAPduInst,
                                        APDU_LEDCTRL_LENGTH,
                                        "bl",
                                        10,
                                        u64ExtAddr);

    PDUM_eAPduInstanceSetPayloadSize(hAPduInst, 10);

    /* send frame */
    u8Status = ZPS_eAplAfUnicastDataReq(hAPduInst,
                                        JENNIC_HSD_LED_CONTROL_CLUSTER_ID,
                                        AN1122_ZBP_CONTROLLERNODE_LED_CONTROL_ENDPOINT,
                                        AN1122_ZBP_SENSORNODE_LED_CONTROL_ENDPOINT,
                                        u16NwkAddr,
                                        ZPS_E_APL_AF_UNSECURE,
                                        0 /* default radius */,
                                        &u8SeqNum);

    u8SeqNum++;
    DBG_vPrintf(TRACE_APP, "APP: Sent LED data\n");
}

//
//int vStrlen(char *s)
//{
//   int c = 0;
//
//   while(*(s+c))
//      c++;
//
//   return c;
//}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
