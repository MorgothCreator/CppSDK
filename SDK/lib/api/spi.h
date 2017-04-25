/*
 *  api/spi.h
 */

#ifndef MCSPI_DEF_H_
#define MCSPI_DEF_H_
/*#####################################################*/
#include <stdbool.h>
#include <api/gpio.h>
#include <interface/spi.h>
/*#####################################################*/
typedef struct
{
	char name[12];
	unsigned int sck;
	unsigned int moSi;
	unsigned int miSo;
	unsigned int cs;
	unsigned long speed;
	enum spiMode{
		spiMode0,
		spiMode1,
		spiMode2,
		spiMode3,
	}spiMode;
	bool lsbFirst;
} CfgSpi;
/*#####################################################*/
namespace GI
{
namespace Dev
{
class Spi
{
public:
	Spi(ioSettings *cfg);
	~Spi();
	int ioctl(charDev *fp, int command, void *param);
	int assert();
	int deassert();
	SysErr writeRead(unsigned char *buffWrite, unsigned int lenWrite,
			unsigned char *buffRead, unsigned int lenRead);
	int readBytes(unsigned char *buff, unsigned int len);
	int writeBytes(unsigned char *buff, unsigned int len);
	SysErr writeReadByte(unsigned char *byte);
	SysErr setSpeed(unsigned long baud);
	SysErr err;
	ioSettings *cfg;
    bool disableCsHandle;
private:
	unsigned char oldCsSelect;
	unsigned char unitNr;
	unsigned char channel;
	unsigned int speed;
	volatile bool useInterrupt;
	volatile unsigned char flag;
	volatile unsigned char Priority;
	volatile unsigned char Mode;
	volatile unsigned int BytesCnt;
	bool cPol;bool cPha;
	bool lsbFirst;
	unsigned char wordSize;
	bool slave;
	void *userData;

};
}
}
/*#####################################################*/
#endif /* MCSPI_DEF_H_ */
/*#####################################################*/
