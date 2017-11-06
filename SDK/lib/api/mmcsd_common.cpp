/*
 *  api/mmcsd_common.cpp
 */

#include "mmcsd_common.h"

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

void GI::Dev::MmcSdCommon::decExtCsd(extCsd_t *extCsd, unsigned char *extCsdRaw)
{
    extCsd->s_cmd_set						= extCsdRaw[504];
    extCsd->hpi_features					= extCsdRaw[503];// [503] HPI features
    extCsd->bkops_support					= extCsdRaw[502];// [502] Background operations support
    // [501:247] Reserved
    extCsd->bkops_status					= extCsdRaw[246]; // [246] Background operations status
    extCsd->correctly_prg_sectors_num		=((unsigned long)extCsdRaw[245] << 24) | ((unsigned long)extCsdRaw[244] << 16) | ((unsigned long)extCsdRaw[243] << 8) | (unsigned long)extCsdRaw[242];  // [245:242] Number of correctly programmed sectors
    extCsd->ini_timeout_ap					= extCsdRaw[241]; // [241] 1st initialization time after partitioning
    // [240] Reserved
    extCsd->pwr_cl_ddr_52_360				= extCsdRaw[239];  // [239] Power class for 52MHz, DDR at 3.6V
    extCsd->pwr_cl_ddr_52_195				= extCsdRaw[238];  // [238] Power class for 52MHz, DDR at 1.95V
    // [237:236]
    extCsd->min_perf_ddr_w_8_52				= extCsdRaw[235];// [235] Minimum Write Performance for 8bit at 52MHz in DDR mode
    extCsd->min_perf_ddr_r_8_52				= extCsdRaw[234];// [234] Minimum Read Performance for 8bit at 52MHz in DDR mode
    // [233]
    extCsd->trim_mult						= extCsdRaw[232];  // [232] TRIM Multiplier
    extCsd->sec_feature_support				= extCsdRaw[231];  // [231] Secure Feature support
    extCsd->sec_erase_mult					= extCsdRaw[230];// [230] Secure Erase Multiplier
    extCsd->sec_trim_mult					= extCsdRaw[229];  // [229] Secure TRIM Multiplier
    extCsd->boot_info						= extCsdRaw[228];  // [228] Boot information
    // [227] Reserved
    extCsd->boot_size_multi					= extCsdRaw[226];// [226] Boot partition size
    extCsd->acc_size						= extCsdRaw[225];// [225] Access size;
    extCsd->hc_erase_grp_size				= extCsdRaw[224];  // [224] High-capacity erase unit size
    extCsd->erase_timeout_mult				= extCsdRaw[223];// [223] High-capacity erase timeout
    extCsd->rel_wr_sec_c					= extCsdRaw[222];  // [222] Reliable write sector count
    extCsd->hc_wp_grp_size					= extCsdRaw[221];// [221] High-capacity write protect group size
    extCsd->s_c_vcc							= extCsdRaw[220];// [220] Sleep current (VCC)
    extCsd->s_c_vccq						= extCsdRaw[219];// [219] Sleep current (VCCQ)
    // [218] Reserved
    extCsd->s_a_timeout						= extCsdRaw[217];// [217] Sleep/awake timeout
    // [216] Reserved
    extCsd->sec_count						= ((unsigned long)extCsdRaw[215] << 24) | ((unsigned long)extCsdRaw[214] << 16) | ((unsigned long)extCsdRaw[213] << 8) | (unsigned long)extCsdRaw[212]; // [215:212] Sector Count
    // [211] Reserved
    extCsd->min_perf_w_8_52					= extCsdRaw[210];// [210] Minimum Write Performance for 8bit at 52MHz
    extCsd->min_perf_r_8_52					= extCsdRaw[209];// [209] Minimum Read Performance for 8bit at 52MHz
    extCsd->min_perf_w_8_26_4_52			= extCsdRaw[208];// [208] Minimum Write Performance for 8bit at 26MHz, for 4bit at 52MHz
    extCsd->min_perf_r_8_26_4_52			= extCsdRaw[207];// [207] Minimum Read Performance for 8bit at 26MHz, for 4bit at 52MHz
    extCsd->min_perf_w_4_26					= extCsdRaw[206];// [206] Minimum Write Performance for 4bit at 26MHz
    extCsd->min_perf_r_4_26					= extCsdRaw[205];// [205] Minimum Read Performance for 4bit at 26MHz
    // [211] Reserved
    extCsd->pwr_cl_26_360					= extCsdRaw[203];  // [203] Power class for 26MHz at 3.6V
    extCsd->pwr_cl_52_360					= extCsdRaw[202];  // [202] Power class for 52MHz at 3.6V
    extCsd->pwr_cl_26_195					= extCsdRaw[201];  // [201] Power class for 26MHz at 1.95V
    extCsd->pwr_cl_52_195					= extCsdRaw[200];  // [200] Power class for 52MHz at 1.95V
    extCsd->partition_switch_time			= extCsdRaw[199];  // [199] Partition switching timing
    extCsd->out_of_interrupt_time			= extCsdRaw[198];  // [198] Out-of-interrupt busy timing
    // [197] Reserved
    extCsd->card_type						= extCsdRaw[196];  // [196] Card type
    // [195] Reserved
    extCsd->csd_structure					= extCsdRaw[194];  // [194] CSD structure version
    // [193] Reserved
    extCsd->ext_csd_rev						= extCsdRaw[192];// [192] Extended CSD revision
    extCsd->cmd_set							= extCsdRaw[191];// [191] Command set
    // [190] Reserved
    extCsd->cmd_set_rev						= extCsdRaw[189];// [189] Command set revision
    // [188] Reserved
    extCsd->power_class						= extCsdRaw[187];// [187] Power class
    // [186] Reserved
    extCsd->hs_timing						= extCsdRaw[185];  // [185] High-speed interface timing
    // [184] Reserved
    extCsd->bus_width						= extCsdRaw[183];  // [183] Bus width mode
    // [182] Reserved
    extCsd->erased_mem_cont					= extCsdRaw[181];// [181] Erased memory content
    // [180] Reserved
    extCsd->partition_config				= extCsdRaw[179];// [179] Partition configuration
    extCsd->boot_config_prot				= extCsdRaw[178];// [178] Boot config protection
    extCsd->boot_bus_width					= extCsdRaw[177];// [177] Boot bus width1
    // [176] Reserved
    extCsd->erase_group_def					= extCsdRaw[175];// [175] High-density erase group definition
    // [174] Reserved
    extCsd->boot_wp							= extCsdRaw[173];// [173] Boot area write protection register
    // [172] Reserved
    extCsd->user_wp							= extCsdRaw[171];// [171] User area write protection register
    // [170] Reserved
    extCsd->fw_config						= extCsdRaw[169];  // [169] FW configuration
    extCsd->rpmb_size_mult					= extCsdRaw[168];// [168] RPMB Size
    extCsd->wr_rel_set						= extCsdRaw[167];// [167] Write reliability setting register
    extCsd->wr_rel_param					= extCsdRaw[166];// [166] Write reliability parameter register
    // [165] Reserved
    extCsd->bkops_start						= extCsdRaw[164];// [164] Manually start background operations
    extCsd->bkops_en						= extCsdRaw[163];// [163] Enable background operations handshake
    extCsd->rst_n_function					= extCsdRaw[162];// [162] H/W reset function
    extCsd->hpi_mgmt						= extCsdRaw[161];// [161] HPI management
    extCsd->partitioning_support			= extCsdRaw[160];// [160] Partitioning Support
    extCsd->max_enh_size_mult				= ((unsigned long)extCsdRaw[159] << 16) | ((unsigned long)extCsdRaw[158] << 8) | (unsigned long)extCsdRaw[157]; // [159:157] Max Enhanced Area Size
    extCsd->partitions_attribute			= extCsdRaw[156];// [156] Partitions attribute
    extCsd->partition_setting_completed		= extCsdRaw[155];// [155] Paritioning Setting
    extCsd->gp_size_mult[0]					= ((unsigned long)extCsdRaw[154] << 24) | ((unsigned long)extCsdRaw[153] << 16) | ((unsigned long)extCsdRaw[152] << 8) | (unsigned long)extCsdRaw[151];// [154:143] General Purpose Partition Size
    extCsd->gp_size_mult[1]					= ((unsigned long)extCsdRaw[150] << 24) | ((unsigned long)extCsdRaw[149] << 16) | ((unsigned long)extCsdRaw[148] << 8) | (unsigned long)extCsdRaw[147];// [154:143] General Purpose Partition Size
    extCsd->gp_size_mult[2]					= ((unsigned long)extCsdRaw[146] << 24) | ((unsigned long)extCsdRaw[145] << 16) | ((unsigned long)extCsdRaw[144] << 8) | (unsigned long)extCsdRaw[143];// [154:143] General Purpose Partition Size
    extCsd->enh_size_mult					= ((unsigned long)extCsdRaw[142] << 16) | ((unsigned long)extCsdRaw[141] << 8) | (unsigned long)extCsdRaw[140];// [142:140] Enhanced User Data Area Size
    extCsd->enh_start_addr					= ((unsigned long)extCsdRaw[139] << 24) | ((unsigned long)extCsdRaw[138] << 16) | ((unsigned long)extCsdRaw[137] << 8) | (unsigned long)extCsdRaw[136];// [139:136] Enhanced User Data Start Address
    // [135] Reserved;
    extCsd->sec_bad_blk_mgmnt				= extCsdRaw[134];  // [134] Bad Block Management mode
    // [133:0] Reserved

    //tranSpeed = SD_CARD0_TRANSPEED(this);
    //blkLen = 1 << (SD_CARD0_RDBLKLEN(this));
    //nBlks = extCsd->sec_count;
    //size = nBlks * blkLen;
}

void GI::Dev::MmcSdCommon::decCsd(CSDInfo_t *CSDInfo, unsigned long *rawCsd)
{
    /* Describes the version of the CSD structure. */
    CSDInfo->csd_structure		= ((rawCsd[3] & 0xC0000000) >> 30); // [127:126]
    CSDInfo->spec_vers			= ((rawCsd[3] & 0x3C000000) >> 26); // [125:122] everything above 4 is reserved
    // [121:120] reserved
    CSDInfo->taac				= ((rawCsd[3] & 0x00FF0000) >> 16); // [119:112] Data read access-time 1
    CSDInfo->nsac				= ((rawCsd[3] & 0x0000FF00) >> 8); // [111:104] Data read access-time 2 in CLK cycles (NSAC*100)
    CSDInfo->tran_speed			=  (rawCsd[3] & 0x000000FF); // [103:96] Max. bus clock frequency

    CSDInfo->ccc				= ((rawCsd[2] & 0xFF000000) >> 20) | ((rawCsd[2] & 0x00F00000) >> 20); // [95:84] Card command classes
    CSDInfo->read_bl_len		= ((rawCsd[2] & 0x000F0000) >> 16); // [83:80] Max. read data block length
    CSDInfo->read_bl_partial	= ((rawCsd[2] & 0x00008000) >> 15); // [79:79] Partial blocks for read allowed
    CSDInfo->write_blk_misalign	= ((rawCsd[2] & 0x00004000) >> 14); // [78:78] WRITE_BLK_MISALIGN
    CSDInfo->read_blk_misalign	= ((rawCsd[2] & 0x00002000) >> 13); // [77:77] READ_BLK_MISALIGN
    CSDInfo->dsr_imp				= ((rawCsd[2] & 0x00001000) >> 12); // [76:76] DSR implemented
    // [75:74] reserved
    CSDInfo->c_size				= ((rawCsd[2] & 0x000003FF) << 2) | ((rawCsd[1] & 0xC0000000) >> 30); // [73:62] Device size
    CSDInfo->vdd_r_curr_min		= ((rawCsd[1] & 0x38000000) >> 27); // [61:59] Max. read current @ VDD min
    CSDInfo->vdd_r_curr_max		= ((rawCsd[1] & 0x07000000) >> 24); // [58:56] Max. read current @ VDD max
    CSDInfo->vdd_w_curr_min		= ((rawCsd[1] & 0x00E00000) >> 21); // [55:53] Max. write current @ VDD min
    CSDInfo->vdd_w_curr_max		= ((rawCsd[1] & 0x001C0000) >> 18); // [52:50] Max. write current @ VDD max
    CSDInfo->c_size_mult			= ((rawCsd[1] & 0x00038000) >> 15); // [49:47] Device size multiplier
    CSDInfo->erase_grp_size		= ((rawCsd[1] & 0x00007C00) >> 10); // [46:42] Erase group size
    CSDInfo->erase_grp_mult		= ((rawCsd[1] & 0x000003E0) >> 5); // [41:37] Erase group size multiplier
    CSDInfo->wp_grp_size		=  (rawCsd[1] & 0x0000001F); // [36:32] Write protect group size
    CSDInfo->wp_grp_enable		= ((rawCsd[0] & 0x80000000) >> 31); // [31:31] WP_GRP_ENABLE
    CSDInfo->default_ecc		= ((rawCsd[0] & 0x60000000) >> 29); // [30:29] Manufacturer default ECC
    CSDInfo->r2w_factor			= ((rawCsd[0] & 0x1C000000) >> 26); // [28:26] Write speed factor
    CSDInfo->write_bl_len		= ((rawCsd[0] & 0x03C00000) >> 22); // [25:22] Max. write data block length
    CSDInfo->write_bl_partial	= ((rawCsd[0] & 0x00200000) >> 21); // [21:21] Partial blocks for write allowed
    // [20:17]
    CSDInfo->content_prot_app	= ((rawCsd[0] & 0x00010000) >> 16); // [16:16] Content protection application
    CSDInfo->file_format_grp	= ((rawCsd[0] & 0x00008000) >> 13); // [15:15] File format group
    CSDInfo->copy				= ((rawCsd[0] & 0x00004000) >> 12); // [14:14] Copy flag (OTP)
    CSDInfo->perm_write_protect	= ((rawCsd[0] & 0x00002000) >> 11); // [13:13] Permanent write protection
    CSDInfo->tmp_write_protect	= ((rawCsd[0] & 0x00001000) >> 10); // [12:12] Temporary write protection
    CSDInfo->file_format		= ((rawCsd[0] & 0x00000C00) >> 10); // [11:10] File format
    CSDInfo->ecc				= ((rawCsd[0] & 0x00000300) >> 8); // [9:8] ECC code
    CSDInfo->crc				=  (rawCsd[0] & 0x000000FE); // [7:1] CRC
    // [0:0] Not used, always’1’
}


void GI::Dev::MmcSdCommon::decCid(cardCid_t *cardCid, unsigned long *rawCid)
{
    unsigned int temp[4];
    int i;

    // UNSTUFF_BITS() read in a reverse order so use a temp buffer
    for(i=0; i<4; i++)
    {
        temp[3-i] = rawCid[i];
    }

    unsigned int *resp = temp;

    cardCid->mid   = (char)mmcsd_spi_unstuffBits(resp,120,8);
    cardCid->cbx   = (char)mmcsd_spi_unstuffBits(resp,112,2);
    cardCid->oid   = (char)mmcsd_spi_unstuffBits(resp,102,8);
    cardCid->pnm   =	   mmcsd_spi_unstuffBits(resp,56,48); // This value is not correct!
    cardCid->prv   = (char)mmcsd_spi_unstuffBits(resp,48,8);
    cardCid->psn   =	   mmcsd_spi_unstuffBits(resp,16,32);
    cardCid->mdt   = (char)mmcsd_spi_unstuffBits(resp,8,8);
    cardCid->crc   = (char)mmcsd_spi_unstuffBits(resp,1,7);
}



