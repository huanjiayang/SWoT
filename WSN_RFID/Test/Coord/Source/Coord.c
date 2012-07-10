/****************************************************************************
 *
 * MODULE:             Wireless UART
 *
 * COMPONENT:          wuart_c.c,v
 *
 * VERSION:
 *
 * REVISION:           1.3
 *
 * DATED:              2006/11/09 12:57:10
 *
 * STATUS:             Exp
 *
 * AUTHOR:
 *
 * DESCRIPTION
 *
 * CHANGE HISTORY:
 *
 * wuart_c.c,v
 * Revision 1.3  2006/11/09 12:57:10
 * Updated copyright information in file header
 *
 * Revision 1.2  2006/11/06 15:42:10
 * Updated to use interrupt driven comms
 *
 * Revision 1.1  2006/08/24 14:58:35
 * Initial version
 *
 *
 *
 * LAST MODIFIED BY:
 *                     $Modtime: $
 *
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
 * Copyright Jennic Ltd 2005, 2006, 2007. All rights reserved
 *
 ****************************************************************************/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <AppHardwareApi.h>
#include <AppQueueApi.h>
#include <mac_sap.h>
#include <mac_pib.h>
#include <string.h>
#include <AppApi.h>
#include <LedControl.h>

#include "config.h"
#include "serialq.h"
#include "uart.h"
#include "serial.h"
#include "lcd.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* System states with respect to screen display being shown */
typedef enum
{
    E_STATE_INIT,
    E_STATE_START_COORDINATOR,
    E_STATE_RUNNING_UART_APP

} teState;

///* Used to track an association between extended address and short address */
//typedef struct
//{
//    uint32 u32ExtAddrLo;
//    uint32 u32ExtAddrHi;
//    uint16 u16ShortAddr;
//} tsAssocNodes;


///* All application data with scope within the entire file is kept here,
//   including all stored node data, GUI settings and current state */
//typedef struct
//{
//    struct
//	{
//	    tsAssocNodes asAssocNodes[MAX_UART_NODES];
//	    uint8        u8AssociatedNodes;
//	} sNode;
//
//    struct
//    {
//        teState eState;
//        uint8   u8Channel;
//    } sSystem;
//} tsCoordData;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
//PRIVATE tsCoordData sCoordData;
//PRIVATE uint8 u8TxFrameHandle = 0;
//PRIVATE uint8 u8RxFrameHandle = 0;
//PRIVATE void *pvMac;
//PRIVATE MAC_Pib_s *psPib;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
PRIVATE void vWUART_Init(void);
PRIVATE void vWUART_TxData(void);
PRIVATE void vProcessEventQueues(void);
PRIVATE void vStartCoordinator(void);
//PRIVATE void vHandleNodeAssociation(MAC_MlmeDcfmInd_s *psMlmeInd);
PRIVATE void vProcessIncomingMlme(MAC_MlmeDcfmInd_s *psMlmeInd);
PRIVATE void vProcessIncomingData(MAC_McpsDcfmInd_s *psMcpsInd);
PRIVATE void vProcessIncomingHwEvent(AppQApiHwInd_s *psAHI_Ind);
PRIVATE void vTickTimerISR(uint32 u32Device, uint32 u32ItemBitmap);
//PRIVATE void vStartEnergyScan(void);
//PRIVATE void vHandleEnergyScanResponse(MAC_MlmeDcfmInd_s *psMlmeInd);

/****************************************************************************
 *
 * NAME: AppColdStart
 *
 * DESCRIPTION:
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * Entry point for a power on reset or wake from sleep mode.
 ****************************************************************************/
PUBLIC void AppColdStart(void)
{
	/* Disable watchdog if enabled by default */
	#ifdef WATCHDOG_ENABLED
	vAHI_WatchdogStop();
	#endif

    vWUART_Init();

    while(1)
    {
        vProcessEventQueues();

        switch (sCoordData.sSystem.eState)
        {
        case E_STATE_INIT:
            sCoordData.sSystem.eState = E_STATE_START_COORDINATOR;
            break;

        case E_STATE_START_COORDINATOR:
            vStartCoordinator();
            sCoordData.sSystem.eState = E_STATE_RUNNING_UART_APP;
            break;

        case E_STATE_RUNNING_UART_APP:
            break;
        }

    }
}

/****************************************************************************
 *
 * NAME: AppWarmStart
 *
 * DESCRIPTION:
 * Entry point for a wake from sleep mode with the memory contents held. We
 * are not using this mode and so should never get here.
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PUBLIC void AppWarmStart(void)
{
    AppColdStart();
}

/****************************************************************************
 *
 * NAME: vWUART_Init
 *
 * DESCRIPTION:
 * Initialises stack and hardware, sets non-default values in the 802.15.4
 * PIB.
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vWUART_Init(void)
{
    vLedInitRfd();
	vLedControl(0,0);
	vLedControl(1,0);

    /* Initialise stack and hardware interfaces. We aren't using callbacks
       at all, just monitoring the upward queues in a loop */
    (void)u32AHI_Init();
    (void)u32AppQApiInit(NULL, NULL, NULL);

    /* Initialise the serial port and rx/tx queues */
    vSerial_Init();

    /* Initialise tick timer to give 10ms interrupt */
    vAHI_TickTimerConfigure(E_AHI_TICK_TIMER_DISABLE);
    vAHI_TickTimerWrite(0);
    vAHI_TickTimerInit(vTickTimerISR);
    vAHI_TickTimerInterval(TICK_PERIOD_COUNT);
    vAHI_TickTimerConfigure(E_AHI_TICK_TIMER_RESTART);
    vAHI_TickTimerIntEnable(TRUE);
}

/****************************************************************************
 *
 * NAME: vProcessEventQueues
 *
 * DESCRIPTION:
 * Check each of the three event queues and process and items found.
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vProcessEventQueues(void)
{
    MAC_MlmeDcfmInd_s *psMlmeInd;
	MAC_McpsDcfmInd_s *psMcpsInd;
    AppQApiHwInd_s    *psAHI_Ind;

    /* Check for anything on the MCPS upward queue */
    do
    {
        psMcpsInd = psAppQApiReadMcpsInd();
        if (psMcpsInd != NULL)
        {
            vProcessIncomingData(psMcpsInd);
            vAppQApiReturnMcpsIndBuffer(psMcpsInd);
        }
    } while (psMcpsInd != NULL);

    /* Check for anything on the MLME upward queue */
    do
    {
        psMlmeInd = psAppQApiReadMlmeInd();
        if (psMlmeInd != NULL)
        {
            vProcessIncomingMlme(psMlmeInd);
            vAppQApiReturnMlmeIndBuffer(psMlmeInd);
        }
    } while (psMlmeInd != NULL);

    /* Check for anything on the AHI upward queue */
    do
    {
        psAHI_Ind = psAppQApiReadHwInd();
        if (psAHI_Ind != NULL)
        {
            vProcessIncomingHwEvent(psAHI_Ind);
            vAppQApiReturnHwIndBuffer(psAHI_Ind);
        }
    } while (psAHI_Ind != NULL);
}

/****************************************************************************
 *
 * NAME: vProcessIncomingMlme
 *
 * DESCRIPTION:
 * Process any incoming managment events from the stack.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  psMlmeInd
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vProcessIncomingMlme(MAC_MlmeDcfmInd_s *psMlmeInd)
{
//    switch(psMlmeInd->u8Type)
//    {
//    case MAC_MLME_IND_ASSOCIATE:
//
//        /* Only allow nodes to associate if network has been started */
//        if (sCoordData.sSystem.eState == E_STATE_RUNNING_UART_APP)
//        {
//            vHandleNodeAssociation(psMlmeInd);
//        }
//        break;
//
//    case MAC_MLME_DCFM_SCAN:
//
//        if (psMlmeInd->uParam.sDcfmScan.u8Status == MAC_ENUM_SUCCESS)
//        {
//            if (psMlmeInd->uParam.sDcfmScan.u8ScanType == MAC_MLME_SCAN_TYPE_ENERGY_DETECT)
//            {
//                if (sCoordData.sSystem.eState == E_STATE_ENERGY_SCANNING)
//                {
//                    vHandleEnergyScanResponse(psMlmeInd);
//                    sCoordData.sSystem.eState = E_STATE_START_COORDINATOR;
//                }
//            }
//        }
//        break;
//    }
}

/****************************************************************************
 *
 * NAME: vProcessIncomingData
 *
 * DESCRIPTION:
 * Process incoming data events from the stack.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  psMcpsInd
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vProcessIncomingData(MAC_McpsDcfmInd_s *psMcpsInd)
{
//    MAC_RxFrameData_s *psFrame;
//    MAC_Addr_s *psAddr;
//    uint16 u16NodeAddr;
//    uint8 i;
//
//    psFrame = &psMcpsInd->uParam.sIndData.sFrame;
//    psAddr = &psFrame->sSrcAddr;
//
//    /* Check that this is a data frame */
//    if (psMcpsInd->u8Type == MAC_MCPS_IND_DATA)
//    {
//        /* Check that data is from UART node */
//        u16NodeAddr = psAddr->uAddr.u16Short;
//
//        if (u16NodeAddr == sCoordData.sNode.asAssocNodes[0].u16ShortAddr)
//        {
//            if (psFrame->au8Sdu[0] == u8RxFrameHandle)
//            {
//                u8RxFrameHandle++;
//
//                /* Copy frame data to serial buffer for output on UART */
//                for (i = 1; i < psFrame->u8SduLength; i++)
//                {
//                    vSerial_TxChar(psFrame->au8Sdu[i]);
//                }
//            }
//            /* Must have missed a frame */
//            else if (psFrame->au8Sdu[0] > u8RxFrameHandle)
//            {
//                u8RxFrameHandle = psFrame->au8Sdu[0] + 1;
//
//                /* Copy frame data to serial buffer for output on UART */
//                for (i = 1; i < psFrame->u8SduLength; i++)
//                {
//                    vSerial_TxChar(psFrame->au8Sdu[i]);
//                }
//            }
//            /* Must be the same frame as last time */
//            else if (psFrame->au8Sdu[0] < u8RxFrameHandle)
//            {
//                /* Dont do anything as we already have the data */
//            }
//        }
//    }
}

/****************************************************************************
 *
 * NAME: vProcessIncomingHwEvent
 *
 * DESCRIPTION:
 * Process any hardware events.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  psAHI_Ind
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vProcessIncomingHwEvent(AppQApiHwInd_s *psAHI_Ind)
{
}

/****************************************************************************
 *
 * NAME: vTickTimerISR
 *
 * DESCRIPTION:
 *
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vTickTimerISR(uint32 u32Device, uint32 u32ItemBitmap)
{
    static uint8 u8ToggleCount;

    /* Flash LED 1 to show we are alive */
    if (u8ToggleCount++ & 0x40)
    {
        vLedControl(0,1);
    }
    else
    {
    	vLedControl(0,0);
    }

    if (sCoordData.sSystem.eState == E_STATE_RUNNING_UART_APP)
    {
        vWUART_RxData();
    }
}

/****************************************************************************
 *
 * NAME: vHandleNodeAssociation
 *
 * DESCRIPTION:
 * Handle request by node to join the network. If the nodes address matches
 * the address of a light switch then it is assumed to be a light switch and
 * is allowed to join the network.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  psMlmeInd
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
//PRIVATE void vHandleNodeAssociation(MAC_MlmeDcfmInd_s *psMlmeInd)
//{
//    MAC_MlmeReqRsp_s   sMlmeReqRsp;
//    MAC_MlmeSyncCfm_s  sMlmeSyncCfm;
//
//    uint16 u16ShortAddress;
//
//    /* Default to PAN access denied */
//    uint8 u8AssocStatus = 2;
//
//    /* Default short address */
//    u16ShortAddress = 0xffff;
//
//    /* Check that the device only wants to use a short address */
//    if (psMlmeInd->uParam.sIndAssociate.u8Capability & 0x80)
//    {
//        if (sCoordData.sNode.u8AssociatedNodes < MAX_UART_NODES)
//        {
//            /* Allocate short address as next in list */
//            u16ShortAddress = UART_NODE_ADDR_BASE + sCoordData.sNode.u8AssociatedNodes;
//
//            /* Store details for future use */
//            sCoordData.sNode.asAssocNodes[sCoordData.sNode.u8AssociatedNodes].u16ShortAddr = u16ShortAddress;
//            sCoordData.sNode.u8AssociatedNodes++;
//
//            /* Assume association succeeded */
//            u8AssocStatus = 0;
//
//            /* Turn on LED to show node has assocaited */
//			vLedControl(1,1);
//        }
//    }
//
//    /* Create association response */
//    sMlmeReqRsp.u8Type = MAC_MLME_RSP_ASSOCIATE;
//    sMlmeReqRsp.u8ParamLength = sizeof(MAC_MlmeRspAssociate_s);
//    sMlmeReqRsp.uParam.sRspAssociate.sDeviceAddr.u32H = psMlmeInd->uParam.sIndAssociate.sDeviceAddr.u32H;
//    sMlmeReqRsp.uParam.sRspAssociate.sDeviceAddr.u32L = psMlmeInd->uParam.sIndAssociate.sDeviceAddr.u32L;
//    sMlmeReqRsp.uParam.sRspAssociate.u16AssocShortAddr = u16ShortAddress;
//    sMlmeReqRsp.uParam.sRspAssociate.u8Status = u8AssocStatus;
//    sMlmeReqRsp.uParam.sRspAssociate.u8SecurityEnable = FALSE;
//
//    /* Send association response. There is no confirmation for an association
//       response, hence no need to check */
//    vAppApiMlmeRequest(&sMlmeReqRsp, &sMlmeSyncCfm);
//}

/****************************************************************************
 *
 * NAME: bStartCoordinator
 *
 * DESCRIPTION:
 * Starts the network by configuring the controller board to act as the PAN
 * coordinator.
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * TRUE if network was started successfully otherwise FALSE
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vStartCoordinator(void)
{
//    /* Structures used to hold data for MLME request and response */
//    MAC_MlmeReqRsp_s   sMlmeReqRsp;
//    MAC_MlmeSyncCfm_s  sMlmeSyncCfm;
//
//    /* Start Pan */
//    sMlmeReqRsp.u8Type = MAC_MLME_REQ_START;
//    sMlmeReqRsp.u8ParamLength = sizeof(MAC_MlmeReqStart_s);
//    sMlmeReqRsp.uParam.sReqStart.u16PanId = PAN_ID;
//    sMlmeReqRsp.uParam.sReqStart.u8Channel = sCoordData.sSystem.u8Channel;
//    sMlmeReqRsp.uParam.sReqStart.u8BeaconOrder = 0x0f; /* No beacons */
//    sMlmeReqRsp.uParam.sReqStart.u8SuperframeOrder = 0x0f;
//    sMlmeReqRsp.uParam.sReqStart.u8PanCoordinator = TRUE;
//    sMlmeReqRsp.uParam.sReqStart.u8BatteryLifeExt = FALSE;
//    sMlmeReqRsp.uParam.sReqStart.u8Realignment = FALSE;
//    sMlmeReqRsp.uParam.sReqStart.u8SecurityEnable = FALSE;
//
//    vAppApiMlmeRequest(&sMlmeReqRsp, &sMlmeSyncCfm);
}

 /****************************************************************************
 *
 * NAME: vTxUARTData
 *
 * DESCRIPTION:
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vWUART_RxData(void)
{
    int16 i16RxChar;
    LCD_vInit();

    i16RxChar = i16Serial_RxChar();

    if (i16RxChar >= 0)
    {
    	LCD_vText(0,(char*)(i16RxChar) );

    }
}

/****************************************************************************
 *
 * NAME: vStartEnergyScan
 *
 * DESCRIPTION:
 * Starts an enery sacn on the channels specified.
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
//PRIVATE void vStartEnergyScan(void)
//{
//    /* Structures used to hold data for MLME request and response */
//    MAC_MlmeReqRsp_s   sMlmeReqRsp;
//    MAC_MlmeSyncCfm_s  sMlmeSyncCfm;
//
//    /* Start energy detect scan */
//    sMlmeReqRsp.u8Type = MAC_MLME_REQ_SCAN;
//    sMlmeReqRsp.u8ParamLength = sizeof(MAC_MlmeReqStart_s);
//    sMlmeReqRsp.uParam.sReqScan.u8ScanType = MAC_MLME_SCAN_TYPE_ENERGY_DETECT;
//    sMlmeReqRsp.uParam.sReqScan.u32ScanChannels = SCAN_CHANNELS;
//    sMlmeReqRsp.uParam.sReqScan.u8ScanDuration = ENERGY_SCAN_DURATION;
//
//    vAppApiMlmeRequest(&sMlmeReqRsp, &sMlmeSyncCfm);
//}

/****************************************************************************
 *
 * NAME: vHandleEnergyScanResponse
 *
 * DESCRIPTION:
 * Selects a channel with low enery content for use by the wireless UART.
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
//PRIVATE void vHandleEnergyScanResponse(MAC_MlmeDcfmInd_s *psMlmeInd)
//{
//    uint8 i = 0;
//    uint8 u8MinEnergy;
//
//	u8MinEnergy = (psMlmeInd->uParam.sDcfmScan.uList.au8EnergyDetect[0]) ;
//
//	/* Search list to find quietest channel */
//    while (i < psMlmeInd->uParam.sDcfmScan.u8ResultListSize)
//    {
//        if ((psMlmeInd->uParam.sDcfmScan.uList.au8EnergyDetect[i]) < u8MinEnergy)
//        {
//			u8MinEnergy = (psMlmeInd->uParam.sDcfmScan.uList.au8EnergyDetect[i]);
//			sCoordData.sSystem.u8Channel = i + CHANNEL_MIN;
//		}
//		i++;
//    }
//}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
