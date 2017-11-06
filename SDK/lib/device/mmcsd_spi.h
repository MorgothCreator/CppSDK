/*
 * device/mmcsd_spi.h
 */

#ifndef LIB_DEVICE_MMCSD_SPI_H_
#define LIB_DEVICE_MMCSD_SPI_H_

#include <api/spi.h>
#include <api/gpio.h>
#include <api/mmcsd.h>
#include <api/mmcsd_common.h>
#include "lib/fs/fat/inc/ff.h"

/*
 * EXT_CSD struct
 */

/*#####################################################*/
#define _Disk_ErrInit                   1
#define _Disk_ErrReset                  2
#define _Disk_InitOk                    3
#define _Disk_Busy                      4
#define _Disk_DelayWrite                5
#define _Disk_DelayRead                 6
#define _Disk_ReleaseError              7
#define _Disk_ReadWriteOk               8
#define _Disk_ReadInitError             9
#define _Disk_ErrVoltageRange           10
/*#####################################################*/
#define MMC_CMD_RESET                   0
#define MMC_CMD_START_INITIALIZATION    1
#define MMC_SEND_IF_COND                8
#define MMC_CMD_SEND_CSD                9
#define MMC_CMD_SEND_CID                10
#define MMC_CMD_STOP_TRANSMISSION       12
#define MMC_CMD_SEND_STATUS             13
#define MMC_CMD_SET_BLOCK_SIZE          16
#define MMC_CMD_READ_SINGLE_BLOCK       17
#define MMC_CMD_READ_MULTIPLE_BLOCKS    18
#define MMC_CMD_WRITE_SINGLE_BLOCK      24
#define MMC_CMD_WRITE_MULTIPLE_BLOCKS   25
#define MMC_CMD_PROGRAM_CSD             27
#define MMC_CMD_SET_WRITE_PROT          28
#define MMC_CMD_CLR_WRITE_PROT          29
#define MMC_CMD_SEND_WRITE_PROT         30
#define MMC_CMD_TAG_SECTOR_START        32
#define MMC_CMD_TAG_SECTOR_END          33
#define MMC_CMD_UNTAG_SECTOR            34
#define MMC_CMD_TAG_ERASE_GROUP_START   35
#define MMC_CMD_TAG_ERASE_GROUP_END     36
#define MMC_CMD_UNTAG_ERASE_GROUP       37
#define MMC_CMD_ERASE                   38
#define MMC_CMD_SD_SEND_OP_COND         41
#define MMC_CMD_LOCK_UNLOCK             42
#define MMC_CMD_APP_CMD                 55
#define MMC_CMD_READ_OCR                58
#define MMC_CMD_CRC_ON_OFF              59
/*#####################################################*/
// SPI Response Flags
#define MMC_IN_IDLE_STATE               0x01
#define MMC_ERASE_RESET                 0x02
#define MMC_ILLEGAL_COMMAND             0x04
#define MMC_COM_CRC_ERROR               0x08
#define MMC_ERASE_ERROR                 0x10
#define MMC_ADRESS_ERROR                0x20
#define MMC_PARAMETER_ERROR             0x40
/*#####################################################*/
#define SD_RAW_SPEC_1                   0
#define SD_RAW_SPEC_2                   1
#define SD_RAW_SPEC_SDHC                2
/*#####################################################*/
#define MMC_DATA_TOKEN                  0xFE
/*#####################################################*/
#define MMC_RESPONSE_OK                 0x05
#define MMC_RESPONSE_CRC_ERROR          0x0B
#define MMC_RESPONSE_WRITE_ERROR        0x0D
#define MMC_RESPONSE_MASK               0x1F
/*#####################################################*/
#define DelayWriteCommand               65536
#define DelayReadCommand                1024
#define SdCard_NumberToTryResetCmd      100
#define SdCard_NumberToTryIfCondCmd     254
#define SdCard_NumberToTryInitCmdSd     30
#define SdCard_NumberToTryInitCmdSdHc   254
#define SdDriverDelaysConstant          20
#define NumberOfIdleBytes               65536
/*#####################################################*/
#define SD_CMD(x)   (x)

/* Command/Response flags for notifying some information to controller */
#define SD_CMDRSP_NONE          BIT(0)
#define SD_CMDRSP_STOP          BIT(1)
#define SD_CMDRSP_FS            BIT(2)
#define SD_CMDRSP_ABORT         BIT(3)
#define SD_CMDRSP_BUSY          BIT(4)
#define SD_CMDRSP_136BITS       BIT(5)
#define SD_CMDRSP_DATA          BIT(6)
#define SD_CMDRSP_READ          BIT(7)
#define SD_CMDRSP_WRITE         BIT(8)


#define SD_CMDRSP_R1            (0)
#define SD_CMDRSP_R1b           (0 | SD_CMDRSP_BUSY)
#define SD_CMDRSP_R2            (SD_CMDRSP_136BITS)
#define SD_CMDRSP_R3            (SD_CMDRSP_R1)



/* SD voltage enumeration as per VHS field of the interface command */
#define SD_VOLT_2P7_3P6                 (0x000100u)

/* SD OCR register definitions */
/* High capacity */
#define SD_OCR_HIGH_CAPACITY        BIT(30)
/* Voltage */
#define SD_OCR_VDD_2P7_2P8      BIT(15)
#define SD_OCR_VDD_2P8_2P9      BIT(16)
#define SD_OCR_VDD_2P9_3P0      BIT(17)
#define SD_OCR_VDD_3P0_3P1      BIT(18)
#define SD_OCR_VDD_3P1_3P2      BIT(19)
#define SD_OCR_VDD_3P2_3P3      BIT(20)
#define SD_OCR_VDD_3P3_3P4      BIT(21)
#define SD_OCR_VDD_3P4_3P5      BIT(22)
#define SD_OCR_VDD_3P5_3P6      BIT(23)
/* This is for convenience only. Sets all the VDD fields */
#define SD_OCR_VDD_WILDCARD     (0x1FF << 15)

/* SD CSD register definitions */
#define SD_TRANSPEED_25MBPS     (0x32u)
#define SD_TRANSPEED_50MBPS     (0x5Au)

#define SD_CARD_CSD_VERSION(crd) (((crd)->raw_csd[3] & 0xC0000000) >> 30)

#define SD_CSD0_DEV_SIZE(csd3, csd2, csd1, csd0) (((csd2 & 0x000003FF) << 2) | ((csd1 & 0xC0000000) >> 30))
#define SD_CSD0_MULT(csd3, csd2, csd1, csd0) ((csd1 & 0x00038000) >> 15)
#define SD_CSD0_RDBLKLEN(csd3, csd2, csd1, csd0) ((csd2 & 0x000F0000) >> 16)
#define SD_CSD0_TRANSPEED(csd3, csd2, csd1, csd0) ((csd3 & 0x000000FF) >> 0)

#define SD_CARD0_DEV_SIZE(crd) SD_CSD0_DEV_SIZE((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD0_MULT(crd) SD_CSD0_MULT((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD0_RDBLKLEN(crd) SD_CSD0_RDBLKLEN((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD0_TRANSPEED(crd) SD_CSD0_TRANSPEED((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD0_NUMBLK(crd) ((SD_CARD0_DEV_SIZE((crd)) + 1) * (1 << (SD_CARD0_MULT((crd)) + 2)))
#define SD_CARD0_SIZE(crd) ((SD_CARD0_NUMBLK((crd))) * (1 << (SD_CARD0_RDBLKLEN(crd))))

#define SD_CSD1_DEV_SIZE(csd3, csd2, csd1, csd0) (((csd2 & 0x0000003F) << 16) | ((csd1 & 0xFFFF0000) >> 16))
#define SD_CSD1_RDBLKLEN(csd3, csd2, csd1, csd0) ((csd2 & 0x000F0000) >> 16)
#define SD_CSD1_WRBLKLEN(csd3, csd2, csd1, csd0) ((csd0 & 0x03C00000) >> 22)
#define SD_CSD1_TRANSPEED(csd3, csd2, csd1, csd0) ((csd3 & 0x000000FF) >> 0)

#define SD_CARD1_DEV_SIZE(crd) SD_CSD1_DEV_SIZE((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD1_RDBLKLEN(crd) SD_CSD1_RDBLKLEN((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD1_WRBLKLEN(crd) SD_CSD1_WRBLKLEN((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD1_TRANSPEED(crd) SD_CSD1_TRANSPEED((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD1_SIZE(crd) ((unsigned long long)(SD_CARD1_DEV_SIZE((crd)) + 1) * (unsigned long long)(524288))


/* Check RCA/status */
#define SD_RCA_ADDR(rca)             ((rca & 0xFFFF0000) >> 16)
#define SD_RCA_STAT(rca)             (rca & 0x0xFFFF)

/* Check pattern that can be used for card response validation */
#define SD_CHECK_PATTERN   0xAA

/* SD SCR related macros */
#define SD_VERSION_1P0      0
#define SD_VERSION_1P1      1
#define SD_VERSION_2P0      2
#define SD_BUS_WIDTH_1BIT   1
#define SD_BUS_WIDTH_4BIT   4
#define SD_BUS_WIDTH_8BIT   8

/* Helper macros */
/* Note card registers are big endian */
#define SD_CARD_VERSION(sdcard)     ((sdcard)->raw_scr[0] & 0xF)
#define SD_CARD_BUSWIDTH(sdcard)    (((sdcard)->raw_scr[0] & 0xF00) >> 8)
#define GET_SD_CARD_BUSWIDTH(sdcard)  ((((sdcard.busWidth) & 0x0F) == 0x01) ? \
                                      0x1 : ((((sdcard).busWidth & 0x04) == \
                                      0x04) ? 0x04 : 0xFF))
#define GET_SD_CARD_FRE(sdcard)       (((sdcard.tranSpeed) == 0x5A) ? 50 : \
                                      (((sdcard.tranSpeed) == 0x32) ? 25 : 0))

/* Cacheline size */
#ifndef SOC_CACHELINE_SIZE
#define SOC_CACHELINE_SIZE         128
#endif

/* CM6 Swith mode arguments for High Speed */
#define SD_SWITCH_MODE        0x80FFFFFF
#define SD_CMD6_GRP1_SEL      0xFFFFFFF0
#define SD_CMD6_GRP1_HS       0x1
/*#####################################################*/

namespace GI {
namespace Dev {
class MmcSd_Spi : public MmcSd{
public:
	MmcSd_Spi(unsigned int unitNr, char *spiPath, char *cdPath, char *ledPath);
	~MmcSd_Spi();
	static void mmcSdSpiIdle(void *handler);
	static unsigned int mmcSdSpiRead(void *handler, void* _Buffer, unsigned long _block, unsigned int nblks);
	static unsigned int mmcSdSpiWrite(void *handler, void* _Buffer, unsigned long _block, unsigned int nblks);
	static void mmcSdSpiIoctl(void *handler, unsigned int  command,  unsigned int *buffer);
	static void mmcSdGetRawCid(void *handler, unsigned long *rawCid);
	static void mmcSdGetRawCsd(void *handler, unsigned long *rawCsd);
	int select();
	int waitReady (unsigned int wt);

	unsigned long blkLen;
	unsigned long nBlks;
	unsigned long long size;
	unsigned char tranSpeed;
	bool SD_Init_OK;
	bool connected;
	unsigned int initFlg;
	mmcsd_type_e sdType;
	GI::Dev::Gpio *sdPresent;
	GI::Dev::Gpio *led;
	//FATFS g_s_mmcFatFs;
	GI::Dev::Spi *spiUnit;
	bool fs_mounted;
	FATFS mmcFatFs;
	unsigned int unitNr;
	unsigned long maxSpeed;
private:
	unsigned char ioData(unsigned char Value);
	void csAssert();
	void csDeassert();
	void deselect();
	unsigned char sendCmd (unsigned char cmd, unsigned long arg);
	bool initCard();
	bool rcvrDatablock(unsigned char *buff, unsigned int bytes_to_read);
	unsigned int readPage(void* _Buffer, unsigned long block, unsigned int nblks);
	void spiMulti ( const unsigned char *buff, unsigned long btx);
	int xmitDatablock ( const unsigned char *buff, unsigned char token);
	unsigned int writePage(void* _Buffer, unsigned long block, unsigned int nblks);

	//unsigned long raw_scr[2];
	unsigned long raw_csd[4];
	unsigned long raw_cid[4];
};
}
}




#endif /* LIB_DEVICE_MMCSD_SPI_H_ */
