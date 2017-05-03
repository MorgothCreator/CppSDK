/*
 * device/mmcsd_spi.cpp
 */

#include "mmcsd_spi.h"
#include <api/dev_request.h>
#include <api/io_handle.h>
#include <api/timer.h>
#include <main.h>

/* MMC/SD command */
#define CMD0    (0)         /* GO_IDLE_STATE */
#define CMD1    (1)         /* SEND_OP_COND (MMC) */
#define CMD2    (2)         /* SEND_CID */
#define ACMD41  (0x80+41)   /* SEND_OP_COND (SDC) */
#define CMD8    (8)         /* SEND_IF_COND */
#define CMD9    (9)         /* SEND_CSD */
#define CMD10   (10)        /* SEND_CID */
#define CMD12   (12)        /* STOP_TRANSMISSION */
#define ACMD13  (0x80+13)   /* SD_STATUS (SDC) */
#define CMD16   (16)        /* SET_BLOCKLEN */
#define CMD17   (17)        /* READ_SINGLE_BLOCK */
#define CMD18   (18)        /* READ_MULTIPLE_BLOCK */
#define CMD23   (23)        /* SET_BLOCK_COUNT (MMC) */
#define ACMD23  (0x80+23)   /* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24   (24)        /* WRITE_BLOCK */
#define CMD25   (25)        /* WRITE_MULTIPLE_BLOCK */
#define CMD32   (32)        /* ERASE_ER_BLK_START */
#define CMD33   (33)        /* ERASE_ER_BLK_END */
#define CMD38   (38)        /* ERASE */
#define CMD55   (55)        /* APP_CMD */
#define CMD58   (58)        /* READ_OCR */
#define CMD59   (59)        /* CRC_ON_OFF */

/* Card type flags (CardType) */
#define CT_MMC      0x01        /* MMC ver 3 */
#define CT_SD1      0x02        /* SD ver 1 */
#define CT_SD2      0x04        /* SD ver 2 */
#define CT_SDC      (CT_SD1|CT_SD2) /* SD */
#define CT_BLOCK    0x08        /* Block addressing */


/*
 * EXT_CSD switch cmd macros
 */

#define EXT_CSD_FLUSH_CACHE             32      /* W */
#define EXT_CSD_CACHE_CTRL              33      /* R/W */
#define EXT_CSD_POWER_OFF_NOTIFICATION  34      /* R/W */
#define EXT_CSD_PACKED_FAILURE_INDEX    35      /* RO */
#define EXT_CSD_PACKED_CMD_STATUS       36      /* RO */
#define EXT_CSD_EXP_EVENTS_STATUS       54      /* RO, 2 bytes */
#define EXT_CSD_EXP_EVENTS_CTRL         56      /* R/W, 2 bytes */
#define EXT_CSD_DATA_SECTOR_SIZE        61      /* R */
#define EXT_CSD_GP_SIZE_MULT            143     /* R/W */
#define EXT_CSD_PARTITION_ATTRIBUTE     156     /* R/W */
#define EXT_CSD_PARTITION_SUPPORT       160     /* RO */
#define EXT_CSD_HPI_MGMT                161     /* R/W */
#define EXT_CSD_RST_N_FUNCTION          162     /* R/W */
#define EXT_CSD_BKOPS_EN                163     /* R/W */
#define EXT_CSD_BKOPS_START             164     /* W */
#define EXT_CSD_SANITIZE_START          165     /* W */
#define EXT_CSD_WR_REL_PARAM            166     /* RO */
#define EXT_CSD_RPMB_MULT               168     /* RO */
#define EXT_CSD_BOOT_WP                 173     /* R/W */
#define EXT_CSD_ERASE_GROUP_DEF         175     /* R/W */
#define EXT_CSD_PART_CONFIG             179     /* R/W */
#define EXT_CSD_ERASED_MEM_CONT         181     /* RO */
#define EXT_CSD_BUS_WIDTH               183     /* R/W */
#define EXT_CSD_HS_TIMING               185     /* R/W */
#define EXT_CSD_POWER_CLASS             187     /* R/W */
#define EXT_CSD_REV                     192     /* RO */
#define EXT_CSD_STRUCTURE               194     /* RO */
#define EXT_CSD_CARD_TYPE               196     /* RO */
#define EXT_CSD_OUT_OF_INTERRUPT_TIME   198     /* RO */
#define EXT_CSD_PART_SWITCH_TIME        199     /* RO */
#define EXT_CSD_PWR_CL_52_195           200     /* RO */
#define EXT_CSD_PWR_CL_26_195           201     /* RO */
#define EXT_CSD_PWR_CL_52_360           202     /* RO */
#define EXT_CSD_PWR_CL_26_360           203     /* RO */
#define EXT_CSD_SEC_CNT                 212     /* RO, 4 bytes */
#define EXT_CSD_S_A_TIMEOUT             217     /* RO */
#define EXT_CSD_REL_WR_SEC_C            222     /* RO */
#define EXT_CSD_HC_WP_GRP_SIZE          221     /* RO */
#define EXT_CSD_ERASE_TIMEOUT_MULT      223     /* RO */
#define EXT_CSD_HC_ERASE_GRP_SIZE       224     /* RO */
#define EXT_CSD_BOOT_MULT               226     /* RO */
#define EXT_CSD_SEC_TRIM_MULT           229     /* RO */
#define EXT_CSD_SEC_ERASE_MULT          230     /* RO */
#define EXT_CSD_SEC_FEATURE_SUPPORT     231     /* RO */
#define EXT_CSD_TRIM_MULT               232     /* RO */
#define EXT_CSD_PWR_CL_200_195          236     /* RO */
#define EXT_CSD_PWR_CL_200_360          237     /* RO */
#define EXT_CSD_PWR_CL_DDR_52_195       238     /* RO */
#define EXT_CSD_PWR_CL_DDR_52_360       239     /* RO */
#define EXT_CSD_BKOPS_STATUS            246     /* RO */
#define EXT_CSD_POWER_OFF_LONG_TIME     247     /* RO */
#define EXT_CSD_GENERIC_CMD6_TIME       248     /* RO */
#define EXT_CSD_CACHE_SIZE              249     /* RO, 4 bytes */
#define EXT_CSD_TAG_UNIT_SIZE           498     /* RO */
#define EXT_CSD_DATA_TAG_SUPPORT        499     /* RO */
#define EXT_CSD_MAX_PACKED_WRITES       500     /* RO */
#define EXT_CSD_MAX_PACKED_READS        501     /* RO */
#define EXT_CSD_BKOPS_SUPPORT           502     /* RO */
#define EXT_CSD_HPI_FEATURES            503     /* RO */

/*
 * EXT_CSD field definitions
 */

#define EXT_CSD_WR_REL_PARAM_EN         (1<<2)

#define EXT_CSD_BOOT_WP_B_PWR_WP_DIS    (0x40)
#define EXT_CSD_BOOT_WP_B_PERM_WP_DIS   (0x10)
#define EXT_CSD_BOOT_WP_B_PERM_WP_EN    (0x04)
#define EXT_CSD_BOOT_WP_B_PWR_WP_EN     (0x01)

#define EXT_CSD_PART_CONFIG_ACC_MASK    (0x7)
#define EXT_CSD_PART_CONFIG_ACC_BOOT0   (0x1)
#define EXT_CSD_PART_CONFIG_ACC_RPMB    (0x3)
#define EXT_CSD_PART_CONFIG_ACC_GP0     (0x4)

#define EXT_CSD_PART_SUPPORT_PART_EN    (0x1)

#define EXT_CSD_CMD_SET_NORMAL          (1<<0)
#define EXT_CSD_CMD_SET_SECURE          (1<<1)
#define EXT_CSD_CMD_SET_CPSECURE        (1<<2)

#define EXT_CSD_CARD_TYPE_26    (1<<0)  /* Card can run at 26MHz */
#define EXT_CSD_CARD_TYPE_52    (1<<1)  /* Card can run at 52MHz */
#define EXT_CSD_CARD_TYPE_MASK  0x3F    /* Mask out reserved bits */
#define EXT_CSD_CARD_TYPE_DDR_1_8V  (1<<2)   /* Card can run at 52MHz */
                                              /* DDR mode @1.8V or 3V I/O */
#define EXT_CSD_CARD_TYPE_DDR_1_2V  (1<<3)   /* Card can run at 52MHz */
                                              /* DDR mode @1.2V I/O */
#define EXT_CSD_CARD_TYPE_DDR_52       (EXT_CSD_CARD_TYPE_DDR_1_8V  \
                                         | EXT_CSD_CARD_TYPE_DDR_1_2V)
 #define EXT_CSD_CARD_TYPE_SDR_1_8V      (1<<4)  /* Card can run at 200MHz */
#define EXT_CSD_CARD_TYPE_SDR_1_2V      (1<<5)  /* Card can run at 200MHz */
                                                 /* SDR mode @1.2V I/O */

#define EXT_CSD_BUS_WIDTH_1     0       /* Card is in 1 bit mode */
#define EXT_CSD_BUS_WIDTH_4     1       /* Card is in 4 bit mode */
#define EXT_CSD_BUS_WIDTH_8     2       /* Card is in 8 bit mode */
#define EXT_CSD_DDR_BUS_WIDTH_4 5       /* Card is in 4 bit DDR mode */
#define EXT_CSD_DDR_BUS_WIDTH_8 6       /* Card is in 8 bit DDR mode */

#define EXT_CSD_SEC_ER_EN       BIT(0)
#define EXT_CSD_SEC_BD_BLK_EN   BIT(2)
#define EXT_CSD_SEC_GB_CL_EN    BIT(4)
#define EXT_CSD_SEC_SANITIZE    BIT(6)  /* v4.5 only */

#define EXT_CSD_RST_N_EN_MASK   0x3
#define EXT_CSD_RST_N_ENABLED   1       /* RST_n is enabled on card */

#define EXT_CSD_NO_POWER_NOTIFICATION   0
#define EXT_CSD_POWER_ON                1
#define EXT_CSD_POWER_OFF_SHORT         2
#define EXT_CSD_POWER_OFF_LONG          3

#define EXT_CSD_PWR_CL_8BIT_MASK        0xF0    /* 8 bit PWR CLS */
#define EXT_CSD_PWR_CL_4BIT_MASK        0x0F    /* 8 bit PWR CLS */
#define EXT_CSD_PWR_CL_8BIT_SHIFT       4
#define EXT_CSD_PWR_CL_4BIT_SHIFT       0

#define EXT_CSD_PACKED_EVENT_EN BIT(3)

/*
 * EXCEPTION_EVENT_STATUS field
 */
#define EXT_CSD_URGENT_BKOPS            BIT(0)
#define EXT_CSD_DYNCAP_NEEDED           BIT(1)
#define EXT_CSD_SYSPOOL_EXHAUSTED       BIT(2)
#define EXT_CSD_PACKED_FAILURE          BIT(3)

#define EXT_CSD_PACKED_GENERIC_ERROR    BIT(0)
#define EXT_CSD_PACKED_INDEXED_ERROR    BIT(1)


/*
 * MMC_SWITCH access modes
 *
 * The SWITCH command response is of type R1b, therefore, the host should read the card status, using
 * SEND_STATUS command, after the busy signal is de-asserted, to check the result of the SWITCH
 * operation.
 */
#define MMC_SWITCH_MODE_CMD_SET         0x00    /* The command set is changed according to the Cmd Set field of the argument */
#define MMC_SWITCH_MODE_SET_BITS        0x01    /* The bits in the pointed byte are set, according to the ‘1’ bits in the Value field. */
#define MMC_SWITCH_MODE_CLEAR_BITS      0x02    /* The bits in the pointed byte are cleared, according to the ‘1’ bits in the Value field. */
#define MMC_SWITCH_MODE_WRITE_BYTE      0x03    /* The Value field is written into the pointed byte. */


static unsigned int mmcsd_spi_unstuffBits(unsigned int *resp,unsigned int start,unsigned int size)
{
    const int __size = size;
    const unsigned int __mask = (__size < 32 ? 1 << __size : 0) - 1;
    const int __off = 3 - ((start) / 32);
    const int __shft = (start) & 31;
    unsigned int __res;

    __res = resp[__off] >> __shft;
    if (__size + __shft > 32)
    {
        __res |= resp[__off-1] << ((32 - __shft) % 32);
    }

    return(__res & __mask);
}




void GI::Dev::MmcSd_Spi::decodeExtCsd(unsigned char *buffer)
{
    extCsd.s_cmd_set                  = buffer[504];
    extCsd.hpi_features                   = buffer[503];// [503] HPI features
    extCsd.bkops_support              = buffer[502];// [502] Background operations support
    // [501:247] Reserved
    extCsd.bkops_status                   = buffer[246]; // [246] Background operations status
    extCsd.correctly_prg_sectors_num  =(buffer[245] << 24) | (buffer[244] << 16) | (buffer[243] << 8) | buffer[242];  // [245:242] Number of correctly programmed sectors
    extCsd.ini_timeout_ap             = buffer[241]; // [241] 1st initialization time after partitioning
    // [240] Reserved
    extCsd.pwr_cl_ddr_52_360          = buffer[239];  // [239] Power class for 52MHz, DDR at 3.6V
    extCsd.pwr_cl_ddr_52_195          = buffer[238];  // [238] Power class for 52MHz, DDR at 1.95V
    // [237:236]
    extCsd.min_perf_ddr_w_8_52            = buffer[235];// [235] Minimum Write Performance for 8bit at 52MHz in DDR mode
    extCsd.min_perf_ddr_r_8_52            = buffer[234];// [234] Minimum Read Performance for 8bit at 52MHz in DDR mode
    // [233]
    extCsd.trim_mult                  = buffer[232];  // [232] TRIM Multiplier
    extCsd.sec_feature_support            = buffer[231];  // [231] Secure Feature support
    extCsd.sec_erase_mult             = buffer[230];// [230] Secure Erase Multiplier
    extCsd.sec_trim_mult              = buffer[229];  // [229] Secure TRIM Multiplier
    extCsd.boot_info                  = buffer[228];  // [228] Boot information
    // [227] Reserved
    extCsd.boot_size_multi                = buffer[226];// [226] Boot partition size
    extCsd.acc_size                       = buffer[225];// [225] Access size;
    extCsd.hc_erase_grp_size          = buffer[224];  // [224] High-capacity erase unit size
    extCsd.erase_timeout_mult         = buffer[223];// [223] High-capacity erase timeout
    extCsd.rel_wr_sec_c               = buffer[222];  // [222] Reliable write sector count
    extCsd.hc_wp_grp_size             = buffer[221];// [221] High-capacity write protect group size
    extCsd.s_c_vcc                        = buffer[220];// [220] Sleep current (VCC)
    extCsd.s_c_vccq                       = buffer[219];// [219] Sleep current (VCCQ)
    // [218] Reserved
    extCsd.s_a_timeout                    = buffer[217];// [217] Sleep/awake timeout
    // [216] Reserved
    extCsd.sec_count                  = (buffer[215] << 24) | (buffer[214] << 16) | (buffer[213] << 8) | buffer[212]; // [215:212] Sector Count
    // [211] Reserved
    extCsd.min_perf_w_8_52                = buffer[210];// [210] Minimum Write Performance for 8bit at 52MHz
    extCsd.min_perf_r_8_52                = buffer[209];// [209] Minimum Read Performance for 8bit at 52MHz
    extCsd.min_perf_w_8_26_4_52           = buffer[208];// [208] Minimum Write Performance for 8bit at 26MHz, for 4bit at 52MHz
    extCsd.min_perf_r_8_26_4_52           = buffer[207];// [207] Minimum Read Performance for 8bit at 26MHz, for 4bit at 52MHz
    extCsd.min_perf_w_4_26                = buffer[206];// [206] Minimum Write Performance for 4bit at 26MHz
    extCsd.min_perf_r_4_26                = buffer[205];// [205] Minimum Read Performance for 4bit at 26MHz
    // [211] Reserved
    extCsd.pwr_cl_26_360              = buffer[203];  // [203] Power class for 26MHz at 3.6V
    extCsd.pwr_cl_52_360              = buffer[202];  // [202] Power class for 52MHz at 3.6V
    extCsd.pwr_cl_26_195              = buffer[201];  // [201] Power class for 26MHz at 1.95V
    extCsd.pwr_cl_52_195              = buffer[200];  // [200] Power class for 52MHz at 1.95V
    extCsd.partition_switch_time      = buffer[199];  // [199] Partition switching timing
    extCsd.out_of_interrupt_time      = buffer[198];  // [198] Out-of-interrupt busy timing
    // [197] Reserved
    extCsd.card_type                  = buffer[196];  // [196] Card type
    // [195] Reserved
    extCsd.csd_structure              = buffer[194];  // [194] CSD structure version
    // [193] Reserved
    extCsd.ext_csd_rev                    = buffer[192];// [192] Extended CSD revision
    extCsd.cmd_set                        = buffer[191];// [191] Command set
    // [190] Reserved
    extCsd.cmd_set_rev                    = buffer[189];// [189] Command set revision
    // [188] Reserved
    extCsd.power_class                    = buffer[187];// [187] Power class
    // [186] Reserved
    extCsd.hs_timing                  = buffer[185];  // [185] High-speed interface timing
    // [184] Reserved
    extCsd.bus_width                  = buffer[183];  // [183] Bus width mode
    // [182] Reserved
    extCsd.erased_mem_cont                = buffer[181];// [181] Erased memory content
    // [180] Reserved
    extCsd.partition_config               = buffer[179];// [179] Partition configuration
    extCsd.boot_config_prot               = buffer[178];// [178] Boot config protection
    extCsd.boot_bus_width             = buffer[177];// [177] Boot bus width1
    // [176] Reserved
    extCsd.erase_group_def                = buffer[175];// [175] High-density erase group definition
    // [174] Reserved
    extCsd.boot_wp                        = buffer[173];// [173] Boot area write protection register
    // [172] Reserved
    extCsd.user_wp                        = buffer[171];// [171] User area write protection register
    // [170] Reserved
    extCsd.fw_config                  = buffer[169];  // [169] FW configuration
    extCsd.rpmb_size_mult             = buffer[168];// [168] RPMB Size
    extCsd.wr_rel_set                     = buffer[167];// [167] Write reliability setting register
    extCsd.wr_rel_param                   = buffer[166];// [166] Write reliability parameter register
    // [165] Reserved
    extCsd.bkops_start                    = buffer[164];// [164] Manually start background operations
    extCsd.bkops_en                       = buffer[163];// [163] Enable background operations handshake
    extCsd.rst_n_function             = buffer[162];// [162] H/W reset function
    extCsd.hpi_mgmt                       = buffer[161];// [161] HPI management
    extCsd.partitioning_support           = buffer[160];// [160] Partitioning Support
    extCsd.max_enh_size_mult          = (buffer[159] << 16) | (buffer[158] << 8) | buffer[157]; // [159:157] Max Enhanced Area Size
    extCsd.partitions_attribute           = buffer[156];// [156] Partitions attribute
    extCsd.partition_setting_completed    = buffer[155];// [155] Paritioning Setting
    extCsd.gp_size_mult[0]                = (buffer[154] << 24) | (buffer[153] << 16) | (buffer[152] << 8) | buffer[151];// [154:143] General Purpose Partition Size
    extCsd.gp_size_mult[1]                = (buffer[150] << 24) | (buffer[149] << 16) | (buffer[148] << 8) | buffer[147];// [154:143] General Purpose Partition Size
    extCsd.gp_size_mult[2]                = (buffer[146] << 24) | (buffer[145] << 16) | (buffer[144] << 8) | buffer[143];// [154:143] General Purpose Partition Size
    extCsd.enh_size_mult              = (buffer[142] << 16) | (buffer[141] << 8) | buffer[140];// [142:140] Enhanced User Data Area Size
    extCsd.enh_start_addr             = (buffer[139] << 24) | (buffer[138] << 16) | (buffer[137] << 8) | buffer[136];// [139:136] Enhanced User Data Start Address
    // [135] Reserved;
    extCsd.sec_bad_blk_mgmnt          = buffer[134];  // [134] Bad Block Management mode
    // [133:0] Reserved

    tranSpeed = SD_CARD0_TRANSPEED(this);
    blkLen = 1 << (SD_CARD0_RDBLKLEN(this));
    nBlks = extCsd.sec_count;
    size = nBlks * blkLen;

}

void GI::Dev::MmcSd_Spi::getCsdPar()
{
    //unsigned int blockNr = 0;
    //unsigned int mult = 0;

    /* Describes the version of the CSD structure. */
    CSDInfo.csd_structure = ((raw_csd[3] & 0xC0000000) >> 30); // [127:126]
    CSDInfo.spec_vers = ((raw_csd[3] & 0x3C000000) >> 26); // [125:122] everything above 4 is reserved
    // [121:120] reserved
    CSDInfo.taac = ((raw_csd[3] & 0x00FF0000) >> 16); // [119:112] Data read access-time 1
    CSDInfo.nsac = ((raw_csd[3] & 0x0000FF00) >> 8); // [111:104] Data read access-time 2 in CLK cycles (NSAC*100)
    CSDInfo.tran_speed = (raw_csd[3] & 0x000000FF); // [103:96] Max. bus clock frequency

    CSDInfo.ccc = ((raw_csd[2] & 0xFF000000) >> 20) | ((raw_csd[2] & 0x00F00000) >> 20); // [95:84] Card command classes
    CSDInfo.read_bl_len = ((raw_csd[2] & 0x000F0000) >> 16); // [83:80] Max. read data block length
    CSDInfo.read_bl_partial = ((raw_csd[2] & 0x00008000) >> 15); // [79:79] Partial blocks for read allowed
    CSDInfo.write_blk_misalign = ((raw_csd[2] & 0x00004000) >> 14); // [78:78] WRITE_BLK_MISALIGN
    CSDInfo.read_blk_misalign = ((raw_csd[2] & 0x00002000) >> 13); // [77:77] READ_BLK_MISALIGN
    CSDInfo.dsr_imp = ((raw_csd[2] & 0x00001000) >> 12); // [76:76] DSR implemented
    // [75:74] reserved
    CSDInfo.c_size = ((raw_csd[2] & 0x000003FF) << 2) | ((raw_csd[1] & 0xC0000000) >> 30); // [73:62] Device size
    CSDInfo.vdd_r_curr_min = ((raw_csd[1] & 0x38000000) >> 27); // [61:59] Max. read current @ VDD min
    CSDInfo.vdd_r_curr_max = ((raw_csd[1] & 0x07000000) >> 24); // [58:56] Max. read current @ VDD max
    CSDInfo.vdd_w_curr_min = ((raw_csd[1] & 0x00E00000) >> 21); // [55:53] Max. write current @ VDD min
    CSDInfo.vdd_w_curr_max = ((raw_csd[1] & 0x001C0000) >> 18); // [52:50] Max. write current @ VDD max
    CSDInfo.c_size_mult = ((raw_csd[1] & 0x00038000) >> 15); // [49:47] Device size multiplier
    CSDInfo.erase_grp_size = ((raw_csd[1] & 0x00007C00) >> 10); // [46:42] Erase group size
    CSDInfo.erase_grp_mult = ((raw_csd[1] & 0x000003E0) >> 5); // [41:37] Erase group size multiplier
    CSDInfo.wp_grp_size = (raw_csd[1] & 0x0000001F); // [36:32] Write protect group size
    CSDInfo.wp_grp_enable = ((raw_csd[0] & 0x80000000) >> 31); // [31:31] WP_GRP_ENABLE
    CSDInfo.default_ecc = ((raw_csd[0] & 0x60000000) >> 29); // [30:29] Manufacturer default ECC
    CSDInfo.r2w_factor = ((raw_csd[0] & 0x1C000000) >> 26); // [28:26] Write speed factor
    CSDInfo.write_bl_len = ((raw_csd[0] & 0x03C00000) >> 22); // [25:22] Max. write data block length
    CSDInfo.write_bl_partial = ((raw_csd[0] & 0x00200000) >> 21); // [21:21] Partial blocks for write allowed
    // [20:17]
    CSDInfo.content_prot_app = ((raw_csd[0] & 0x00010000) >> 16); // [16:16] Content protection application
    CSDInfo.file_format_grp = ((raw_csd[0] & 0x00008000) >> 13); // [15:15] File format group
    CSDInfo.copy = ((raw_csd[0] & 0x00004000) >> 12); // [14:14] Copy flag (OTP)
    CSDInfo.perm_write_protect = ((raw_csd[0] & 0x00002000) >> 11); // [13:13] Permanent write protection
    CSDInfo.tmp_write_protect = ((raw_csd[0] & 0x00001000) >> 10); // [12:12] Temporary write protection
    CSDInfo.file_format = ((raw_csd[0] & 0x00000C00) >> 10); // [11:10] File format
    CSDInfo.ecc = ((raw_csd[0] & 0x00000300) >> 8); // [9:8] ECC code
    CSDInfo.crc = (raw_csd[0] & 0x000000FE); // [7:1] CRC
    // [0:0] Not used, always’1’


    /*mult = 2^(CSDInfo.c_size + 2);
    blockNr = (CSDInfo.c_size + 1) * mult;
    size = blockNr * mult;


    //card->blkLen = 1 << (CSDInfo.read_bl_len);*/
    //blkLen = 1 << (CSDInfo.read_bl_len - 1); // Set it to 512 /////////////////////////////////////////////////////////

    //nBlks = size/blkLen;

    //tranSpeed = CSDInfo.tran_speed ;
}


void GI::Dev::MmcSd_Spi::getCID()
{
    unsigned int temp[4];
    int i;

    // UNSTUFF_BITS() read in a reverse order so use a temp buffer
    for(i=0; i<4; i++)
    {
        temp[3-i] = raw_cid[i];
    }

    unsigned int *resp = temp;

    cardCid.mid   = (char)mmcsd_spi_unstuffBits(resp,120,8);
    cardCid.cbx   = (char)mmcsd_spi_unstuffBits(resp,112,2);
    cardCid.oid   = (char)mmcsd_spi_unstuffBits(resp,102,8);
    cardCid.pnm   = mmcsd_spi_unstuffBits(resp,56,48); // This value is not correct!
    cardCid.prv   = (char)mmcsd_spi_unstuffBits(resp,48,8);
    cardCid.psn   = mmcsd_spi_unstuffBits(resp,16,32);
    cardCid.mdt   = (char)mmcsd_spi_unstuffBits(resp,8,8);
    cardCid.crc   = (char)mmcsd_spi_unstuffBits(resp,1,7);
}
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
            raw_csd[0] = csd[15] + (csd[14] << 8) +  (csd[13] << 16) +  (csd[12] << 24);
            raw_csd[1] = csd[11] + (csd[10] << 8) +  (csd[9] << 16) +  (csd[8] << 24);
            raw_csd[2] = csd[7] + (csd[6] << 8) +  (csd[5] << 16) +  (csd[4] << 24);
            raw_csd[3] = csd[3] + (csd[2] << 8) +  (csd[1] << 16) +  (csd[0] << 24);
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
    this->unitNr = unitNr;
    driverHandler_Ptr = (void*)this;
    initCard();
    initFlg = true;
}

GI::Dev::MmcSd_Spi::~MmcSd_Spi()
{

}
//#######################################################################################



