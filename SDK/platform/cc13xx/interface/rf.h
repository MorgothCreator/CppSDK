/*
 * rf.h
 *
 *  Created on: May 1, 2017
 *      Author: Cutare
 */

#ifndef CC13XX_INTERFACE_RF_H_
#define CC13XX_INTERFACE_RF_H_

#include <include/global.h>
#include <driver/rfc.h>
#include <driver/rf_prop_cmd.h>
#include <driver/rf_prop_mailbox.h>
#include <driver/rf_common_cmd.h>
#include <driver/rf_data_entry.h>

#define RF_QUEUE_DATA_ENTRY_HEADER_SIZE  8                       // Contant header size of a Generic Data Entry
#define RF_QUEUE_QUEUE_ALIGN_PADDING     3                       // Padding offset

#define RF_QUEUE_DATA_ENTRY_BUFFER_SIZE(numEntries, dataSize, appendedBytes)                                                    \
(numEntries*(RF_QUEUE_DATA_ENTRY_HEADER_SIZE + dataSize + appendedBytes + RF_QUEUE_QUEUE_ALIGN_PADDING))


namespace GI
{
namespace Dev
{
class Rf
{
public:
    Rf();
    ~Rf();
    SysErr txPocket(unsigned char *buff, unsigned short len, unsigned long *addrs, unsigned char addrsNr);
    SysErr rxPocket(unsigned char *buff, unsigned short *len, unsigned long *addrs, unsigned char addrsNr, signed short *rssi);
    void abort();

private:
    dataQueue_t dataQueue;
    rfc_dataEntryGeneral_t* currentDataEntry;
    rfc_propRxOutput_t rxStatistics;
    rfc_dataEntryGeneral_t* readEntry;
    unsigned char send_buff[0x128];
    unsigned short seqNumber;
    unsigned char packetLength;
    unsigned char* packetDataPointer;

    rfc_dataEntryGeneral_t *RFQueue_getDataEntry()
    {
      return (readEntry);
    }
    unsigned char RFQueue_nextEntry()
    {
      /* Set status to pending */
      readEntry->status = DATA_ENTRY_PENDING;

      /* Move read entry pointer to next entry */
      readEntry = (rfc_dataEntryGeneral_t*)readEntry->pNextEntry;

      return (readEntry->status);
    }
    unsigned char defineQueue(dataQueue_t *dataQueue, unsigned char *buf, unsigned short buf_len, unsigned char numEntries, unsigned short length);

};
}
}



#endif /* CC13XX_INTERFACE_RF_H_ */
