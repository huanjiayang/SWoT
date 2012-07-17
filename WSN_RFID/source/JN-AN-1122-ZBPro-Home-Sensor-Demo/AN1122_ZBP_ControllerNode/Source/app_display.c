/*****************************************************************************
 *
 * MODULE:             Home Sensor Demo
 *
 * COMPONENT:          app_display.c
 *
 * AUTHOR:             MRW
 *
 * DESCRIPTION:        Manage LCD display on a coordinator
 *                     Three display modes are present - splash screen, main
 *                     sensor graph and node details screen
 *
 * $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Application_Notes/JN-AN-1122-ZBPro-Home-Sensor-Demo/Tags/ZBPRO-HOME-SENSOR-DEMO_1v7_RC4/AN1122_ZBP_ControllerNode/Source/app_display.c $
 *
 * $Revision: 29687 $
 *
 * $LastChangedBy: nxp29741 $
 *
 * $LastChangedDate: 2011-03-02 14:55:12 +0000 (Wed, 02 Mar 2011) $
 *
 * $Id: app_display.c 29687 2011-03-02 14:55:12Z nxp29741 $
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

#include "jendefs.h"
#include "LcdDriver.h"
#include "jenniclogo.h"
#include "utilities.h"
#include "os.h"
#include "os_gen.h"
#include "dbg.h"
#include "app_home_sensor_demo.h"
#include "app_timer_driver.h"
#include "app_controller_node.h"
#include "app_log.h"
#include "app_display.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifndef TRACE_ASSRT
#define TRACE_ASSERT FALSE
#endif

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void vBuildSplashScreen(void);
PRIVATE void vUpdateSplashScreen(void);
PRIVATE void vBuildNetworkStartupScreen(void);
PRIVATE void vBuildNetworkScreen(void);
PRIVATE void vUpdateNetworkScreen(APP_teSensor eSensor);
PRIVATE void vBuildNodeScreen(uint8 u8Node);
PRIVATE void vUpdateNodeScreen(uint8 u8Node);
PRIVATE void vUpdateSensor(APP_tsLogNodeHistory *psNodeHistory,
                           APP_teSensor eSensor,
                           uint8 u8Row,
                           uint8 u8Col,
                           bool bShowInfo);
PRIVATE void vDrawGraph(uint8 *pu8GraphData, uint8 u8StartCol, uint8 u8StartRow);
PRIVATE void vValToDec(char *pcOutString, uint8 u8Value, char *pcLabel);
PRIVATE void vStringCopy(char *pcFrom, char *pcTo);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
/* only exported for inlined functions */
APP_teDisplayState app_eDisplayState;
APP_teSensor app_eCurrentSensor;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

PRIVATE volatile bool_t s_UpdatePending = FALSE;
/* Row and column positions of info fields on LCD */
PRIVATE const uint8 s_au8NodeLcdRow[APP_HSDEMO_NUM_SENSORNODES_POSITIONS] =
    { 0, 0, 3, 3 };
PRIVATE const uint8 s_au8NodeLcdCol[APP_HSDEMO_NUM_SENSORNODES_POSITIONS] =
    { 0, 64, 0, 64 };
PRIVATE uint8 s_u8CurrentNode = 0;

PRIVATE const char *s_apcNodeNameList[APP_HSDEMO_NUM_SENSORNODES] =
    { "Hall", "Bedroom", "Lounge", "Bathroom" };

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: APP_vDisplayInitialise
 *
 * DESCRIPTION:
 * Initialises the LCD display
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vDisplayInitialise(void)
{
    /* reset the LCD */
    vLcdResetDefault();
}

/****************************************************************************
 *
 * NAME: APP_u8GetCurrentNode
 *
 * DESCRIPTION:
 * Returns ID of current node
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC uint8 APP_u8GetCurrentNode(void)
{
    return s_u8CurrentNode;
}
/****************************************************************************
 *
 * NAME: APP_vDisplaySetSensor
 *
 * DESCRIPTION:
 * Cycles the displayed node.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vDisplayCycleNode(void)
{
    s_u8CurrentNode++;
    if (s_u8CurrentNode == APP_u8ControllerNodeNumDataSensors())
    {
        s_u8CurrentNode = 0;
    }
}

/****************************************************************************
 *
 * NAME: APP_vDisplayUpdate
 *
 * DESCRIPTION:
 * Redraws the display
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/

PUBLIC void APP_vDisplayUpdate(void)
{
    if (!s_UpdatePending)
    {
        s_UpdatePending = TRUE;
        OS_eActivateTask(APP_taskDisplayUpdate);
    }
}

/****************************************************************************
 *
 * NAME: APP_DisplayUpdateTask
 *
 * DESCRIPTION:
 * Redraws the display
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
OS_TASK(APP_taskDisplayUpdate)
{
    static APP_teDisplayState s_ePrevDisplayState = APP_E_DISPLAY_STATE_INIT;
    bool bDisplayStateChanged = FALSE;

    /*
     * ensure the scrolling text timer is not running if we are not in the
     * splash screen state
     */
    if (APP_E_DISPLAY_STATE_SPLASH != app_eDisplayState && OS_E_SWTIMER_STOPPED
            != OS_eGetSWTimerStatus(APP_tmrDisplaySplashScreen))
    {
        OS_eStopSWTimer(APP_tmrDisplaySplashScreen);
    }

//    if (APP_E_DISPLAY_STATE_NETWORK_STARTUP != app_eDisplayState && OS_E_SWTIMER_STOPPED
//            != OS_eGetSWTimerStatus(APP_tmrFormingNetwork))
//    {
//        OS_eStopSWTimer(APP_tmrFormingNetwork);
//    }

    /* check to see if the state of the display is being changed */
    if (s_ePrevDisplayState != app_eDisplayState)
    {
        bDisplayStateChanged = TRUE;
        DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eEnterCriticalSection(mutexMEDIA));
        vLcdClear();
        DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eExitCriticalSection(mutexMEDIA));
        s_ePrevDisplayState = app_eDisplayState;
    }

    /* update the display ... */
    if (APP_E_DISPLAY_STATE_SPLASH == app_eDisplayState)
    {
        if (bDisplayStateChanged)
        {
            vBuildSplashScreen();
        }
        vUpdateSplashScreen();
    }
    else if (APP_E_DISPLAY_STATE_NETWORK_STARTUP == app_eDisplayState)
    {
        if (bDisplayStateChanged)
        {
            vBuildNetworkStartupScreen();
        }
    }
    else if (APP_E_DISPLAY_STATE_NETWORK == app_eDisplayState)
    {
        if (bDisplayStateChanged)
        {
            vBuildNetworkScreen();
        }
        vUpdateNetworkScreen(app_eCurrentSensor);
    }
    else if (APP_E_DISPLAY_STATE_NODE == app_eDisplayState)
    {
        if (bDisplayStateChanged)
        {
            vBuildNodeScreen(s_u8CurrentNode);
        }
        vUpdateNodeScreen(s_u8CurrentNode);
    }

    /* Refresh LCD with newly drawn screen */
    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eEnterCriticalSection(mutexMEDIA));
    vLcdRefreshAll();
    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eExitCriticalSection(mutexMEDIA));

    /* set flag to indicate we have completed the requested update */
    s_UpdatePending = FALSE;
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: vBuildSplashScreen
 *
 * DESCRIPTION:
 * Updates the Splash screen, when it first appears or when the user
 * changes the channel number.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vBuildSplashScreen(void)
{
    /* Draw the splash screen */
    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eEnterCriticalSection(mutexMEDIA));
    vLcdWriteBitmap((tsBitmap *) &sJennicLogo, 0, 0);
    vLcdWriteText("Ch", 7, 0);
    vLcdWriteText("\\", 7, 47);
    vLcdWriteText("]", 7, 74);
    vLcdWriteText("Done", 7, 103);
    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eExitCriticalSection(mutexMEDIA));

    vUpdateSplashScreen();

    OS_eStartSWTimer(APP_tmrDisplaySplashScreen, APP_TIME_MS(10), NULL);
}

/****************************************************************************
 *
 * NAME: APP_DisplayScrollText
 *
 * DESCRIPTION:
 * Update the scrolling title text to the next display position. Task is
 * activated by timer APP_DisplaySplashScreenTimer.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
OS_TASK(APP_taskDisplayScroll)
{
    static const char s_szTitle[] = "ZBPro Home Sensor Demo ]]] ";
    static const char s_au8Pixel[] =
        { 6, 6, 6, 6, 6, 4, 6, 6, 6, 6, 4, 6, 6, 6, 6, 6, 6, 4, 6, 6, 6, 6, 4, 6,
        6, 6, 4 };
    static uint8 s_u8CurPixPos = 0;
    static uint8 s_u8CurCharPos = 0;

    if (APP_E_DISPLAY_STATE_SPLASH == app_eDisplayState)
    {
        char szDisplayLine[24];
        unsigned int i;

        if (OS_E_SWTIMER_RUNNING
                != OS_eGetSWTimerStatus(APP_tmrDisplaySplashScreen))
        {
            OS_eContinueSWTimer(APP_tmrDisplaySplashScreen,
                                APP_TIME_MS(150),
                                NULL);
        }

        for (i = 0; i < 22; i++)
        {
            unsigned int p = s_u8CurCharPos + i;

            if (p >= (sizeof(s_szTitle) - 1))
            {
                p -= sizeof(s_szTitle) - 1;
            }

            szDisplayLine[i] = s_szTitle[p];
        }
        szDisplayLine[i] = 0;

        DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eEnterCriticalSection(mutexMEDIA));
        vLcdWriteText(szDisplayLine, 4, 5 - s_u8CurPixPos);
        vLcdWriteText(" ", 4, 0);
        vLcdWriteText(" ", 4, 124);
        vLcdRefreshArea(0, 4, 128, 1);
        DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eExitCriticalSection(mutexMEDIA));

        s_u8CurPixPos++;
        if (s_u8CurPixPos == s_au8Pixel[s_u8CurCharPos])
        {
            s_u8CurCharPos++;
            if ((sizeof(s_szTitle) - 1) == s_u8CurCharPos)
            {
                s_u8CurCharPos = 0;
            }
            s_u8CurPixPos = 0;
        }
    }

    /* Indicate app is fine when displaying splash screen */
    APP_bAppHealthy = TRUE;
}

/****************************************************************************
 *
 * NAME: vUpdateSplashScreen
 *
 * DESCRIPTION:
 * Updates the Splash screen, when it first appears or when the user
 * changes the channel number.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vUpdateSplashScreen(void)
{
    char acString[5];

    vValToDec(acString, APP_u8ControllerNodeGetCurRadioChan(), "  ");
    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eEnterCriticalSection(mutexMEDIA));
    vLcdWriteText(acString, 7, 16);
    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eExitCriticalSection(mutexMEDIA));
}

/****************************************************************************
 *
 * NAME: vBuildNetworkScreen
 *
 * DESCRIPTION:
 * Creates the Network screen. Depending on how the GUI has been configured
 * it may want to display up to 3 or up to 4 nodes simultaneuously. Also, it
 * only shows nodes that have successfully associated. To achieve this, it
 * makes use of an array of the four display positions on the screen, and
 * loops through this to position each node in the correct position.
 *
 * This function only draws the text required, then uses the related update
 * function to display the actual data and to refresh the LCD.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  eSensor         R   Sensor to display
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vBuildNetworkStartupScreen(void)
{
    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eEnterCriticalSection(mutexMEDIA));
    vLcdWriteText("Forming ZBPro Network", 3, 5);
    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eExitCriticalSection(mutexMEDIA));
}

/****************************************************************************
 *
 * NAME: vBuildNetworkScreen
 *
 * DESCRIPTION:
 * Creates the Network screen. Depending on how the GUI has been configured
 * it may want to display up to 3 or up to 4 nodes simultaneuously. Also, it
 * only shows nodes that have successfully associated. To achieve this, it
 * makes use of an array of the four display positions on the screen, and
 * loops through this to position each node in the correct position.
 *
 * This function only draws the text required, then uses the related update
 * function to display the actual data and to refresh the LCD.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  eSensor         R   Sensor to display
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vBuildNetworkScreen(void)
{
    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eEnterCriticalSection(mutexMEDIA));

    if (APP_bGetPermitJoining())
    {
        vLcdWriteText("Joining on  ", 6, 0);
    }
    else
    {
        vLcdWriteText("Joining off", 6, 0);
    }

    /* Hot buttons at bottom of screen */
    vLcdWriteText("Node", 7, 0);

    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eExitCriticalSection(mutexMEDIA));

    vUpdateNetworkScreen(app_eCurrentSensor);
}

/****************************************************************************
 *
 * NAME: vUpdateNetworkScreen
 *
 * DESCRIPTION:
 * Draws the graphs and values for the Network screen. See the description
 * for vBuildNetworkScreen for an explanation of the positioning of elements
 * on the display.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  eSensor         R   Sensor to display
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vUpdateNetworkScreen(APP_teSensor eSensor)
{
    static const char *s_apcSensorLabel[APP_HSDEMO_NUM_SENSOR_TYPES] =
    { "Temp", "Humidity", "Light" };
    static const uint8 s_au8LabelPos[APP_HSDEMO_NUM_SENSOR_TYPES] =
    { 29, 58, 102 };
    uint8 u8Node;

    /* Show labels */
    uint8 u8NumDataSensors = APP_u8ControllerNodeNumDataSensors();

    if (u8NumDataSensors == 0)
    {
        DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eEnterCriticalSection(mutexMEDIA));
        vLcdWriteText("No sensors detected", 3, 0);
        DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eExitCriticalSection(mutexMEDIA));
    }
    else
    {
        DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eEnterCriticalSection(mutexMEDIA));
        vLcdWriteText("                           ", 3, 0);
        DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eExitCriticalSection(mutexMEDIA));

        for (u8Node = 0; u8Node < u8NumDataSensors; u8Node++)
        {
            uint8 u8Row = s_au8NodeLcdRow[u8Node] + 1;
            uint8 u8Col = s_au8NodeLcdCol[u8Node];

            DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eEnterCriticalSection(mutexMEDIA));
            vLcdWriteText((char *) s_apcNodeNameList[u8Node],
                          s_au8NodeLcdRow[u8Node],
                          s_au8NodeLcdCol[u8Node]);
            DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eExitCriticalSection(mutexMEDIA));
            vUpdateSensor(APP_psLogGetSensorNodeHistory(u8Node),
                          eSensor,
                          u8Row,
                          u8Col,
                          TRUE);
        }
    }

    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eEnterCriticalSection(mutexMEDIA));
    if (APP_bGetPermitJoining())
    {
        vLcdWriteText("Joining on  ", 6, 0);
    }
    else
    {
        vLcdWriteText("Joining off", 6, 0);
    }

    unsigned int iIndex;
    for (iIndex = 0; iIndex < APP_HSDEMO_NUM_SENSOR_TYPES; iIndex++)
    {
        vLcdWriteText((char *) s_apcSensorLabel[iIndex],
                      7,
                      s_au8LabelPos[iIndex]);
    }
    vLcdWriteInvertedText((char *) s_apcSensorLabel[eSensor],
                          7,
                          s_au8LabelPos[eSensor]);

    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eExitCriticalSection(mutexMEDIA));
}

/****************************************************************************
 *
 * NAME: vBuildNodeScreen
 *
 * DESCRIPTION:
 * Builds the text to appear on a Node screen, then uses the update function
 * to populate it with data.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  u8Node          R   Node to display
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vBuildNodeScreen(uint8 u8Node)
{
    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eEnterCriticalSection(mutexMEDIA));

    vLcdWriteText("Humidity", 0, 64);
    vLcdWriteText("Temp", 3, 0);
    vLcdWriteText("Light", 3, 64);
    vLcdWriteText("Node", 7, 0);
    vLcdWriteText("Back", 7, 40);
    vLcdWriteText("Off", 7, 75);
    vLcdWriteText("On", 7, 115);

    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eExitCriticalSection(mutexMEDIA));

    vUpdateNodeScreen(u8Node);
}

/****************************************************************************
 *
 * NAME: vUpdateNodeScreen
 *
 * DESCRIPTION:
 * Draws the three sensor graphs for a node.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  u8Node          R   Node to display
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vUpdateNodeScreen(uint8 u8Node)
{
    APP_tsLogNodeHistory *psNodeHistory;
    char acString[8];

    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eEnterCriticalSection(mutexMEDIA));

    vLcdWriteText("                ", 0, 0);
    vLcdWriteText((char *) s_apcNodeNameList[u8Node], 0, 0);

    psNodeHistory = APP_psLogGetSensorNodeHistory(u8Node);

    /* Status */
    vValToDec(acString, psNodeHistory->u8FramesMissed - 1, "    ");
    vLcdWriteText(acString, 1, 20);

    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eExitCriticalSection(mutexMEDIA));

    /* Update graphs, alarms and values */
    vUpdateSensor(psNodeHistory, APP_E_SENSOR_TEMP, 4, 0, FALSE);
    vUpdateSensor(psNodeHistory, APP_E_SENSOR_HTS, 1, 64, FALSE);
    vUpdateSensor(psNodeHistory, APP_E_SENSOR_ALS, 4, 64, FALSE);
}

/****************************************************************************
 *
 * NAME: vLcdUpdateElement
 *
 * DESCRIPTION:
 * Draws the graph and text for a single sensor for a single node. The text
 * includes alarm indications if the sensor value exceeds user specified
 * limits.
 *
 * PARAMETERS:  Name                RW  Usage
 *              psNodeElementData   R   Pointer to data for node
 *              u8Row               R   Character row to display on
 *              u8Col               R   Pixel column to display on
 *              boShowInfo          R   TRUE to show missed frames indication
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vUpdateSensor(APP_tsLogNodeHistory *psNodeHistory,
                           APP_teSensor eSensor,
                           uint8 u8Row,
                           uint8 u8Col,
                           bool bShowInfo)
{
    char acString[10];
    uint8 u8NowValue;
    APP_tsLogSensorData *psSensorData =
            &(psNodeHistory->asNodeSensorData[eSensor]);

    u8NowValue = psSensorData->u8NowValue;

    switch (eSensor)
    {
        case APP_E_SENSOR_TEMP:
            vValToDec(acString, u8NowValue, "[C");
            break;

        case APP_E_SENSOR_HTS:
            vValToDec(acString, u8NowValue, "%");
            break;

        case APP_E_SENSOR_ALS:
            /* This is a light sensor so display symbol */
            acString[0] = '&' + u8NowValue;
            acString[1] = '\0';
            break;

        default:
            break;
    }

    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eEnterCriticalSection(mutexMEDIA));

    vLcdWriteText("       ", u8Row, u8Col);
    vLcdWriteText(acString, u8Row, u8Col);

    if (bShowInfo)
    {
        vLcdWriteText("       ", (uint8) (u8Row + 1), u8Col);
        vUTIL_NumToString(psNodeHistory->u16ShortAddr, acString);
        vLcdWriteText(&acString[4], (uint8) (u8Row + 1), u8Col);
    }

    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eExitCriticalSection(mutexMEDIA));

    /* Draw graph */
    vDrawGraph(psSensorData->au8GraphData, (uint8) (u8Col + 27), u8Row);
}

/****************************************************************************
 *
 * NAME: vDrawGraph
 *
 * DESCRIPTION:
 * Creates a bitmap from an array of values. Each value is represented by a
 * column on the graph, and a lookup table is used to translate each value
 * (assumed to be in the range 0 to 13) to the data required for the bitmap.
 * Finally, the bitmap is displayed via a board API.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  pu8GraphData    R   Array of 32 elements of graph data
 *                  u8StartCol      R   First column of bitmap
 *                  u8StartRow      R   Top character row of bitmap
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vDrawGraph(uint8 *pu8GraphData, uint8 u8StartCol, uint8 u8StartRow)
{
    static const uint16 au16LineData[14] =
    { 0x4000, 0x6000, 0x7000, 0x7800, 0x7c00, 0x7e00, 0x7f00, 0x7f80, 0x7fc0,
        0x7fe0, 0x7ff0, 0x7ff8, 0x7ffc, 0x7ffe };
    uint8 au8GraphBitmap[66];
    const tsBitmap sGraphBitmap =
    { au8GraphBitmap, 33, 2 };
    int i;
    uint16 u16LineData;
    uint8 u8DataPos = APP_u8LogGetDataStartPos();

    /* Draw y axis */
    au8GraphBitmap[0] = 0xfe;
    au8GraphBitmap[33] = 0x7f;

    /* Fill in data */
    for (i = 0; i < APP_LOG_HISTORY_LEN; i += 1)
    {
        u16LineData = au16LineData[pu8GraphData[u8DataPos]];

        au8GraphBitmap[i * 2 + 1] = (uint8) (u16LineData & 0xff);
        au8GraphBitmap[i * 2 + 2] = (uint8) (u16LineData & 0xff);
        au8GraphBitmap[i * 2 + 34] = (uint8) (u16LineData >> 8);
        au8GraphBitmap[i * 2 + 35] = (uint8) (u16LineData >> 8);

        /* Increment data point */
        u8DataPos = (u8DataPos + 1) & (APP_LOG_HISTORY_LEN - 1);
    }

    /* Write bitmap to shadow memory */
    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eEnterCriticalSection(mutexMEDIA));
    vLcdWriteBitmap((tsBitmap *) &sGraphBitmap, u8StartCol, u8StartRow);
    DBG_vAssert(TRACE_ASSERT, OS_E_OK == OS_eExitCriticalSection(mutexMEDIA));
}

/****************************************************************************
 *
 * NAME: vValToDec
 *
 * DESCRIPTION:
 * Converts an 8-bit value to a string of the textual decimal representation.
 * Adds a text string after the text.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  pcOutString     R   Location for new string
 *                  u8Value         R   Value to convert
 *                  pcLabel         R   Label to append to string
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vValToDec(char *pcOutString, uint8 u8Value, char *pcLabel)
{
    static const uint8 au8Digits[3] =
    { 100, 10, 1 };
    uint8 u8Digit;
    uint8 u8DigitIndex;
    uint8 u8Count;
    bool_t boPreviousDigitPrinted = FALSE;

    for (u8DigitIndex = 0; u8DigitIndex < 3; u8DigitIndex++)
    {
        u8Count = 0;
        u8Digit = au8Digits[u8DigitIndex];
        while (u8Value >= u8Digit)
        {
            u8Value -= u8Digit;
            u8Count++;
        }

        if ((u8Count != 0) || (boPreviousDigitPrinted == TRUE) || (u8DigitIndex
                == 2))
        {
            *pcOutString = '0' + u8Count;
            boPreviousDigitPrinted = TRUE;
            pcOutString++;
        }
    }

    vStringCopy(pcLabel, pcOutString);
}

/****************************************************************************
 *
 * NAME: vStringCopy
 *
 * DESCRIPTION:
 * Simple string copy as standard libraries not available.
 *
 * PARAMETERS:      Name    RW  Usage
 *                  pcFrom  R   Pointer to string to copy
 *                  pcTo    W   Pointer to store for new string
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vStringCopy(char *pcFrom, char *pcTo)
{
    while (*pcFrom != '\0')
    {
        *pcTo = *pcFrom;
        pcTo++;
        pcFrom++;
    }
    *pcTo = '\0';
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
