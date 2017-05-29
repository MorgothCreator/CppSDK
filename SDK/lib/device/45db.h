/*
 * 45db.h
 */

#ifndef LIB_45DB_H_
#define LIB_45DB_H_

#include "api/spi.h"
#include <include/global.h>

#define DEVICE_45DB_PAGE_SIZE           528
#ifndef DEVICE_45DB_PAGES_NR
#define DEVICE_45DB_PAGES_NR            4096
#endif

#define DEVICE_45DB_RDY                 7
#define DEVICE_45DB_COMP                6
#define DEVICE_45DB_CAPACITY            2
#define DEVICE_45DB_CAPACITY_MASK       (0x0F<<2)
#define DEVICE_45DB_PROTECT             1

/*#####################################################*/
namespace GI {
namespace Device {
class Flash45db
{
public:
    Flash45db(string spiPath);
    ~Flash45db();
    SysErr enterSleep();
    SysErr resumeSleep();
    SysErr writeBuff(unsigned char buffNr, unsigned int pageNr, unsigned char *data);
    SysErr writeFromBuffer(unsigned char buffNr, unsigned int pageNr);
    SysErr pageErase(unsigned int pageNr);
    SysErr write(unsigned char buffNr, unsigned int pageNr, unsigned char *data);
    SysErr read(unsigned int pageNr, unsigned char *data, unsigned int size);
    GI::Dev::Spi *SPI;
    SysErr err;
private:
    SysErr readStatus(unsigned char *status);
    Sizes pageSize;
    unsigned short pagesNr;
};
}
}
/*#####################################################*/
#endif /* LIB_45DB_H_ */
