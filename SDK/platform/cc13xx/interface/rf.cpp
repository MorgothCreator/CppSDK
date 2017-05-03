/*
 * rf.cpp
 */

#include "rf.h"

// CMD_FS
const rfc_CMD_FS_t RF_cmdFs =
{
        /*.commandNo = */0x0803,
        /*.status = */0x0000,
        /*.pNextOp = */0, // INSERT APPLICABLE POINTER: (unsigned char*)&xxx
        /*.startTime = */0x00000000,
        /*.startTrigger.triggerType = */0x0,
        /*.startTrigger.bEnaCmd = */0x0,
        /*.startTrigger.triggerNo = */0x0,
        /*.startTrigger.pastTrig = */0x0,
        /*.condition.rule = */0x1,
        /*.condition.nSkip = */0x0,
        /*.frequency = */0x0364,
        /*.fractFreq = */0x0000,
        /*.synthConf.bTxMode = */0x0,
        /*.synthConf.refFreq = */0x0,
        /*.__dummy0 = */0x00,
        /*.midPrecal = */0x00,
        /*.ktPrecal = */0x00,
        /*.tdcPrecal = */0x0000,
};

// Overrides for CMD_PROP_RADIO_DIV_SETUP
static unsigned long pOverrides[] =
{
        // override_synth_prop_863_970_div5.xml
        ADI_HALFREG_OVERRIDE(0,61,0xF,0xD),
        HW_REG_OVERRIDE(0x4038,0x003A),
        HW_REG_OVERRIDE(0x4020,0x7F00),
        HW_REG_OVERRIDE(0x4064,0x0040),
        (unsigned long)0x000684A3,
        (unsigned long)0xC0040141,
        (unsigned long)0x0533B107,
        (unsigned long)0x0A480583,
        (unsigned long)0x7AB80603,
        ADI_REG_OVERRIDE(1,4,0x9F),
        ADI_HALFREG_OVERRIDE(1,7,0x4,0x4),
        (unsigned long)0x02010403,
        (unsigned long)0x00108463,
        (unsigned long)0x04B00243,
        // override_phy_gfsk_rx_rssi.xml
        HW_REG_OVERRIDE(0x6084,0x35F1),
        (unsigned long)0x00038883,
        (unsigned long)0x00FB88A3,
        // override_phy_agc_reflevel_0x1a.xml
        HW_REG_OVERRIDE(0x6088,0x001A),
        // override_phy_rx_aaf_bw_0xd.xml
        ADI_HALFREG_OVERRIDE(0,61,0xF,0xD),
        // override_phy_agc_reflevel_0x1a.xml
        HW_REG_OVERRIDE(0x6088,0x001A),
        // TX power override
        ADI_REG_OVERRIDE(0,12,0xF8),
        (unsigned long)0xFFFFFFFF,
};

// CMD_PROP_RADIO_DIV_SETUP
const rfc_CMD_PROP_RADIO_DIV_SETUP_t RF_cmdPropRadioDivSetup =
{
        /*.commandNo = */0x3807,
        /*.status = */0x0000,
        /*.pNextOp = */0, // INSERT APPLICABLE POINTER: (unsigned char*)&xxx
        /*.startTime = */0x00000000,
        /*.startTrigger.triggerType = */0x0,
        /*.startTrigger.bEnaCmd = */0x0,
        /*.startTrigger.triggerNo = */0x0,
        /*.startTrigger.pastTrig = */0x0,
        /*.condition.rule = */0x1,
        /*.condition.nSkip = */0x0,
        /*.modulation.modType = */0x1,
        /*.modulation.deviation = */0x64,
        /*.symbolRate.preScale = */0xf,
        /*.symbolRate.rateWord = */0x8000,
        /*.rxBw = */0x24,
        /*.preamConf.nPreamBytes = */0x4,
        /*.preamConf.preamMode = */0x0,
        /*.formatConf.nSwBits = */0x20,
        /*.formatConf.bBitReversal = */0x0,
        /*.formatConf.bMsbFirst = */0x1,
        /*.formatConf.fecMode = */0x0,
        /*.formatConf.whitenMode = */0x0,
        /*.config.frontEndMode = */0x0,
        /*.config.biasMode = */0x1,
        /*.config.bNoFsPowerUp = */0x0,
        /*.txPower = */0xa73f,
        /*.pRegOverride = */(unsigned long *)pOverrides,
        /*.centerFreq = */0x0364,
        /*.intFreq = */0x8000,
        /*.loDivider = */0x05,
};

// CMD_PROP_TX
const rfc_CMD_PROP_TX_t RF_cmdPropTx =
{
        /*.commandNo = */0x3801,
        /*.status = */0x0000,
        /*.pNextOp = */0, // INSERT APPLICABLE POINTER: (unsigned char*)&xxx
        /*.startTime = */0x00000000,
        /*.startTrigger.triggerType = */0x0,
        /*.startTrigger.bEnaCmd = */0x0,
        /*.startTrigger.triggerNo = */0x0,
        /*.startTrigger.pastTrig = */0x0,
        /*.condition.rule = */0x1,
        /*.condition.nSkip = */0x0,
        /*.pktConf.bFsOff = */0x0,
        /*.pktConf.bUseCrc = */0x1,
        /*.pktConf.bVarLen = */0x1,
        /*.pktLen = */0x1e, // SET APPLICATION PAYLOAD LENGTH
        /*.syncWord = */0x930b51de,
        /*.pPkt = */0, // INSERT APPLICABLE POINTER: (unsigned char*)&xxx
};

// CMD_PROP_RX
const rfc_CMD_PROP_RX_t RF_cmdPropRx =
{
        /*.commandNo = */0x3802,
        /*.status = */0x0000,
        /*.pNextOp = */0, // INSERT APPLICABLE POINTER: (unsigned char*)&xxx
        /*.startTime = */0x00000000,
        /*.startTrigger.triggerType = */0x0,
        /*.startTrigger.bEnaCmd = */0x0,
        /*.startTrigger.triggerNo = */0x0,
        /*.startTrigger.pastTrig = */0x0,
        /*.condition.rule = */0x1,
        /*.condition.nSkip = */0x0,
        /*.pktConf.bFsOff = */0x0,
        /*.pktConf.bRepeatOk = */0x0,
        /*.pktConf.bRepeatNok = */0x0,
        /*.pktConf.bUseCrc = */0x1,
        /*.pktConf.bVarLen = */0x1,
        /*.pktConf.bChkAddress = */0x0,
        /*.pktConf.endType = */0x0,
        /*.pktConf.filterOp = */0x0,
        /*.rxConf.bAutoFlushIgnored = */0x0,
        /*.rxConf.bAutoFlushCrcErr = */0x0,
        /*.rxConf.bIncludeHdr = */0x1,
        /*.rxConf.bIncludeCrc = */0x0,
        /*.rxConf.bAppendRssi = */0x0,
        /*.rxConf.bAppendTimestamp = */0x0,
        /*.rxConf.bAppendStatus = */0x1,
        /*.syncWord = */0x930b51de,
        /*.maxPktLen = */0x7d, // MAKE SURE DATA ENTRY IS LARGE ENOUGH
        /*.address0 = */0x00,
        /*.address1 = */0x00,
        /*.endTrigger.triggerType = */0x1,
        /*.endTrigger.bEnaCmd = */0x0,
        /*.endTrigger.triggerNo = */0x0,
        /*.endTrigger.pastTrig = */0x0,
        /*.endTime = */0x00000000,
        /*.pQueue = */0, // INSERT APPLICABLE POINTER: (dataQueue_t*)&xxx
        /*.pOutput = */0, // INSERT APPLICABLE POINTER: (unsigned char*)&xxx
};



static rfc_CMD_START_RAT_t RF_cmdStartRat =
{
    .commandNo = CMD_START_RAT
};

static rfc_CMD_GET_RSSI_t rfc_CMD_GET_RSSI =
{
    .commandNo = 0x0403
};

static rfc_CMD_ABORT_t RF_cmdAbort =
{
    .commandNo = CMD_ABORT
};

static volatile rfc_CMD_FS_t rfc_CMD_FS;
static rfc_CMD_START_RAT_t rfc_CMD_START_RAT;
static volatile rfc_CMD_PROP_RADIO_DIV_SETUP_t rfc_CMD_PROP_RADIO_DIV_SETUP;
static volatile rfc_CMD_PROP_TX_t rfc_CMD_PROP_TX;
static volatile rfc_CMD_PROP_RX_t rfc_CMD_PROP_RX;

GI::Dev::Rf::Rf()
{
    volatile int cnt = 0;
    volatile int result = 0;
    memcpy(&rfc_CMD_START_RAT, &RF_cmdStartRat, sizeof(rfc_CMD_START_RAT_t));
    result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_START_RAT);
    //while(rfc_CMD_START_RAT.status < 3);
    for(cnt = 0; cnt < 250000; cnt++);
    memcpy((rfc_CMD_PROP_RADIO_DIV_SETUP_t *)&rfc_CMD_PROP_RADIO_DIV_SETUP, &RF_cmdPropRadioDivSetup, sizeof(rfc_CMD_PROP_RADIO_DIV_SETUP_t));
    //rfc_CMD_PROP_RADIO_DIV_SETUP.symbolRate.preScale
    result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_PROP_RADIO_DIV_SETUP);
    while(rfc_CMD_PROP_RADIO_DIV_SETUP.status < 3);
    for(cnt = 0; cnt < 250000; cnt++);

}

GI::Dev::Rf::~Rf()
{

}

SysErr GI::Dev::Rf::txPocket(unsigned char *buff, unsigned short len, unsigned long *addrs, unsigned char addrsNr)
{
    memcpy((rfc_CMD_FS_t *)&rfc_CMD_FS, &RF_cmdFs, sizeof(rfc_CMD_FS_t));
    rfc_CMD_FS.synthConf.bTxMode = 1;
     /* Set the frequency */
    //ui32Freq /= 1000;
    //rfc_CMD_FS.frequency = (unsigned short)(ui32Freq / 1000);
    //rfc_CMD_FS.fractFreq = ((ui32Freq - (rfc_CMD_FS.frequency * 1000)) * 65);
    volatile int result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_FS);
    if(result != 1)
        return SYS_ERR_TRANSMISSION;
    volatile int cnt = 0;
    for(cnt = 0; cnt < 2500; cnt++)
    {
        if(rfc_CMD_FS.status == DONE_OK)
            break;
    }
    if(cnt == 2500)
        return SYS_ERR_TIMEOUT;

    memcpy((rfc_CMD_PROP_TX_t *)&rfc_CMD_PROP_TX, &RF_cmdPropTx, sizeof(rfc_CMD_PROP_TX_t));
    rfc_CMD_PROP_TX.startTrigger.triggerType = TRIG_ABSTIME;
    rfc_CMD_PROP_TX.startTrigger.pastTrig = 1;
    rfc_CMD_PROP_TX.startTime = 0;
    memcpy(send_buff + 3, buff, len);
    send_buff[0] = *addrs;
    send_buff[1] = (unsigned char)(seqNumber >> 8);
    send_buff[2] = (unsigned char)(seqNumber);
    rfc_CMD_PROP_TX.pPkt = send_buff;
    rfc_CMD_PROP_TX.pktLen = len + 2;
    rfc_CMD_PROP_TX.pktConf.bFsOff = 0;
    result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_PROP_TX);
    if(result != 1)
        return SYS_ERR_TRANSMISSION;
    for(cnt = 0; cnt < 2500000; cnt++)
    {
        if(rfc_CMD_PROP_TX.status == PROP_DONE_OK)
        {
            seqNumber++;
            return SYS_ERR_OK;
        }
    }
    return SYS_ERR_TIMEOUT;
}

SysErr GI::Dev::Rf::rxPocket(unsigned char *buff, unsigned short *len, unsigned long *addrs, unsigned char addrsNr, signed short *rssi)
{
    volatile int cnt = 0;
    volatile int result = 0;
    memcpy((rfc_CMD_FS_t *)&rfc_CMD_FS, &RF_cmdFs, sizeof(rfc_CMD_FS_t));
    rfc_CMD_FS.synthConf.bTxMode = 0;

     /* Set the frequency */
    /*rfc_CMD_FS.frequency = (unsigned short)(ui32Freq / 1000000);
    rfc_CMD_FS.fractFreq = (unsigned short) (((uint64_t)ui32Freq -
                (rfc_CMD_FS.frequency * 1000000)) * 65536 / 1000000);*/
    //ui32Freq /= 1000;
    //rfc_CMD_FS.frequency = (unsigned short)(ui32Freq / 1000);
    //rfc_CMD_FS.fractFreq = ((ui32Freq - (rfc_CMD_FS.frequency * 1000)) * 65);
    result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_FS);
    if(result != 1)
        return SYS_ERR_RECEPTION;
    for(cnt = 0; cnt < 2500; cnt++)
    {
        if(rfc_CMD_FS.status == DONE_OK)
            break;
    }
    if(cnt == 2500)
        return SYS_ERR_TIMEOUT;

    memcpy((rfc_CMD_PROP_RX_t *)&rfc_CMD_PROP_RX, &RF_cmdPropRx, sizeof(rfc_CMD_PROP_RX_t));
    rfc_CMD_PROP_RX.pQueue = &dataQueue;
    rfc_CMD_PROP_RX.pOutput = (unsigned char*)&rxStatistics;
    rfc_CMD_PROP_RX.startTrigger.triggerType = TRIG_NOW;
    rfc_CMD_PROP_RX.pktConf.bFsOff = 0;
    rfc_CMD_PROP_RX.startTrigger.pastTrig = 1;
    rfc_CMD_PROP_RX.startTime = 0;
    if(addrsNr == 1)
        rfc_CMD_PROP_RX.address0 = *addrs;
    else if(addrsNr == 2)
    {
        rfc_CMD_PROP_RX.address0 = addrs[0];
        rfc_CMD_PROP_RX.address1 = addrs[1];
    }
    else
        return SYS_ERR_INVALID_PARAM;
    rfc_CMD_PROP_RX.pktConf.bChkAddress = 1;
    result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_PROP_RX);
    if(result != 1)
        return SYS_ERR_RECEPTION;
    for(cnt = 0; cnt < 1000000; cnt++)
    {
        if(rfc_CMD_PROP_RX.status >= PROP_DONE_OK && rfc_CMD_PROP_RX.status <= PROP_DONE_BUSYTIMEOUT)
        {
            /* Get current unhandled data entry */
            currentDataEntry = RFQueue_getDataEntry();
            /* Handle the packet data, located at &currentDataEntry->data:
            * - Length is the first byte with the current configuration
            * - Data starts from the second byte */
            packetLength      = *(unsigned char*)(&currentDataEntry->data);
            packetDataPointer = (unsigned char*)(&currentDataEntry->data + 1);
            /* Copy the payload + the status byte to the packet variable */
            memcpy(buff, packetDataPointer, (packetLength));
            *len = packetLength;

            RFQueue_nextEntry();
            return SYS_ERR_TIMEOUT;
        }
    }
    result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_GET_RSSI);
    while(result < 3 && result >= 0);
    *rssi = result >> 16;
    //UARTprintf(DebugCom, "RSSI = %d.\r\n" , (long) rssi);
    //*rssi = result;
    *len = 0;
    abort();
    return SYS_ERR_TIMEOUT;
}

void GI::Dev::Rf::abort()
{
    RFCDoorbellSendTo((unsigned long)&RF_cmdAbort);
}

//*****************************************************************************
//
//! Define a queue
//!
//! \param dataQueue is a pointer to the queue to use
//! \param buf is the prealocated byte buffer to use
//! \param buf_len is the number of preallocated bytes
//! \param numEntries are the number of dataEntries to split the buffer into
//! \param length is the length of data in every dataEntry
//!
//! \return unsigned char
//
//*****************************************************************************
unsigned char GI::Dev::Rf::defineQueue(dataQueue_t *dataQueue, unsigned char *buf, unsigned short buf_len, unsigned char numEntries, unsigned short length)
{

  if (buf_len < (numEntries * (length + RF_QUEUE_DATA_ENTRY_HEADER_SIZE + RF_QUEUE_QUEUE_ALIGN_PADDING)))
  {
    /* queue does not fit into buffer */
    return (1);
  }

  /* Padding needed for 4-byte alignment? */
  unsigned char pad = 4-((length + RF_QUEUE_DATA_ENTRY_HEADER_SIZE)%4);

  /* Set the Data Entries common configuration */
  unsigned char *first_entry = buf;
  int i;
  for (i = 0; i < numEntries; i++)
  {
    buf = &(*buf)+(i*(RF_QUEUE_DATA_ENTRY_HEADER_SIZE+length+pad));
    ((rfc_dataEntry_t*)buf)->status        = DATA_ENTRY_PENDING;        // Pending - starting state
    ((rfc_dataEntry_t*)buf)->config.type   = DATA_ENTRY_TYPE_GEN;       // General Data Entry
    ((rfc_dataEntry_t*)buf)->config.lenSz  = 0;                         // No length indicator byte in data
    ((rfc_dataEntry_t*)buf)->length        = length;                    // Total length of data field

    ((rfc_dataEntryGeneral_t*)buf)->pNextEntry = &(((rfc_dataEntryGeneral_t*)buf)->data)+length+pad;
  }
  /* Make circular Last.Next -> First */
  ((rfc_dataEntry_t*)buf)->pNextEntry = first_entry;

  /* Create Data Entry Queue and configure for circular buffer Data Entries */
  dataQueue->pCurrEntry = first_entry;
  dataQueue->pLastEntry = NULL;

  /* Set read pointer to first entry */
  readEntry = (rfc_dataEntryGeneral_t*)first_entry;

  return (0);
}

