/*
 * 24c.h
 */

#ifndef LIB_DEVICE_24C_H_
#define LIB_DEVICE_24C_H_

#include "api/i2c.h"
#include <include/global.h>

/*#####################################################*/
namespace GI {
namespace Device {
class Eep24c
{
public:
    Eep24c(string i2cPath, unsigned char icNr, Sizes devSize);
    ~Eep24c();
    SysErr write(unsigned long addr, unsigned char *buff, unsigned long len);
    SysErr read(unsigned long addr, unsigned char *buff, unsigned long len);
    GI::Dev::I2c* I2C;
    SysErr err;
private:
    unsigned char IcNr;
    Sizes devSize;
};
}
}
/*#####################################################*/
#endif /* LIB_DEVICE_24C_H_ */
