#include <string.h>
#include <interface/mmcsd.h>
#include "api/init.h"
#include <api/dev_request.h>
#include "lib/fs/fat.h"

/**
 * @brief  Mask for errors Card Status R1 (OCR Register)
 */
#define SD_OCR_ADDR_OUT_OF_RANGE        ((unsigned long)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((unsigned long)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((unsigned long)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((unsigned long)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((unsigned long)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((unsigned long)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((unsigned long)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((unsigned long)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((unsigned long)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((unsigned long)0x00200000)
#define SD_OCR_CC_ERROR                 ((unsigned long)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((unsigned long)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN     ((unsigned long)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((unsigned long)0x00020000)
//#define SD_OCR_CID_CSD_OVERWRIETE       ((unsigned long)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((unsigned long)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((unsigned long)0x00004000)
#define SD_OCR_ERASE_RESET              ((unsigned long)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR            ((unsigned long)0x00000008)
#define SD_OCR_ERRORBITS                ((unsigned long)0xFDFFE008)

/**
 * @brief  Masks for R6 Response
 */
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((unsigned long)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((unsigned long)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((unsigned long)0x00008000)

#define SD_VOLTAGE_WINDOW_SD            ((unsigned long)0x80100000)
#define SD_HIGH_CAPACITY                ((unsigned long)0x40000000)
#define SD_STD_CAPACITY                 ((unsigned long)0x00000000)
//#define SD_CHECK_PATTERN                ((unsigned long)0x000001AA)

#define SD_MAX_VOLT_TRIAL               ((unsigned long)0x0000FFFF)
#define SD_ALLZERO                      ((unsigned long)0x00000000)

#define SD_WIDE_BUS_SUPPORT             ((unsigned long)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((unsigned long)0x00010000)
#define SD_CARD_LOCKED                  ((unsigned long)0x02000000)

//#define SD_DATATIMEOUT                  ((unsigned long)0xFFFFFFFF)
#define SD_0TO7BITS                     ((unsigned long)0x000000FF)
#define SD_8TO15BITS                    ((unsigned long)0x0000FF00)
#define SD_16TO23BITS                   ((unsigned long)0x00FF0000)
#define SD_24TO31BITS                   ((unsigned long)0xFF000000)
#define SD_MAX_DATA_LENGTH              ((unsigned long)0x01FFFFFF)

#define SD_HALFFIFO                     ((unsigned long)0x00000008)
#define SD_HALFFIFOBYTES                ((unsigned long)0x00000020)

/**
 * @brief  Command Class Supported
 */
#define SD_CCCC_LOCK_UNLOCK             ((unsigned long)0x00000080)
#define SD_CCCC_WRITE_PROT              ((unsigned long)0x00000040)
#define SD_CCCC_ERASE                   ((unsigned long)0x00000020)

/**
 * @brief  Following commands are SD Card Specific commands.
 *         SDIO_APP_CMD should be sent before sending these commands.
 */
#define SDIO_SEND_IF_COND               ((unsigned long)0x00000008)

/**
 * @}
 */

/** @defgroup STM324xG_EVAL_SDIO_SD_Private_Macros
 * @{
 */
/**
 * @}
 */

/** @defgroup STM324xG_EVAL_SDIO_SD_Private_Variables
 * @{
 */

//HAL_SD_CardInfoTypedef SDCardInfo[1];

/**
 * @}
 */
fatfs *MmcSdFatFs;

#define PATH_BUF_SIZE   6
static char g_cCwdBuf0[PATH_BUF_SIZE] = "SD1:/";


void GI::Dev::MmcSd::ioctl(void *handler, unsigned int command,
		unsigned int *buffer)
{

}

//HAL_SD_ErrorTypedef SD_WriteMultiBlocks(unsigned char *writebuff, unsigned long WriteAddr, uint16_t BlockSize, unsigned long NumberOfBlocks)
//HAL_SD_ErrorTypedef SD_WriteBlock(unsigned char *writebuff, unsigned long WriteAddr, uint16_t BlockSize)
//HAL_SD_ErrorTypedef SD_ReadMultiBlocks(unsigned char *readbuff, unsigned long ReadAddr, uint16_t BlockSize, unsigned long NumberOfBlocks)
//HAL_SD_ErrorTypedef SD_ReadBlock(unsigned char *readbuff, unsigned long ReadAddr, uint16_t BlockSize)

unsigned int GI::Dev::MmcSd::read(void *handler, void *ptr,
		unsigned long block, unsigned int nblks)
{
	return 0;
}

unsigned int GI::Dev::MmcSd::write(void *handler, void *ptr,
		unsigned long block, unsigned int nblks)
{
	return 0;
}

bool GI::Dev::MmcSd::idle(unsigned int unit_nr)
{
	return 0;
}
/*#####################################################*/

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

GI::Dev::MmcSd::MmcSd(unsigned int unit_nr, char *cardDetectPinPath, char *cardStatusLedPinPath) :
		cardDetected(false)
{

}

GI::Dev::MmcSd::~MmcSd()
{
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/*********** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
