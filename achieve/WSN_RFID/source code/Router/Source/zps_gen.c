/****************************************************************************
 *
 *                 THIS IS A GENERATED FILE. DO NOT EDIT!
 *
 * MODULE:         ZPS
 *
 * COMPONENT:      zps_gen.c
 *
 * DATE:           Fri May 18 04:20:28 2012
 *
 * AUTHOR:         Jennic Zigbee Protocol Stack Configuration Tool
 *
 * DESCRIPTION:    ZPS definitions
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
#include <pdm.h>
#include <pdum_gen.h>
#include <os_gen.h>
#include <zps_gen.h>

#include "zps_apl.h"
#include "zps_apl_aib.h"
#include "zps_apl_af.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/*** ZDP Context **************************************************/

typedef struct {
    uint8 u8ZdpSeqNum;
} zps_tsZdpContext;

/*** ZDO Context **************************************************/

typedef bool (*zps_tprAplZdoServer)(void *psApl, void *pvServerContext, ZPS_tsAfEvent *psZdoServerEvent);

typedef struct {
    zps_tprAplZdoServer prServer;
    void *pvServerContext;
} zps_tsAplZdoServer;

typedef struct
{
    uint8               au8Key[ZPS_NWK_KEY_LENGTH];
    uint8               u8KeySeqNum;
    uint8               u8KeyType;
} zps_tsAplZdoInitSecKey;

typedef struct {
    uint8 eNetworkState;
    uint8 eZdoDeviceType;
    uint8 eNwkKeyState;
    uint8 u8PermitJoinTime;
    uint8 u8StackProfile;
    uint8 u8ZigbeeVersion;
    uint8 u8ScanDuration;
    bool_t bLookupTCAddr;
    zps_tsAplZdoServer *psZdoServers;
    void (*prvZdoServersInit)(void);
    ZPS_tsTsvTimer sAuthenticationTimer;
    ZPS_tsTsvTimer sAuthenticationPollTimer;
    bool_t bTcOverride;
    uint8 u8NumPollFailures;
    uint8 u8MaxNumPollFailures;
    bool_t bAutomaticJoin;
    bool_t bSecurityDisabled;
    zps_tsAplZdoInitSecKey *psInitSecKey;
} zps_tsZdoContext;

/*** Trust Center Context *****************************************/

typedef struct
{
    uint64 u64DeviceAddress;
    uint8  u8Status;
} zps_tsAplTCDeviceTable;

typedef struct
{
    zps_tsAplTCDeviceTable *asTCDeviceTable;
    uint32  u32SizeOfTCDeviceTable;
} zps_tsAplTCib;

typedef struct {
    uint64 u64InitiatorAddr;
    uint64 u64ResponderAddr;
    ZPS_tsTsvTimer sTimer;
} zps_tsRequestKeyRequests;

typedef struct
{
    void (*prvTrustCenterInit)(void*);
    void (*prvTrustCenterUpdateDevice)(void*, uint64, uint64, uint8, uint16);
    void (*prvTrustCenterRequestKey)(void*, uint64, uint8, uint64);
    zps_tsAplTCib sTCib;
    zps_tsRequestKeyRequests *psRequestKeyReqs;
    uint32 u32ReqKeyTimeout;
    uint8 u8MaxNumSimulRequestKeyReqs;
} zps_tsTrustCenterContext;

/*** AF Context ***************************************************/

typedef struct zps_tsAplAfDynamicContext zps_tsAplAfDynamicContext;

typedef struct _zps_tsAplAfSimpleDescCont
{
    OS_thMessage hMsg;
    ZPS_tsAplAfSimpleDescriptor sSimpleDesc;
    PDUM_thAPdu *phAPduInClusters;
    bool_t bEnabled;
} zps_tsAplAfSimpleDescCont;

typedef struct {
    zps_tsAplAfDynamicContext *psDynamicContext;
    ZPS_tsAplAfNodeDescriptor *psNodeDescriptor;
    ZPS_tsAplAfNodePowerDescriptor *psNodePowerDescriptor;
    uint32 u32NumSimpleDescriptors;
    zps_tsAplAfSimpleDescCont *psSimpleDescConts;
    ZPS_tsAplAfUserDescriptor *psUserDescriptor;
    OS_thMessage hOverrunMsg;   
    uint8 zcp_u8FragApsAckValue;
    uint8 zcp_u8FragBlockControl;
} zps_tsAfContext;

/*** APS Context **************************************************/

typedef struct
{
    uint8 u8Type;
    uint8 u8ParamLength;
} ZPS_tsAplApsmeDcfmIndHdr;

typedef struct
{
    uint8 u8Type;
    uint8 u8ParamLength;
} ZPS_tsAplApsdeDcfmIndHdr;

typedef struct {
    ZPS_tuAddress uDstAddr;
    PDUM_thAPduInstance hAPduInst;
    uint8 *pu8SeqCounter;
    uint16 u16ProfileId;
    uint16 u16ClusterId;
    uint8 u8DstEndpoint;
    uint8 u8SrcEndpoint;
    uint8 u8Radius;
    uint8 eDstAddrMode;
    uint8 eTxOptions;
} ZPS_tsAplApsdeReqData;

typedef union
{
    ZPS_tsAplApsdeReqData  sReqData;
} ZPS_tuAplApsdeReqRspParam;

typedef struct
{
    uint8                 u8Type;
    uint8                 u8ParamLength;
    uint16                u16Pad;
    ZPS_tuAplApsdeReqRspParam uParam;
} ZPS_tsAplApsdeReqRsp;

typedef struct
{
    struct {
        uint32 u6Reserved       : 6;
        uint32 u2Fragmentation  : 2;
        uint32 u24Padding       : 24;
    } sEFC;
    uint8 u8BlockNum;
    uint8 u8Ack;
} zps_tsExtendedFrameControlField;

typedef union {
    struct {
        uint8   u8DstEndpoint;
        uint16  u16ClusterId;
        uint16  u16ProfileId;
        uint8   u8SrcEndpoint;
        uint8   u8ApsCounter;
    } sUnicast;

    struct {
            uint16  u16GroupAddr;
            uint16  u16ClusterId;
            uint16  u16ProfileId;
            uint8   u8SrcEndpoint;
            uint8   u8ApsCounter;
        } sGroup;
} zps_tuApsAddressingField;

typedef struct {
    uint16    *psDuplicateTableSrcAddr;
    uint32    *psDuplicateTableHash;
    uint8     *psDuplicateTableApsCnt;
    uint8     u8TableIndex;
} zps_tsApsDuplicateTable;

typedef struct zps_tsMsgRecord_tag {
    struct zps_tsMsgRecord_tag *psNext;
    ZPS_tsAplApsdeReqRsp sApsdeReqRsp;
    ZPS_tsTsvTimer sAckTimer;       /* ack timer */
    uint8       u8ReTryCnt;
    uint8       u8ApsCount;
} zps_tsMsgRecord;

typedef struct zps_tsDcfmRecord_tag{
    union {
        uint16 u16DstAddr;
        uint64 u64DstAddr;
    };
    uint8   u8Handle;
    uint8   u8SrcEp;
    uint8   u8DstEp;
    uint8   u8DstAddrMode;
    uint8   u8SeqNum;
} zps_tsDcfmRecord;

typedef struct zps_tsDcfmRecordPool_tag{
    zps_tsDcfmRecord *psDcfmRecords;
    uint8 u8NextHandle;
    uint8 u8NumRecords;
} zps_tsDcfmRecordPool;

typedef struct zps_tsFragmentTransmit_tag {
    enum {
        ZPS_FRAG_TX_STATE_IDLE,
        ZPS_FRAG_TX_STATE_SENDING,
        ZPS_FRAG_TX_STATE_RESENDING,
        ZPS_FRAG_TX_STATE_WAIT_FOR_ACK
    }eState;
    PDUM_thAPduInstance hAPduInst;
    uint16  u16DstAddress;
    uint16  u16ProfileId;
    uint16  u16ClusterId;
    uint8   u8DstEndpoint;
    uint8   u8SrcEndpoint;
    uint8   u8Radius;
    uint8   u8SeqNum;

    ZPS_tsTsvTimer sAckTimer;
    uint8   u8CurrentBlock;
    uint8   u8SentBlocksInWindow;
    uint8   u8MinBlockNumber;
    uint8   u8MaxBlockNumber;
    uint8   u8TotalBlocksToSend;
    uint8   u8RetryCount;
    uint8   u8AckedBlocksInWindow;
    uint8   u8WindowSize;
    uint8   u8BlockSize;
    bool_t  bSecure;
} zps_tsFragmentTransmit;

typedef struct zps_tsfragTxPool_tag {
    zps_tsFragmentTransmit *psFragTxRecords;
    uint8   u8NumRecords;
} zps_tsFragTxPool;

typedef struct zps_tsFragmentReceive_tag {
    enum {
        ZPS_FRAG_RX_STATE_IDLE,
        ZPS_FRAG_RX_STATE_RECEIVING,
        ZPS_FRAG_RX_STATE_PERSISTING
    }eState;
    PDUM_thAPduInstance hAPduInst;
    uint16  u16SrcAddress;
    uint16  u16ProfileId;
    uint16  u16ClusterId;
    uint8   u8DstEndpoint;
    uint8   u8SrcEndpoint;
    uint8   u8SeqNum;

    ZPS_tsTsvTimer  sWindowTimer;
    PDUM_thNPdu     hNPduPrevious;
    uint16  u16ReceivedBytes;
    uint8   u8TotalBlocksToReceive;
    uint8   u8ReceivedBlocksInWindow;
    uint8   u8MinBlockNumber;
    uint8   u8MaxBlockNumber;
    uint8   u8HighestUnAckedBlock;
    uint8   u8WindowSize;
    uint8   u8BlockSize;
    uint8   u8PreviousBlock;
} zps_tsFragmentReceive;

typedef struct zps_tsfragRxPool_tag {
    zps_tsFragmentReceive *psFragRxRecords;
    uint8   u8NumRecords;
    uint8   u8PersistanceTime;
} zps_tsFragRxPool;

typedef struct zps_tsApsPollTimer {
    ZPS_tsTsvTimer sPollTimer;
    uint16 u16PollInterval;
    uint8 u8PollActive;
} zps_tsApsPollTimer;

typedef struct zps_tsApsmeCmdContainer {
    struct zps_tsApsmeCmdContainer *psNext; /* must be first element of struct */
    ZPS_tsNwkNldeReqRsp sNldeReqRsp;
    ZPS_tsTsvTimer sTimer;
    PDUM_thNPdu hNPduCopy;
    uint8 u8Retries;
} zps_tsApsmeCmdContainer;

typedef struct {
    zps_tsApsmeCmdContainer *psFreeList;
    zps_tsApsmeCmdContainer *psSubmittedList;
} zps_tsApsmeCmdMgr;

typedef struct {
    /* APSDE */
    void *pvParam;
    ZPS_tsAplApsdeDcfmIndHdr *(*prpsGetApsdeBuf)(void *);
    void (*prvPostApsdeDcfmInd)(void *, ZPS_tsAplApsdeDcfmIndHdr *);
    /* APSME */
    void *pvApsmeParam;
    ZPS_tsAplApsmeDcfmIndHdr *(*prpsGetApsmeBuf)(void *);
    void (*prvPostApsmeDcfmInd)(void *, ZPS_tsAplApsmeDcfmIndHdr *);

    zps_tsApsDuplicateTable *psApsDuplicateTable;
    zps_tsMsgRecord  *psSyncMsgPool;
    uint8 u8ApsDuplicateTableSize;
    uint8 u8SeqNum;
    uint8 u8SyncMsgPoolSize;
    uint8 u8MaxFragBlockSize;
    zps_tsDcfmRecordPool sDcfmRecordPool;
    zps_tsFragRxPool sFragRxPool;
    zps_tsFragTxPool sFragTxPool;
    ZPS_teStatus (*preStartFragmentTransmission)(void *, ZPS_tsAplApsdeReqRsp *, uint16, uint8);
    void (*prvHandleExtendedDataAck)(void *, ZPS_tsNwkNldeDcfmInd *, zps_tuApsAddressingField *, zps_tsExtendedFrameControlField *);
    void (*prvHandleDataFragmentReceive)(void *, ZPS_tsAplApsdeDcfmIndHdr *);
    zps_tsApsmeCmdMgr sApsmeCmdMgr;
    zps_tsApsPollTimer sApsPollTimer;    
} zps_tsApsContext;

/*** APL Context **************************************************/

typedef struct {
    /* indirection table for patchable functions */
#ifdef PATCH_ENABLE
    void **aprPatchTable; /* NB This must be the first item in this structure */
#endif

    void *pvMac;
    void *pvNwk;
    void *pvNwkTableSizes;
    void *pvNwkTables;
    void *pvInFCSet;
    
    ZPS_tsNwkNib *psNib;
    ZPS_tsAplAib *psAib;
    
    OS_thMutex hZpsMutex;
    OS_thMutex hMacMutex;
    OS_thMessage hDefaultStackEventMsg;
    OS_thMessage hMcpsDcfmIndMsg;
    OS_thMessage hMlmeDcfmIndMsg;
    OS_thMessage hTimeEventMsg;
    
    /* sub-layer contexts */
    zps_tsZdpContext sZdpContext;
    zps_tsZdoContext sZdoContext;
    zps_tsAfContext  sAfContext;
    zps_tsApsContext sApsContext;

    /* trust center context if present */
    zps_tsTrustCenterContext *psTrustCenterContext;

    /* persisted data descriptors
     * These don't need to be initialised
     */
    PDM_tsRecordDescriptor sAibPDDesc;
    PDM_tsRecordDescriptor sAibBindTblPDDesc;
    PDM_tsRecordDescriptor sAibGroupTblPDDesc;
    PDM_tsRecordDescriptor sAibLinkKeyTblPDDesc;
    PDM_tsRecordDescriptor sTcibDevTblPDDesc;
    PDM_tsRecordDescriptor sZdoPDDesc;    
} zps_tsApl;

/*** NIB Defaults **************************************************/

typedef struct
{
    uint32 u32VsOldRouteExpiryTime;
    uint8  u8MaxRouters;
    uint8  u8MaxChildren;
    uint8  u8MaxDepth;
    uint8  u8PassiveAckTimeout;
    uint8  u8MaxBroadcastRetries;
    uint8  u8MaxSourceRoute;
    uint8  u8NetworkBroadcastDeliveryTime;
    uint8  u8UniqueAddr;
    uint8  u8AddrAlloc;
    uint8  u8UseTreeRouting;
    uint8  u8SymLink;
    uint8  u8UseMulticast;
    uint8  u8LinkStatusPeriod;
    uint8  u8RouterAgeLimit;
    uint8  u8RouteDiscoveryRetriesPermitted;
    uint8  u8VsFormEdThreshold;
    uint8  u8SecurityLevel;
    uint8  u8AllFresh;
    uint8  u8SecureAllFrames;
    uint8  u8VsTxFailThreshold;
    uint8  u8VsMaxOutgoingCost;
} zps_tsNwkNibInitialValues;

/****************************************************************************/
/***        External Dependencies                                         ***/
/****************************************************************************/

PUBLIC ZPS_teStatus zps_eStartFragmentedTransmission(void *, ZPS_tsAplApsdeReqRsp *, uint16 , uint8);
PUBLIC void zps_vHandleExtendedDataAckFrame(void *, ZPS_tsNwkNldeDcfmInd *, zps_tuApsAddressingField *, zps_tsExtendedFrameControlField * );
PUBLIC void zps_vHandleApsdeDataFragIndNotSupported(void *pvApl, ZPS_tsAplApsdeDcfmIndHdr *);
PUBLIC void zps_vHandleApsdeDataFragInd(void *, ZPS_tsAplApsdeDcfmIndHdr *psApsdeDcfmInd);


/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

PRIVATE void vZdoServersInit(void);
PUBLIC bool zps_bAplZdoDefaultServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoDefaultServerInit(void *, PDUM_thAPdu );
PUBLIC bool zps_bAplZdoZdoClient(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoZdoClientInit(void *, PDUM_thAPdu );
PUBLIC bool zps_bAplZdoDeviceAnnceServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoDeviceAnnceServerInit(void *);
PUBLIC bool zps_bAplZdoActiveEpServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoActiveEpServerInit(void *, PDUM_thAPdu );
PUBLIC bool zps_bAplZdoNwkAddrServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoNwkAddrServerInit(void *, PDUM_thAPdu );
PUBLIC bool zps_bAplZdoIeeeAddrServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoIeeeAddrServerInit(void *, PDUM_thAPdu );
PUBLIC bool zps_bAplZdoSystemServerDiscoveryServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoSystemServerDiscoveryServerInit(void *, PDUM_thAPdu );
PUBLIC bool zps_bAplZdoNodeDescServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoNodeDescServerInit(void *, PDUM_thAPdu );
PUBLIC bool zps_bAplZdoPowerDescServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoPowerDescServerInit(void *, PDUM_thAPdu );
PUBLIC bool zps_bAplZdoMatchDescServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoMatchDescServerInit(void *, PDUM_thAPdu );
PUBLIC bool zps_bAplZdoSimpleDescServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoSimpleDescServerInit(void *, PDUM_thAPdu );
PUBLIC bool zps_bAplZdoMgmtLqiServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoMgmtLqiServerInit(void *, PDUM_thAPdu );
PUBLIC bool zps_bAplZdoMgmtLeaveServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoMgmtLeaveServerInit(void *, PDUM_thAPdu );
PUBLIC bool zps_bAplZdoMgmtNWKUpdateServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoMgmtNWKUpdateServerInit(void *, PDUM_thAPdu , void *);
PUBLIC bool zps_bAplZdoPermitJoiningServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoPermitJoiningServerInit(void *, PDUM_thAPdu );
PUBLIC bool zps_bAplZdoMgmtRtgServer(void *, void *, ZPS_tsAfEvent *);
PUBLIC void zps_vAplZdoMgmtRtgServerInit(void *, PDUM_thAPdu );

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

PRIVATE ZPS_tsAplApsmeBindingTableEntry s_bindingTableStorage[8];
PRIVATE ZPS_tsAplApsmeBindingTable s_bindingTable = { 0, s_bindingTableStorage, 8 };
PRIVATE ZPS_tsAplApsmeBindingTableType s_bindingTables = { NULL, &s_bindingTable };
PRIVATE ZPS_tsAplAib s_sAplAib = {
    0,
    0x0000000000000000ULL,
    0x00800000UL,
    FALSE,
    TRUE,
    0x02,
    0x0a,
    0,
    0,
    0,
    0x08,
    &s_bindingTables,
    NULL,
    NULL,
    NULL,
    0x03e8,
};
PRIVATE uint8 s_sDefaultServerContext[8] __attribute__ ((aligned (4)));
PRIVATE uint8 s_sZdoClientContext[8] __attribute__ ((aligned (4)));
PRIVATE uint8 s_sDeviceAnnceServerContext[0] __attribute__ ((aligned (4)));
PRIVATE uint8 s_sActiveEpServerContext[8] __attribute__ ((aligned (4)));
PRIVATE uint8 s_sNwkAddrServerContext[8] __attribute__ ((aligned (4)));
PRIVATE uint8 s_sIeeeAddrServerContext[8] __attribute__ ((aligned (4)));
PRIVATE uint8 s_sSystemServerDiscoveryServerContext[8] __attribute__ ((aligned (4)));
PRIVATE uint8 s_sNodeDescServerContext[8] __attribute__ ((aligned (4)));
PRIVATE uint8 s_sPowerDescServerContext[8] __attribute__ ((aligned (4)));
PRIVATE uint8 s_sMatchDescServerContext[8] __attribute__ ((aligned (4)));
PRIVATE uint8 s_sSimpleDescServerContext[8] __attribute__ ((aligned (4)));
PRIVATE uint8 s_sMgmtLqiServerContext[8] __attribute__ ((aligned (4)));
PRIVATE uint8 s_sMgmtLeaveServerContext[40] __attribute__ ((aligned (4)));
PRIVATE uint8 s_sMgmtNWKUpdateServerContext[64] __attribute__ ((aligned (4)));
PRIVATE uint8 s_sPermitJoiningServerContext[8] __attribute__ ((aligned (4)));
PRIVATE uint8 s_sMgmtRtgServerContext[8] __attribute__ ((aligned (4)));

/* ZDO Servers */
PRIVATE zps_tsAplZdoServer s_asAplZdoServers[17] = {
    { zps_bAplZdoZdoClient, s_sZdoClientContext },
    { zps_bAplZdoDeviceAnnceServer, s_sDeviceAnnceServerContext },
    { zps_bAplZdoActiveEpServer, s_sActiveEpServerContext },
    { zps_bAplZdoNwkAddrServer, s_sNwkAddrServerContext },
    { zps_bAplZdoIeeeAddrServer, s_sIeeeAddrServerContext },
    { zps_bAplZdoSystemServerDiscoveryServer, s_sSystemServerDiscoveryServerContext },
    { zps_bAplZdoNodeDescServer, s_sNodeDescServerContext },
    { zps_bAplZdoPowerDescServer, s_sPowerDescServerContext },
    { zps_bAplZdoMatchDescServer, s_sMatchDescServerContext },
    { zps_bAplZdoSimpleDescServer, s_sSimpleDescServerContext },
    { zps_bAplZdoMgmtLqiServer, s_sMgmtLqiServerContext },
    { zps_bAplZdoMgmtLeaveServer, s_sMgmtLeaveServerContext },
    { zps_bAplZdoMgmtNWKUpdateServer, s_sMgmtNWKUpdateServerContext },
    { zps_bAplZdoPermitJoiningServer, s_sPermitJoiningServerContext },
    { zps_bAplZdoMgmtRtgServer, s_sMgmtRtgServerContext },
    { zps_bAplZdoDefaultServer, s_sDefaultServerContext },
    { NULL, NULL }
};

/* Simple Descriptors */
PRIVATE uint16 s_au16Endpoint0InputClusterList[83] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001a, 0x001b, 0x001c, 0x001d, 0x001e, 0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002a, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x8000, 0x8001, 0x8002, 0x8003, 0x8004, 0x8005, 0x8006, 0x8010, 0x8011, 0x8012, 0x8014, 0x8015, 0x8016, 0x8017, 0x8018, 0x8019, 0x801a, 0x801b, 0x801c, 0x801d, 0x801e, 0x8020, 0x8021, 0x8022, 0x8023, 0x8024, 0x8025, 0x8026, 0x8027, 0x8028, 0x8029, 0x802a, 0x8030, 0x8031, 0x8032, 0x8033, 0x8034, 0x8035, 0x8036, 0x8037, 0x8038, };
PRIVATE PDUM_thAPdu s_ahEndpoint0InputClusterAPdus[83] = { apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, apduZDP, };
PRIVATE uint8 s_au8Endpoint0InputClusterDiscFlags[11] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

PRIVATE uint16 s_au16Endpoint0OutputClusterList[83] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001a, 0x001b, 0x001c, 0x001d, 0x001e, 0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002a, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x8000, 0x8001, 0x8002, 0x8003, 0x8004, 0x8005, 0x8006, 0x8010, 0x8011, 0x8012, 0x8014, 0x8015, 0x8016, 0x8017, 0x8018, 0x8019, 0x801a, 0x801b, 0x801c, 0x801d, 0x801e, 0x8020, 0x8021, 0x8022, 0x8023, 0x8024, 0x8025, 0x8026, 0x8027, 0x8028, 0x8029, 0x802a, 0x8030, 0x8031, 0x8032, 0x8033, 0x8034, 0x8035, 0x8036, 0x8037, 0x8038, };
PRIVATE uint8 s_au8Endpoint0OutputClusterDiscFlags[11] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

PRIVATE uint16 s_au16Endpoint34InputClusterList[1] = { 0x4311, };
PRIVATE PDUM_thAPdu s_ahEndpoint34InputClusterAPdus[1] = { apduAppEpRxData, };
PRIVATE uint8 s_au8Endpoint34InputClusterDiscFlags[1] = { 0x01 };

PRIVATE uint16 s_au16Endpoint34OutputClusterList[1] = { 0x4311, };
PRIVATE uint8 s_au8Endpoint34OutputClusterDiscFlags[1] = { 0x01 };

PRIVATE zps_tsAplAfSimpleDescCont s_asSimpleDescConts[2] = {
    {
        NULL,
        {
            0x0000,
            0,
            0,
            0,
            83,
            83,
            s_au16Endpoint0InputClusterList,
            s_au16Endpoint0OutputClusterList,
            s_au8Endpoint0InputClusterDiscFlags,
            s_au8Endpoint0OutputClusterDiscFlags,
        },
        s_ahEndpoint0InputClusterAPdus,
        1
    },
    {
        APP_EP_Events,
        {
            0x1134,
            2,
            0,
            34,
            1,
            1,
            s_au16Endpoint34InputClusterList,
            s_au16Endpoint34OutputClusterList,
            s_au8Endpoint34InputClusterDiscFlags,
            s_au8Endpoint34OutputClusterDiscFlags,
        },
        s_ahEndpoint34InputClusterAPdus,
        1
    },
};

/* Node Descriptor */
PRIVATE ZPS_tsAplAfNodeDescriptor s_sNodeDescriptor = {
    1,
    FALSE,
    FALSE,
    0,
    3,
    0,
    0x8e,
    0x0000,
    0x7f,
    0x0050,
    0x0000,
    0x0050,
    0x00};

/* Node Power Descriptor */
PRIVATE ZPS_tsAplAfNodePowerDescriptor s_sNodePowerDescriptor = {
    0x0,
    0x1,
    0x0,
    0xC};

/* APSDE duplicate table */
PRIVATE uint16 s_au16ApsDuplicateTableAddrs[1];
PRIVATE uint32 s_au32ApsDuplicateTableHash[1];
PRIVATE uint8 s_au8ApsDuplicateTableSeqCnts[1];
PRIVATE zps_tsApsDuplicateTable s_sApsDuplicateTable = { s_au16ApsDuplicateTableAddrs, s_au32ApsDuplicateTableHash, s_au8ApsDuplicateTableSeqCnts, 0 };

/* APSDE sync msg pool */
PRIVATE zps_tsMsgRecord s_asApsSyncMsgPool[3];

/* APSDE dcfm record pool */
PRIVATE zps_tsDcfmRecord s_asApsDcfmRecordPool[5];

/* APSME Command Manager Command Containers */
PRIVATE zps_tsApsmeCmdContainer s_sApsmeCmdContainer_4 = { NULL, {}, {}, NULL, 0 };
PRIVATE zps_tsApsmeCmdContainer s_sApsmeCmdContainer_3 = { &s_sApsmeCmdContainer_4, {}, {}, NULL, 0 };
PRIVATE zps_tsApsmeCmdContainer s_sApsmeCmdContainer_2 = { &s_sApsmeCmdContainer_3, {}, {}, NULL, 0 };
PRIVATE zps_tsApsmeCmdContainer s_sApsmeCmdContainer_1 = { &s_sApsmeCmdContainer_2, {}, {}, NULL, 0 };

/* Network Layer Context */
PRIVATE uint8                   s_sNwkContext[1760] __attribute__ ((aligned (4)));
PRIVATE ZPS_tsNwkDiscNtEntry    s_asNwkNtDisc[16];
PRIVATE ZPS_tsNwkActvNtEntry    s_asNwkNtActv[21];
PRIVATE ZPS_tsNwkRtDiscEntry    s_asNwkRtDisc[16];
PRIVATE ZPS_tsNwkRtEntry        s_asNwkRt[16];
PRIVATE ZPS_tsNwkBtr            s_asNwkBtt[9];
PRIVATE ZPS_tsNwkRctEntry       s_asNwkRct[4];
PRIVATE ZPS_tsNwkSecMaterialSet s_asNwkSecMatSet[2];
PRIVATE ZPS_tsNwkInFCDesc       s_asNwkInFCSet[2][21];
PRIVATE uint16                  s_au16NwkAddrMapNwk[16];
PRIVATE uint64                  s_au64NwkAddrMapExt[16];

PRIVATE const zps_tsNwkNibInitialValues s_sNibInitialValues =
{
    600,
    05,
    7,
    15,
    1,
    2,
    12,
    18,
    0,
    2,
    0,
    1,
    0,
    15,
    3,
    3,
    100,
    5,
    TRUE,
    TRUE,
    5,
    4
};


PRIVATE ZPS_tsNwkNibTblSize     s_sNwkTblSize = {
    16,
    21,
    16,
    16,
    9,
    4,
    2,
    21,
    16,
    sizeof(s_sNibInitialValues)
};

PRIVATE ZPS_tsNwkNibTbl s_sNwkTbl = {
    s_asNwkNtDisc,
    s_asNwkNtActv,
    s_asNwkRtDisc,
    s_asNwkRt,
    s_asNwkBtt,
    s_asNwkRct,
    s_asNwkSecMatSet,
    s_au16NwkAddrMapNwk,
    s_au64NwkAddrMapExt,
    (ZPS_tsNwkNibInitialValues*)&s_sNibInitialValues
};

/* Application Layer Context */
PRIVATE zps_tsApl s_sApl = {
    NULL,
    s_sNwkContext,
    &s_sNwkTblSize,
    &s_sNwkTbl,
    s_asNwkInFCSet,
    NULL,
    &s_sAplAib,
    mutexZPS,
    mutexMAC,
    APP_msgZpsEvents,
    zps_msgMcpsDcfmInd,
    zps_msgMlmeDcfmInd,
    zps_msgTimeEvents,
    { 0 },
    {
        0,
        ZPS_ZDO_DEVICE_ROUTER,
        ZPS_ZDO_NO_NETWORK_KEY,
        0xff,
        2,
        2,
        3,
        FALSE,
        s_asAplZdoServers,
        vZdoServersInit,
        { /* timer struct */},
        { /* timer struct */},
        FALSE,
        0,
        3,
        0,
        1,
        NULL
    },
    {
        NULL,
        &s_sNodeDescriptor,
        &s_sNodePowerDescriptor,
        2,
        s_asSimpleDescConts,
        NULL,
        NULL,
        0xff,
        0x00
    },
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        &s_sApsDuplicateTable,
        s_asApsSyncMsgPool,
        0x01,
        0,
        3,
        0,
        { s_asApsDcfmRecordPool, 1, 5 },
        { NULL, 0, 0 },
        { NULL, 0 },
        NULL,
        NULL,
        zps_vHandleApsdeDataFragIndNotSupported,
        { &s_sApsmeCmdContainer_1, NULL },
        { { /* Timer */}, 100, 0 }
    },
    NULL
};

void *zps_g_pvApl = &s_sApl;

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/* ZDO Server Initialisation */
PRIVATE void vZdoServersInit(void)
{
    /* Version compatibility check */
#if JENNIC_CHIP_FAMILY == JN514x
    asm(".extern ZPS_APL_Version_1v3" : ) ;
    asm("b.addi r0,r0,ZPS_APL_Version_1v3" : );
    asm(".extern ZPS_NWK_Version_1v3" : ) ;
    asm("b.addi r0,r0,ZPS_NWK_Version_1v3" : );
#elif JENNIC_CHIP_FAMILY == JN513x
    asm(".extern ZPS_APL_Version_1v3" : ) ;
    asm("l.addi r0,r0,hi(ZPS_APL_Version_1v3)" : );
    asm(".extern ZPS_NWK_Version_1v3" : ) ;
    asm("l.addi r0,r0,hi(ZPS_NWK_Version_1v3)" : );
#endif
    zps_vAplZdoDefaultServerInit(&s_sDefaultServerContext, apduZDP);
    zps_vAplZdoZdoClientInit(&s_sZdoClientContext, apduZDP);
    zps_vAplZdoDeviceAnnceServerInit(&s_sDeviceAnnceServerContext);
    zps_vAplZdoActiveEpServerInit(&s_sActiveEpServerContext, apduZDP);
    zps_vAplZdoNwkAddrServerInit(&s_sNwkAddrServerContext, apduZDP);
    zps_vAplZdoIeeeAddrServerInit(&s_sIeeeAddrServerContext, apduZDP);
    zps_vAplZdoSystemServerDiscoveryServerInit(&s_sSystemServerDiscoveryServerContext, apduZDP);
    zps_vAplZdoNodeDescServerInit(&s_sNodeDescServerContext, apduZDP);
    zps_vAplZdoPowerDescServerInit(&s_sPowerDescServerContext, apduZDP);
    zps_vAplZdoMatchDescServerInit(&s_sMatchDescServerContext, apduZDP);
    zps_vAplZdoSimpleDescServerInit(&s_sSimpleDescServerContext, apduZDP);
    zps_vAplZdoMgmtLqiServerInit(&s_sMgmtLqiServerContext, apduZDP);
    zps_vAplZdoMgmtLeaveServerInit(&s_sMgmtLeaveServerContext, apduZDP);
    zps_vAplZdoMgmtNWKUpdateServerInit(&s_sMgmtNWKUpdateServerContext, apduZDP, &s_sApl);
    zps_vAplZdoPermitJoiningServerInit(&s_sPermitJoiningServerContext, apduZDP);
    zps_vAplZdoMgmtRtgServerInit(&s_sMgmtRtgServerContext, apduZDP);
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
