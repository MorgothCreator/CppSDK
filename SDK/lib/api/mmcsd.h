/*
 *  api/mmcsd_def.h
 */

#ifndef MMCSD_API_H_
#define MMCSD_API_H_
/*#####################################################*/

#include <include/global.h>

#define ctrlInfo uSdCardInfo

typedef enum
{
	IsNoCard = 0, IsSd, IsSdhc,
} mmcsd_type_e;

namespace GI
{
namespace Dev
{
class MmcSd
{
public:
    //MmcSd(unsigned int unit_nr, char *cardDetectPinPath, char *cardStatusLedPinPath);
    //~MmcSd();

    void (*idle_Ptr)(void *handler);
    unsigned int (*read_Ptr)(void *handler, void *ptr, unsigned long block,
            unsigned int nblks);
    unsigned int (*write_Ptr)(void *handler, void *ptr, unsigned long block,
            unsigned int nblks);
    void (*ioctl_Ptr)(void *handler, unsigned int command,
            unsigned int *buffer);

    SysErr idle();
    SysErr read(void *ptr, unsigned long block,
            unsigned int nblks);
    SysErr write(void *ptr, unsigned long block,
            unsigned int nblks);
    SysErr ioctl(unsigned int command,
            unsigned int *buffer);
    void *driverHandler_Ptr;
private:
    //GI::Dev::Gpio *cardDetectGpio;
    //GI::Dev::Gpio *cardStatusLedPinGpio;
    //bool cardDetected;
};
}
}

/*#####################################################*/
#endif /* MMCSD_API_H_ */
/*#####################################################*/
