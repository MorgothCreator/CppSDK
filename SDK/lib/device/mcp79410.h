/*
 * mcp79410.h
 */

#ifndef LIB_DEVICE_MCP79410_H_
#define LIB_DEVICE_MCP79410_H_

#include <include/global.h>
#include <lib/string.h>
#include "api/i2c.h"

//#####################################################
#define MCP73871_Rtc_DeviceAddr                 (0x6F)
#define MCP73871_Sram_DeviceAddr                (0x6F)
#define MCP73871_Eep_DeviceAddr                 (0x57)
//-----------------------------------------------------
#define MCP73871_Address_Length                 (1)
//-----------------------------------------------------
#define MCP73871_Sram_Page_Size                 (0x00)
#define MCP73871_Sram_Start                     (0x20)
#define MCP73871_Sram_Size                      (0x40)
#define MCP73871_Sram_End                       (0x60)
//-----------------------------------------------------
#define MCP73871_Eep_Page_Size                  (0x08)
#define MCP73871_Eep_Start                      (0x00)
#define MCP73871_Eep_Size                       (0x80)
#define MCP73871_Eep_End                        (0x80)
//#####################################################
typedef enum MCP73871_Enum
{
    //RTCC MEMORY MAP
    MCP73871_RTC_SecReg = 0x00,
    MCP73871_RTC_MinReg,
    MCP73871_RTC_HourReg,
    MCP73871_RTC_DayReg,
    MCP73871_RTC_DateReg,
    MCP73871_RTC_MonthReg,
    MCP73871_RTC_YearReg,
    //RTCC CONTROLL
    MCP73871_RTC_ControlReg,
    MCP73871_RTC_CalibrationReg,
    MCP73871_RTC_UnlockIdReg,
    //RTCC ALARM0
    MCP73871_ALARM0_SecReg,
    MCP73871_ALARM0_MinReg,
    MCP73871_ALARM0_HourReg,
    MCP73871_ALARM0_DayReg,
    MCP73871_ALARM0_DateReg,
    MCP73871_ALARM0_MonthReg,
    MCP73871_ALARM0_Reserved_0x10,
    //RTCC ALARM1
    MCP73871_ALARM1_SecReg,
    MCP73871_ALARM1_MinReg,
    MCP73871_ALARM1_HourReg,
    MCP73871_ALARM1_DayReg,
    MCP73871_ALARM1_DateReg,
    MCP73871_ALARM1_MonthReg,
}MCP73871_t;
//#####################################################
//RTCC MEMORY MAP Bit description
//-----------------------------------------------------
//SECONDS
typedef enum MCP73871_Seconds_Enum
{
    MCP73871_Seconds_gp = 0,
    MCP73871_10Seconds_gp = 4,
    MCP73871_St_bp = 7,
    MCP73871_Seconds_gm = 0x0F<<0,
    MCP73871_10Seconds_gm = 0x07<<4,
    MCP73871_St_bm = 0x01<<7,
}MCP73871_Seconds_t;
//-----------------------------------------------------
//MINUTES
typedef enum MCP73871_Minutes_Enum
{
    MCP73871_Minutes_gp = 0,
    MCP73871_10Minutes_gp = 4,
    MCP73871_Minutes_gm = 0x0F<<0,
    MCP73871_10Minutes_gm = 0x07<<4,
}MCP73871_Minutes_t;
//-----------------------------------------------------
//HOURS
typedef enum MCP73871_Hours_Enum
{
    MCP73871_Hours_gp = 0,
    MCP73871_10Hours_gp = 4,
    MCP73871_12_24_bp = 6,
    MCP73871_Hours_gm = 0x0F<<0,
    MCP73871_10Hours_gm = 0x03<<4,
    MCP73871_12_24_bm = 0x01<<6,
}MCP73871_Hours_t;
//-----------------------------------------------------
//DAYS
typedef enum MCP73871_Days_Enum
{
    MCP73871_Day_gp = 0,
    MCP73871_VBatEn_bp = 3,
    MCP73871_VBat_bp = 4,
    MCP73871_OscOn_bp = 5,
    MCP73871_Day_gm = 0x07<<0,
    MCP73871_VBatEn_bm = 0x01<<3,
    MCP73871_VBat_bm = 0x01<<4,
    MCP73871_OscOn_bm = 0x01<<5,
}MCP73871_Days_t;
//-----------------------------------------------------
//DATES
typedef enum MCP73871_Dates_Enum
{
    MCP73871_Date_gp = 0,
    MCP73871_10Date_gp  = 4,
    MCP73871_Date_gm = 0x0F<<0,
    MCP73871_10Date_gm  = 0x03<<4,
}MCP73871_Dates_t;
//-----------------------------------------------------
//MONTHS
typedef enum MCP73871_Months_Enum
{
    MCP73871_Month_gp = 0,
    MCP73871_10Month_bp = 4,
    MCP73871_LP_bp  = 5,
    MCP73871_Month_gm = 0x0F<<0,
    MCP73871_10Month_bm = 0x01<<4,
    MCP73871_LP_bm  = 0x01<<5,
}MCP73871_Months_t;
//-----------------------------------------------------
//YEARS
typedef enum MCP73871_Years_Enum
{
    MCP73871_Year_gp = 0,
    MCP73871_10Year_gp  = 4,
    MCP73871_Year_gm = 0x0F<<0,
    MCP73871_10Year_gm  = 0x0F<<4,
}MCP73871_Years_t;
//-----------------------------------------------------
//CONTROL
typedef enum MCP73871_Control_Enum
{
    MCP73871_Rs0_bp = 0,
    MCP73871_Rs1_bp = 1,
    MCP73871_Rs2_bp = 2,
    MCP73871_ExtOsc_bp = 3,
    MCP73871_Alm0_bp = 4,
    MCP73871_Alm1_bp = 5,
    MCP73871_Sqwe_bp = 6,
    MCP73871_Out_bp = 7,
    MCP73871_Rs0_bm = 0x01<<0,
    MCP73871_Rs1_bm = 0x01<<1,
    MCP73871_Rs2_bm = 0x01<<2,
    MCP73871_ExtOsc_bm = 0x01<<3,
    MCP73871_Alm0_bm = 0x01<<4,
    MCP73871_Alm1_bm = 0x01<<5,
    MCP73871_Sqwe_bm = 0x01<<6,
    MCP73871_Out_bm = 0x01<<7,
}MCP73871_Control_t;
/*#####################################################*/
namespace GI {
namespace Device {
class Mcp79410
{
public:
    Mcp79410(char *i2cPath);
    ~Mcp79410();

    SysErr ramRead(unsigned char addr, unsigned char *data, unsigned char len);
    SysErr ramWrite(unsigned char addr, unsigned char *data, unsigned char len);
    SysErr eepRead(unsigned char addr, unsigned char *data, unsigned char len);
    SysErr eepWrite(unsigned char addr, unsigned char *data, unsigned char len);
    SysErr checkEepReady();
    SysErr writeRtc(unsigned char reg, unsigned char time);
    SysErr readRtc(unsigned char reg, unsigned char *time);
    GI::String *readTime(string format);
    GI::String *timeStr;
    GI::Dev::I2c* I2C;
private:
    SysErr regRead(unsigned char reg, unsigned char *data, unsigned char len);
    SysErr regWrite(unsigned char reg, unsigned char *data, unsigned char len);
    unsigned char byteToBcd(unsigned char byte);
    unsigned char bcdToByte(unsigned char bcd);
};
}
}
/*#####################################################*/



#endif /* LIB_DEVICE_MCP79410_H_ */
