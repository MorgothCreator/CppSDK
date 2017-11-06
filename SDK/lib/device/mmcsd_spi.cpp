/*
 * device/mmcsd_spi.cpp
 */

#include "mmcsd_spi.h"
#include <api/dev_request.h>
#include <api/io_handle.h>
#include <api/timer.h>
#include <main.h>

//#######################################################################################
unsigned char GI::Dev::MmcSd_Spi::ioData(unsigned char value)
{
    unsigned char tmp = value;
    spiUnit->writeReadByte(&tmp);
    return tmp;
}
//#######################################################################################
void GI::Dev::MmcSd_Spi::csAssert()
{
    spiUnit->disableCsHandle = true;
    spiUnit->assert();
}
//#######################################################################################
void GI::Dev::MmcSd_Spi::csDeassert()
{
    spiUnit->deassert();
}
//#######################################################################################
/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/
int GI::Dev::MmcSd_Spi::waitReady (    /* 1:Ready, 0:Timeout */
    unsigned int wt         /* Timeout [ms] */
)
{
    unsigned char d;

    unsigned int Timer2 = wt;
    do {
        d = ioData(0xFF);
        /* This loop takes a time. Insert rot_rdq() here for multitask envilonment. */
        //sd_delay(1);
    } while (d != 0xFF && Timer2--);    /* Wait for card goes ready or timeout */

    return (d == 0xFF) ? 1 : 0;
}
/*-----------------------------------------------------------------------*/
/* Deselect card and release SPI                                         */
/*-----------------------------------------------------------------------*/
void GI::Dev::MmcSd_Spi::deselect ()
{
    csDeassert();      /* CS = H */
    ioData(0xFF);    /* Dummy clock (force DO hi-z for multiple slave SPI) */
}
/*-----------------------------------------------------------------------*/
/* Select card and wait for ready                                        */
/*-----------------------------------------------------------------------*/
int GI::Dev::MmcSd_Spi::select()    /* 1:OK, 0:Timeout */
{
    csAssert();
    ioData(0xFF);    /* Dummy clock (force DO enabled) */

    if (waitReady(50000))
        return 1;   /* OK */
    deselect();
    return 0;   /* Timeout */
}
/*-----------------------------------------------------------------------*/
/* Send a command packet to the MMC                                      */
/*-----------------------------------------------------------------------*/
unsigned char GI::Dev::MmcSd_Spi::sendCmd (     /* Return value: R1 resp (bit7==1:Failed to send) */
    unsigned char cmd,       /* Command index */
    unsigned long arg       /* Argument */
)
{
    unsigned char n, res;


    if (cmd & 0x80)
    {   /* Send a CMD55 prior to ACMD<n> */
        cmd &= 0x7F;
        res = sendCmd(CMD55, 0);
        if (res > 1)
            return res;
    }

    /* Select card */
    if (cmd != CMD12)
    {
        deselect();
        if (!select())
            return 0xFF;
    }

    //if (cmd == CMD17)
        //sendCmd(CMD59, 0);
    /* Send command packet */
    ioData(0x40 | cmd);              /* Start + command index */
    ioData((unsigned char)(arg >> 24));       /* Argument[31..24] */
    ioData((unsigned char)(arg >> 16));       /* Argument[23..16] */
    ioData((unsigned char)(arg >> 8));            /* Argument[15..8] */
    ioData((unsigned char)arg);               /* Argument[7..0] */
    n = 0x01;                           /* Dummy CRC + Stop */
    if (cmd == CMD0)
        n = 0x95;           /* Valid CRC for CMD0(0) */
    if (cmd == CMD8)
        n = 0x87;           /* Valid CRC for CMD8(0x1AA) */
    ioData(n);

    /* Receive command resp */
    if (cmd == CMD12)
        ioData(0xFF);    /* Diacard following one byte when CMD12 */
    n = 255;                                /* Wait for response (10 bytes max) */
    do
        res = ioData(0xFF);
    while ((res & 0x80) && --n);

    return res;
    /* Return received response */
}
//#######################################################################################
bool GI::Dev::MmcSd_Spi::initCard()
{
    spiUnit->setSpeed(400000);
    csDeassert();
    unsigned char cmd, ty, ocr[4], csd[16];
    unsigned short  n;
    sdType = IsSd;
    for (n = 10; n; n--)
        ioData(0xFF);    /* Send 80 dummy clocks */

    ty = 0;
    if (sendCmd(CMD0, 0) == 1)
    {           /* Put the card SPI/Idle state */
        unsigned int Timer1 = 1000;                     /* Initialization timeout = 1 sec */
        if (sendCmd(CMD8, 0x1AA) == 1)
        {   /* SDv2? */
            for (n = 0; n < 4; n++) ocr[n] = ioData(0xFF);   /* Get 32 bit return value of R7 resp */
            if (ocr[2] == 0x01 && ocr[3] == 0xAA)
            {               /* Is the card supports vcc of 2.7-3.6V? */
                while (Timer1-- && sendCmd(ACMD41, 1UL << 30)) ;    /* Wait for end of initialization with ACMD41(HCS) */
                if (Timer1 && sendCmd(CMD58, 0) == 0)
                {       /* Check CCS bit in the OCR */
                    for (n = 0; n < 4; n++)
                        ocr[n] = ioData(0xFF);
                    ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;  /* Card id SDv2 */
                    if(ty & CT_BLOCK)
                        sdType = IsSdhc;
                }
            }
        } else {    /* Not SDv2 card */
            if (sendCmd(ACMD41, 0) <= 1)
            {   /* SDv1 or MMC? */
                ty = CT_SD1; cmd = ACMD41;  /* SDv1 (ACMD41(0)) */
            } else
            {
                ty = CT_MMC; cmd = CMD1;    /* MMCv3 (CMD1(0)) */
            }
            while (Timer1-- && sendCmd(cmd, 0)) ;       /* Wait for end of initialization */
            if (!Timer1 || sendCmd(CMD16, 512) != 0)    /* Set block length: 512 */
                ty = 0;
        }
    }
    if (sendCmd(CMD59, 0) == 0)
    {

    }

    /*if (sendCmd(CMD10, 0) == 0)
    {
        for (n = 0; n < 32; n++) cid[n] = ioData(0xFF);
        memcpy(raw_cid, cid, 16);
    }*/
    if (ty)
    {           /* OK */
        spiUnit->setSpeed(25000000);
       if (sendCmd(CMD9, 0) == 0)
        {
            unsigned int wait = 0, response = 0;
            while (++wait <= 0x1ff && response != 0xfe)
                response = ioData(0xff);
            if (wait >= 0x1ff)
            {
                SD_Init_OK = false;
                deselect();
                return SD_Init_OK;
            }
            for (n = 0; n < 16; n++)
                csd[n] = ioData(0xFF);
            ioData(0xFF);
            ioData(0xFF);
            raw_csd[0] = ((unsigned long)csd[15] + ((unsigned long)csd[14] << 8) +  ((unsigned long)csd[13] << 16) +  ((unsigned long)csd[12] << 24));
            raw_csd[1] = ((unsigned long)csd[11] + ((unsigned long)csd[10] << 8) +  ((unsigned long)csd[9] << 16) +  ((unsigned long)csd[8] << 24));
            raw_csd[2] = ((unsigned long)csd[7] + ((unsigned long)csd[6] << 8) +  ((unsigned long)csd[5] << 16) +  ((unsigned long)csd[4] << 24));
            raw_csd[3] = ((unsigned long)csd[3] + ((unsigned long)csd[2] << 8) +  ((unsigned long)csd[1] << 16) +  ((unsigned long)csd[0] << 24));
            //memcpy(raw_csd, csd, 16);
        }
        if (sdType == IsSdhc)
        {
            tranSpeed = SD_CARD1_TRANSPEED(this);
            blkLen = 1 << (SD_CARD1_RDBLKLEN(this));
            size = SD_CARD1_SIZE(this);
            nBlks = size >> 9;// blkLen;
        }
        else
        {
            tranSpeed = SD_CARD0_TRANSPEED(this);
            blkLen = 1 << (SD_CARD0_RDBLKLEN(this));
            nBlks = SD_CARD0_NUMBLK(this);
            size = SD_CARD0_SIZE(this);
        }
        SD_Init_OK = true;
    } else
    {           /* Failed */
        //power_off();
        SD_Init_OK = false;
    }

    deselect();
    return SD_Init_OK;
}
//#######################################################################################
bool GI::Dev::MmcSd_Spi::rcvrDatablock(unsigned char *buff, unsigned int bytes_to_read)
{
    unsigned char token;
    unsigned long Timer1 = 50000;
    do
    {                           /* Wait for data packet in timeout of 100ms */
        token = ioData(255);
    } while ((token == 0xFF) && Timer1--);

    if(token != MMC_DATA_TOKEN)
        return false;   /* If not valid data token, return with error */

    do
    {                           // Receive the data block into buffer
        *buff++ = ioData(255);
    } while (--bytes_to_read);
    /*HardUnitStruct->CsSelect = SpiInstance;
    memset(buff, 0xFF, bytes_to_read);
    HardUnitStruct->Buff = buff;
    if(!HardUnitReadWriteBuffFunc(HardUnitStruct, 0, bytes_to_read))
        return false;*/


    ioData(255);                      /* Discard CRC */
    ioData(255);

    return true;                    /* Return with success */
}
//#######################################################################################
unsigned int GI::Dev::MmcSd_Spi::readPage(void* _Buffer, unsigned long block, unsigned int nblks)
{
    if (/*drv || */!nblks)
        return false;       /* Check parameter */
    if (SD_Init_OK == false)
        return false;   /* Check if drive is ready */

    if (sdType == IsSd)
        block *= 512;   /* LBA ot BA conversion (byte addressing cards) */

    if (nblks == 1)
    {   /* Single sector read */
        if ((sendCmd(CMD17, block) == 0)    /* READ_SINGLE_BLOCK */
            && rcvrDatablock((unsigned char *)_Buffer, 512))
            nblks = 0;
    }
    else
    {               /* Multiple sector read */
        unsigned char* Buffer = (unsigned char*)_Buffer;
        if (sendCmd(CMD18, block) == 0)
        {   /* READ_MULTIPLE_BLOCK */
            do
            {
                if (!rcvrDatablock(Buffer, 512))
                    break;
                Buffer += 512;
            } while (--nblks);
            sendCmd(CMD12, 0);              /* STOP_TRANSMISSION */
        }
    }
    deselect();
    return nblks ? false : true;    /* Return result */
}
//#######################################################################################
unsigned int GI::Dev::MmcSd_Spi::mmcSdSpiRead(void *handler, void* _Buffer, unsigned long _block, unsigned int nblks)
{
    GI::Dev::MmcSd_Spi *intHandler = (GI::Dev::MmcSd_Spi *)handler;
    unsigned long block = _block;
    unsigned char* Buffer = (unsigned char*)_Buffer;
    //SD_Struct_t * = (SD_Struct_t *)_SD_Struct;
    //if(sdType == IsSdhc)
    //{
    //  if(!readPage(Buffer, block, nblks))
    //      return false;
    //}
    //else
    {
        do
        {
            if(!intHandler->readPage(Buffer, block++, 1))
                return false;
            Buffer += 512;
        } while (--nblks);
    }
    return true;
}
//#######################################################################################
/* Send multiple byte */
void GI::Dev::MmcSd_Spi::spiMulti (
    const unsigned char *buff,   /* Pointer to the data */
    unsigned long btx            /* Number of bytes to send (512) */
)
{
    do {                        // Transmit data block
        ioData(*buff++);
    } while (btx -= 1);
    /*HardUnitStruct->CsSelect = SpiInstance;
    HardUnitStruct->DisableCsHandle = true;
    HardUnitStruct->Buff = (unsigned char *)buff;
    HardUnitReadWriteBuffFunc(HardUnitStruct, btx, 0);*/

}
/*-----------------------------------------------------------------------*/
/* Send a data packet to the MMC                                         */
/*-----------------------------------------------------------------------*/

int GI::Dev::MmcSd_Spi::xmitDatablock (    /* 1:OK, 0:Failed */
    const unsigned char *buff,   /* Ponter to 512 byte data to be sent */
    unsigned char token          /* Token */
)
{
    unsigned char resp;


    if (!waitReady(50000))
        return 0;       /* Wait for card ready */

    ioData(token);                   /* Send token */
    if (token != 0xFD) {                /* Send data if token is other than StopTran */
        spiMulti(buff, 512);       /* Data */
        //unsigned short crc = mmcsd_crc16(buff, 512);
        ioData(0xFF);
        ioData(0xFF);    /* Dummy CRC */

        resp = ioData(0xFF);             /* Receive data resp */
        if ((resp & 0x1F) != 0x05)      /* Function fails if the data packet was not accepted */
            return 0;
    }
    return 1;
}


unsigned int GI::Dev::MmcSd_Spi::writePage(void* _Buffer, unsigned long block, unsigned int nblks)
{
    if (/*drv || */!nblks)
        return false;       /* Check parameter */
    if (SD_Init_OK == false)
        return false;   /* Check if drive is ready */

    if (sdType == IsSd)
        block *= 512;   /* LBA ot BA conversion (byte addressing cards) */

    if (nblks == 1)
    {   /* Single sector write */
        if ((sendCmd(CMD24, block) == 0)    /* WRITE_BLOCK */
            && xmitDatablock((const unsigned char *)_Buffer, 0xFE))
            nblks = 0;
    }
    else {              /* Multiple sector write */
        if (sdType == IsSd)
            sendCmd(ACMD23, nblks); /* Predefine number of sectors */
        if (sendCmd(CMD25, block) == 0)
        {   /* WRITE_MULTIPLE_BLOCK */
            unsigned char* Buffer = (unsigned char*)_Buffer;
            do
            {
                if (!xmitDatablock(Buffer, 0xFC))
                    break;
                Buffer += 512;
            } while (--nblks);
            if (!xmitDatablock(0, 0xFD))    /* STOP_TRAN token */
                nblks = 1;
        }
    }
    deselect();
    return nblks ? false : true;    /* Return result */
}
//#######################################################################################
unsigned int GI::Dev::MmcSd_Spi::mmcSdSpiWrite(void *handler, void* _Buffer, unsigned long _block, unsigned int nblks)
{
    GI::Dev::MmcSd_Spi *intHandler = (GI::Dev::MmcSd_Spi *)handler;
    unsigned long block = _block;
    unsigned char* Buffer = (unsigned char*)_Buffer;
    //SD_Struct_t * = (SD_Struct_t *)_SD_Struct;
    //if(sdType == IsSdhc)
    //{
    //  if(!writePage(Buffer, block, nblks))
    //      return false;
    //}
    //else
    {
        do
        {
            if(!intHandler->writePage(Buffer, block++, 1))
                return false;
            Buffer += 512;
        } while (--nblks);
    }
    return true;
}
//#######################################################################################
void GI::Dev::MmcSd_Spi::mmcSdSpiIoctl(void *handler, unsigned int  command,  unsigned int *buffer)
{
    GI::Dev::MmcSd_Spi *intHandler = (GI::Dev::MmcSd_Spi *)handler;
    switch(command)
    {

        case GET_SECTOR_COUNT:
        {
           //if(_SdCtrlStruct->CardType == MMCSD_CARD_SD) *buffer = _SdCtrlStruct->card->nBlks;
           //else if(_SdCtrlStruct->card->cardType == MMCSD_CARD_MMC) *buffer = extCsd.sec_count;
           //else *buffer = 0;
           *buffer = intHandler->nBlks;

            break;
        }
        case GET_SECTOR_SIZE:
        {
            *buffer = intHandler->blkLen;

            break;
        }
        case CTRL_SYNC :        /* Make sure that no pending write process */
            intHandler->select();
            if (intHandler->waitReady(5000) == 0xFF)
                *buffer = RES_OK;
            break;
       default:
        {
            *buffer = 0;
            break;
        }

    }
}
//#######################################################################################
void GI::Dev::MmcSd_Spi::mmcSdSpiIdle(void *handler)
{
    GI::Dev::MmcSd_Spi *intHandler = (GI::Dev::MmcSd_Spi *)handler;
    if(intHandler->unitNr > 9)
        return;
    if(!intHandler->sdPresent || intHandler->sdPresent->in() == false)
    {
        if(intHandler->initFlg)
        {
            intHandler->initFlg = 0;
            GI::Sys::Timer::delay(400);
            if(intHandler->initCard())
            {
                intHandler->connected = true;
                intHandler->mmcFatFs.drv_rw_func.DriveStruct = (void *)intHandler;
                intHandler->mmcFatFs.drv_rw_func.drv_r_func = mmcSdSpiRead;
                intHandler->mmcFatFs.drv_rw_func.drv_w_func = mmcSdSpiWrite;
                intHandler->mmcFatFs.drv_rw_func.drv_ioctl_func = mmcSdSpiIoctl;
#if (_FFCONF == 82786)
                char drv_name_buff[4];
                if(!f_mount(3 + unit_nr, &g_s_mmcFatFs))
#else
                char drv_name_buff[9];
                drv_name_buff[0] = 'S';
                drv_name_buff[1] = 'P';
                drv_name_buff[2] = 'I';
                drv_name_buff[3] = 'S';
                drv_name_buff[4] = 'D';
                drv_name_buff[5] = '1' + intHandler->unitNr;
                drv_name_buff[6] = ':';
                drv_name_buff[7] = '\0';
                if(!f_mount(&intHandler->mmcFatFs, drv_name_buff, 1))
#endif
                {
#if (_FFCONF == 82786)
                    drv_name_buff[0] = '0' + 3 + unit_nr;
                    drv_name_buff[1] = ':';
                    drv_name_buff[2] = '/';
                    drv_name_buff[3] = '\0';
#else
                    drv_name_buff[7] = '/';
                    drv_name_buff[8] = '\0';
#endif
                   DIR g_sDirObject;
                   if(f_opendir(&g_sDirObject, drv_name_buff) == FR_OK)
                    {
                       intHandler->fs_mounted = true;
#if (MMCSD_DEBUG_EN == true)
                            GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"MMCSD_SPI%d drive %d mounted\n\r" , intHandler->unitNr , intHandler->unitNr);
                            GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"MMCSD_SPI%d Fat fs detected\n\r" , intHandler->unitNr);
                            GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"MMCSD_SPI%d Fs type:                 " , intHandler->unitNr);
                            if(intHandler->mmcFatFs.fs_type == FS_FAT12) {               	GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"Fat12");}
                            else if(intHandler->mmcFatFs.fs_type == FS_FAT16){           	GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"Fat16");}
                            else if(intHandler->mmcFatFs.fs_type == FS_FAT32){           	GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"Fat32");}
                            else if(intHandler->mmcFatFs.fs_type == FS_EXFAT){           	GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"exFat");}
                            else                                {                GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"None");}
                            GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"\n\r");
                            GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"MMCSD_SPI%d BytesPerSector:          %d \n\r",intHandler->unitNr + 3, /*(int)g_sFatFs.s_size*/512);
                            GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"MMCSD_SPI%d SectorsPerCluster:       %d \n\r",intHandler->unitNr + 3, (int)intHandler->mmcFatFs.csize);
                            GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"MMCSD_SPI%d NumberOfFats:            %d \n\r",intHandler->unitNr + 3, (int)intHandler->mmcFatFs.n_fats);
                            GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"MMCSD_SPI%d DataSectionBegin:        %d \n\r",intHandler->unitNr + 3, (int)intHandler->mmcFatFs.fatbase);
                            unsigned long tmp = (unsigned long long)((unsigned long long)intHandler->mmcFatFs.n_fatent * (unsigned long long)512 *(unsigned long long)intHandler->mmcFatFs.csize) >> 20;
                            GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"MMCSD_SPI%d uSD DiskCapacity:        %uMB\n\r",intHandler->unitNr + 3, tmp);
#endif
                    }
                   else     GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"MMCSD_SPI%d ERROR oppening path\n\r" , intHandler->unitNr);
                }

                else        GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"MMCSD_SPI%d ERROR mounting disk\n\r" , intHandler->unitNr);
            }

            else            GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"MMCSD_SPI%d not detected\n\r" , intHandler->unitNr);
        }
    }
    else
    {
        GI::Sys::Timer::delay(1);
        if(intHandler->initFlg != 1)
        {
            intHandler->fs_mounted = false;
            intHandler->connected = false;
            intHandler->initFlg = 1;
#ifdef MMCSD_DEBUG_EN
            GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"MMCSD_SPI%d Disconnected\n\r" , intHandler->unitNr);
#endif
        }
    }
}
//#######################################################################################
void GI::Dev::MmcSd_Spi::mmcSdGetRawCid(void *handler, unsigned long *rawCid)
{
	GI::Dev::MmcSd_Spi *intHandler = (GI::Dev::MmcSd_Spi *)handler;
	rawCid[0] = intHandler->raw_csd[0];
	rawCid[1] = intHandler->raw_csd[1];
	rawCid[2] = intHandler->raw_csd[2];
	rawCid[3] = intHandler->raw_csd[3];
}

void GI::Dev::MmcSd_Spi::mmcSdGetRawCsd(void *handler, unsigned long *rawCsd)
{
	GI::Dev::MmcSd_Spi *intHandler = (GI::Dev::MmcSd_Spi *)handler;
	rawCsd[0] = intHandler->raw_cid[0];
	rawCsd[1] = intHandler->raw_cid[1];
	rawCsd[2] = intHandler->raw_cid[2];
	rawCsd[3] = intHandler->raw_cid[3];
}
//#######################################################################################
GI::Dev::MmcSd_Spi::MmcSd_Spi(unsigned int unitNr, char *spiPath, char *cdPath, char *ledPath)
{
    memset(this, 0, sizeof(*this));
    GI::Dev::DevRequest::request(spiPath, &spiUnit);
    GI::Dev::DevRequest::request(ledPath, &led);
    GI::Dev::DevRequest::request(cdPath, &sdPresent);
    idle_Ptr = &GI::Dev::MmcSd_Spi::mmcSdSpiIdle;
    read_Ptr = &GI::Dev::MmcSd_Spi::mmcSdSpiRead;
    write_Ptr = &GI::Dev::MmcSd_Spi::mmcSdSpiWrite;
    ioctl_Ptr = &GI::Dev::MmcSd_Spi::mmcSdSpiIoctl;
    getRawCid_Ptr = &GI::Dev::MmcSd_Spi::mmcSdGetRawCid;
    getRawCsd_Ptr = &GI::Dev::MmcSd_Spi::mmcSdGetRawCsd;
    this->unitNr = unitNr;
    driverHandler_Ptr = (void*)this;
    if(!maxSpeed)
    	maxSpeed = 25000000;
    initCard();
    initFlg = true;
}

GI::Dev::MmcSd_Spi::~MmcSd_Spi()
{

}
//#######################################################################################



