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
 * $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Application_Notes/JN-AN-1122-ZBPro-Home-Sensor-Demo/Tags/ZBPRO-HOME-SENSOR-DEMO_1v7_RC4/AN1122_ZBP_SensorNode/Source/app_sample.c $
 *
 * $Revision: 27498 $
 *
 * $LastChangedBy: fbhai $
 *
 * $LastChangedDate: 2010-11-08 14:35:22 +0000 (Mon, 08 Nov 2010) $
 *
 * $Id: app_sample.c 27498 2010-11-08 14:35:22Z fbhai $
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
#include "ALSdriver.h"
#include "HTSdriver.h"
#include "AppHardwareApi.h"
#include "app_buttons.h"
#include "os.h"
#include "os_gen.h"
#include "pdum_nwk.h"
#include "pdum_apl.h"
#include "pdum_gen.h"
#include "dbg.h"
#include "zps_gen.h"
#include "zps_apl.h"
#include "zps_apl_af.h"
#include "appapi.h"
#include "app_home_sensor_demo.h"
#include "app_timer_driver.h"
#include "app_sample.h"
#include "app_sensor_node.h"
#include "app_led.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifndef TRACE_APP_SAMPLE
#define TRACE_APP_SAMPLE    FALSE
#endif

#define u8FindMin(v1, v2) ( ( (v1) < (v2) ) ? (v1) : (v2) )

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void vSendSensorData(PDUM_thAPduInstance hAPduInst);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern bool_t APP_bAppHealthy;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: APP_vSampleInitialise
 *
 * DESCRIPTION:
 * Initialises sampling of the sensors
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vInitialiseSample(void)
{
    DBG_vPrintf(TRACE_APP_SAMPLE, "APP: Initialising Sampling\r\n");

    /* Set up peripheral hardware */
    vALSreset();
    vHTSreset();

    /* Set the edge detection for falling edges */
    vAHI_DioInterruptEdge(0, HTS_DATA_DIO_BIT_MASK);

    /* Disable interrupts on selected edge for now */
    vAHI_DioInterruptEnable(0, HTS_DATA_DIO_BIT_MASK);

    /* Start ALS now: it automatically keeps re-sampling after this */
    vALSstartReadChannel(0);
}

/****************************************************************************
 *
 * NAME: APP_SampleSensorsTask
 *
 * DESCRIPTION:
 * Samples the sensors and sends the data to the controller node
 *
 ****************************************************************************/
OS_TASK(APP_taskSampleSensors)
{
    PRIVATE enum
    {
        E_SAMPLING_START,
        E_SAMPLING_TEMP,
        E_SAMPLING_HUMIDITY,
        E_SAMPLING_LIGHT,
    } eSampleState = E_SAMPLING_START;

    PRIVATE PDUM_thAPduInstance s_hAPduInst = PDUM_INVALID_HANDLE;

    switch (eSampleState)
    {
        case E_SAMPLING_START:
        {
            DBG_vPrintf(TRACE_APP_SAMPLE, "APP: Starting read of Temp sensor.\n");

            s_hAPduInst = PDUM_hAPduAllocateAPduInstance(apduSensorData);
            if (PDUM_INVALID_HANDLE != s_hAPduInst)
            {
                /* Start to read temperature */
                vHTSstartReadTemp();
                vAHI_DioInterruptEnable(HTS_DATA_DIO_BIT_MASK, 0);
                eSampleState = E_SAMPLING_TEMP;
            }
            else
            {
                DBG_vPrintf(TRACE_APP_SAMPLE, "APP: No APDUs left to send data\n");
            }
            break;
        }

        case E_SAMPLING_TEMP:
        {
            /* Read temperature, 0-52 are acceptable */
            uint16 u16Temp = u16HTSreadTempResult();

            PDUM_u16APduInstanceWriteNBO(s_hAPduInst,
                                         APDU_SENSORDATA_TEMP,
                                         "b",
                                         u8FindMin(u16Temp, 52));

            DBG_vPrintf(TRACE_APP_SAMPLE, "APP: Temp Read complete. Value = %d\r\n", u16Temp);

            /* Start to read humidity */
            DBG_vPrintf(TRACE_APP_SAMPLE, "APP: Staring read of Humidity.\r\n");

            vHTSstartReadHumidity();
            vAHI_DioInterruptEnable(HTS_DATA_DIO_BIT_MASK, 0);
            eSampleState = E_SAMPLING_HUMIDITY;
            break;
        }

        case E_SAMPLING_HUMIDITY:
        {
            /* Read humidity, 0-104 are acceptable */
            uint16 u16Humidity = u16HTSreadHumidityResult();
            PDUM_u16APduInstanceWriteNBO(s_hAPduInst,
                                         APDU_SENSORDATA_HUMIDITY,
                                         "b",
                                         u8FindMin(u16Humidity, 104));
            DBG_vPrintf(TRACE_APP_SAMPLE, "APP: Humidity Read complete. Value = %d\r\n", u16Humidity);

            eSampleState = E_SAMPLING_LIGHT;
            OS_eActivateTask(APP_taskSampleSensors);
            break;
        }

        case E_SAMPLING_LIGHT:
        {
            /* Read light level, adjust to range 0-6 in a slightly non-linear way */
            DBG_vPrintf(TRACE_APP_SAMPLE, "APP: Light Sensor Read\r\n");
            uint16 u16Light = u16ALSreadChannelResult();
            DBG_vPrintf(TRACE_APP_SAMPLE, "APP: After Light Sensor Read\r\n");
            DBG_vPrintf(TRACE_APP_SAMPLE, "APP: Light Read complete. Value = %d\r\n", u16Light);
            PDUM_u16APduInstanceWriteNBO(s_hAPduInst,
                                         APDU_SENSORDATA_LIGHT,
                                         "b",
                                         u8FindMin((uint8)(u16Light >> 6), 6));

            /* send the sampled data */
            vSendSensorData(s_hAPduInst);

            /* sample again in 1 second */
            OS_eContinueSWTimer(APP_tmrSampleSensors, SAMPLE_TIME, NULL);

            eSampleState = E_SAMPLING_START;
            break;
        }
    }
}

/****************************************************************************
 *
 * NAME: APP_taskSensorEvent
 *
 * DESCRIPTION:
 * Receives and messages associated with the Sensor Data endpoint.
 * Specifically, receives the APS confirm, and turns LED1 off.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
OS_TASK(APP_taskSensorEvent)
{
    ZPS_tsAfEvent sSensorEvent;

    /* collect the message for the sensor data endpoint */
    if (OS_E_OK == OS_eCollectMessage(APP_msgSensorEvent, &sSensorEvent))
    {
        /* Ensure that the message contains a data confirmation */
        if (ZPS_EVENT_APS_DATA_CONFIRM == sSensorEvent.eType)
        {
            /* Turn off LED1 to indicate transmission of sensor data is complete */
            APP_vLedSet(1, FALSE);
            /* basic check on correct app functionality - if sensor task does not regularly receive
             * aps confirm then something must be wrong with transmission to controller.
             * APP_bAppHealthy is a flag which is used in app_start to kick the watchdog.
             */
            APP_bAppHealthy =  TRUE;
        }
    }
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: vSendData
 *
 * DESCRIPTION:
 * Generates and sends a frame consisting of 4 KVP transactions, for the four
 * values that are passed to the controller. The destination address is fixed
 * as 0x0000 (coordinator) and the endpoint is also fixed.
 *
 * PARAMETERS: Name             RW  Usage
 *             u8LightValue     R   Value from light sensor
 *             u8TempValue      R   Value from temperature sensor
 *             u8HumidityValue  R   Value from humidity sensor
 *             u8Switch         R   Switch value (0 or 1)
 *
 ****************************************************************************/
PRIVATE void vSendSensorData(PDUM_thAPduInstance hAPduInst)
{
    uint64 u64ExtAddr;
    uint8 u8Status;
    uint8 u8SeqNum;

    /* Turn on LED1 to indicate transmission of sensor data. */
    APP_vLedSet(1, TRUE);

    u8SeqNum = APP_u8GetSequenceNumber(TRUE);

    if (PDUM_INVALID_HANDLE != hAPduInst)
    {
        /* Get our extended address */
        u64ExtAddr = ZPS_u64NwkNibGetExtAddr(ZPS_pvAplZdoGetNwkHandle());

        (void) PDUM_u16APduInstanceWriteNBO(hAPduInst,
                                            APDU_SENSORDATA_LENGTH,
                                            "bl",
                                            13,
                                            u64ExtAddr);
        PDUM_eAPduInstanceSetPayloadSize(hAPduInst, 13);

        u8Status = ZPS_eAplAfUnicastDataReq(hAPduInst,
                                            JENNIC_HSD_SENSOR_DATA_CLUSTER_ID,
                                            AN1122_ZBP_SENSORNODE_SENSOR_ENDPOINT,
                                            AN1122_ZBP_CONTROLLERNODE_SENSOR_ENDPOINT,
                                            0x0000 /* Coordinator */,
                                            ZPS_E_APL_AF_UNSECURE,
                                            0 /* default radius */,
                                            &u8SeqNum);
    }
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
