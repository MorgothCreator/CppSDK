/*
 * mmcsd_common.h
 *
 *  Created on: Oct 31, 2017
 *      Author: Eu
 */

#ifndef LIB_API_MMCSD_COMMON_H_
#define LIB_API_MMCSD_COMMON_H_

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

/* Card type flags (CardType) */
#define CT_MMC      0x01        /* MMC ver 3 */
#define CT_SD1      0x02        /* SD ver 1 */
#define CT_SD2      0x04        /* SD ver 2 */
#define CT_SDC      (CT_SD1|CT_SD2) /* SD */
#define CT_BLOCK    0x08        /* Block addressing */

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


typedef enum
{
	IsNoCard = 0, IsSd, IsSdhc,
} mmcsd_type_e;

#define RAW_CSD(name) unsigned long name[4]
#define RAW_CID(name) unsigned long name[4]

typedef struct cardCSD_s
{
	char csd_structure;
	char spec_vers;
	char taac;
	char nsac;
	char tran_speed;
	unsigned short ccc;
	char read_bl_len;
	char read_bl_partial;
	char write_blk_misalign;
	char read_blk_misalign;
	char dsr_imp;
	unsigned short c_size;
	char vdd_r_curr_min;
	char vdd_r_curr_max;
	char vdd_w_curr_min;
	char vdd_w_curr_max;
	char c_size_mult;
	char erase_grp_size;
	char erase_grp_mult;
	char wp_grp_size;
	char wp_grp_enable;
	char default_ecc;
	char r2w_factor;
	char write_bl_len;
	char write_bl_partial;
	char content_prot_app;
	char file_format_grp;
	char copy;
	char perm_write_protect;
	char tmp_write_protect;
	char file_format;
	char ecc;
	char crc;
} CSDInfo_t;


typedef struct cardCid_s {
	char mid; /* Manufacturer ID */
	char cbx; /* Device/BGA */
	char oid; /* OEM/Application ID */
	unsigned long int pnm;      /* Product name */
	char prv; /* Product revision  */
	unsigned long psn; /* Product serial number */
	char mdt; /* Manufacturing date */
	char crc; /* crc7 checksum */
}cardCid_t;

typedef struct extCsd_s {
	// [511:505] Reserved
	char s_cmd_set;       // [504] Supported Command Sets
	char hpi_features;    // [503] HPI features
	char bkops_support;   // [502] Background operations support
	// [501:247] Reserved
	char bkops_status;    // [246] Background operations status
	unsigned int correctly_prg_sectors_num; // [245:242] Number of correctly programmed sectors
	char ini_timeout_ap;  // [241] 1st initialization time after partitioning
	// [240] Reserved
	char pwr_cl_ddr_52_360;        // [239] Power class for 52MHz, DDR at 3.6V
	char pwr_cl_ddr_52_195;        // [238] Power class for 52MHz, DDR at 1.95V
	// [237:236]
	char min_perf_ddr_w_8_52;      // [235] Minimum Write Performance for 8bit at 52MHz in DDR mode
	char min_perf_ddr_r_8_52;      // [234] Minimum Read Performance for 8bit at 52MHz in DDR mode
	// [233]
	char trim_mult;       // [232] TRIM Multiplier
	char sec_feature_support;      // [231] Secure Feature support
	char sec_erase_mult;  // [230] Secure Erase Multiplier
	char sec_trim_mult;   // [229] Secure TRIM Multiplier
	char boot_info;       // [228] Boot information
	// [227] Reserved
	char boot_size_multi; // [226] Boot partition size
	char acc_size;        // [225] Access size;
	char hc_erase_grp_size;        // [224] High-capacity erase unit size
	char erase_timeout_mult;       // [223] High-capacity erase timeout
	char rel_wr_sec_c;    // [222] Reliable write sector count
	char hc_wp_grp_size;  // [221] High-capacity write protect group size
	char s_c_vcc;// [220] Sleep current (VCC)
	char s_c_vccq;        // [219] Sleep current (VCCQ)
	// [218] Reserved
	char s_a_timeout;     // [217] Sleep/awake timeout
	// [216] Reserved
	unsigned int sec_count;        // [215:212] Sector Count
	// [211] Reserved
	char min_perf_w_8_52; // [210] Minimum Write Performance for 8bit at 52MHz
	char min_perf_r_8_52; // [209] Minimum Read Performance for 8bit at 52MHz
	char min_perf_w_8_26_4_52;     // [208] Minimum Write Performance for 8bit at 26MHz, for 4bit at 52MHz
	char min_perf_r_8_26_4_52;     // [207] Minimum Read Performance for 8bit at 26MHz, for 4bit at 52MHz
	char min_perf_w_4_26; // [206] Minimum Write Performance for 4bit at 26MHz
	char min_perf_r_4_26; // [205] Minimum Read Performance for 4bit at 26MHz
	// [211] Reserved
	char pwr_cl_26_360;   // [203] Power class for 26MHz at 3.6V
	char pwr_cl_52_360;   // [202] Power class for 52MHz at 3.6V
	char pwr_cl_26_195;   // [201] Power class for 26MHz at 1.95V
	char pwr_cl_52_195;   // [200] Power class for 52MHz at 1.95V
	char partition_switch_time;    // [199] Partition switching timing
	char out_of_interrupt_time;    // [198] Out-of-interrupt busy timing
	// [197] Reserved
	char card_type;       // [196] Card type
	// [195] Reserved
	char csd_structure;   // [194] CSD structure version
	// [193] Reserved
	char ext_csd_rev;     // [192] Extended CSD revision
	char cmd_set;// [191] Command set
	// [190] Reserved
	char cmd_set_rev;     // [189] Command set revision
	// [188] Reserved
	char power_class;     // [187] Power class
	// [186] Reserved
	char hs_timing;       // [185] High-speed interface timing
	// [184] Reserved
	char bus_width;       // [183] Bus width mode
	// [182] Reserved
	char erased_mem_cont; // [181] Erased memory content
	// [180] Reserved
	char partition_config;// [179] Partition configuration
	char boot_config_prot;// [178] Boot config protection
	char boot_bus_width;  // [177] Boot bus width1
	// [176] Reserved
	char erase_group_def; // [175] High-density erase group definition
	// [174] Reserved;
	char boot_wp;// [173] Boot area write protection register
	// [172] Reserved;
	char user_wp;// [171] User area write protection register
	// [170] Reserved;
	char fw_config;       // [169] FW configuration
	char rpmb_size_mult;  // [168] RPMB Size
	char wr_rel_set;      // [167] Write reliability setting register
	char wr_rel_param;    // [166] Write reliability parameter register
	// [165] Reserved;
	char bkops_start;     // [164] Manually start background operations
	char bkops_en;        // [163] Enable background operations handshake
	char rst_n_function;  // [162] H/W reset function
	char hpi_mgmt;        // [161] HPI management
	char partitioning_support;     // [160] Partitioning Support
	unsigned char max_enh_size_mult;        // [159:157] Max Enhanced Area Size
	char partitions_attribute;     // [156] Partitions attribute
	char partition_setting_completed;       // [155] Paritioning Setting
	unsigned long gp_size_mult[4]; // [154:143] General Purpose Partition Size
	unsigned long enh_size_mult;   // [142:140] Enhanced User Data Area Size
	unsigned long enh_start_addr;  // [139:136] Enhanced User Data Start Address
	// [135] Reserved;
	char sec_bad_blk_mgmnt;// [134] Bad Block Management mode
	// [133:0] Reserved
}extCsd_t;

namespace GI {
namespace Dev {
class MmcSdCommon {
	static void decExtCsd(extCsd_t *extCsd, unsigned char *extCsdRaw);
	static void decCsd(CSDInfo_t *CSDInfo, unsigned long *rawCsd);
	static void decCid(cardCid_t *cardCid, unsigned long *rawCid);
};
}
}


#endif /* LIB_API_MMCSD_COMMON_H_ */
