/*
 *  api/i2c.h
 */

#ifndef TWI_DEF_H_
#define TWI_DEF_H_
#include <stdlib.h>
#include <api/gpio.h>
#include <api/i2c.h>
#include <include/global.h>
/*#####################################################*/
#define twi_timeout	500000
/*#####################################################*/
typedef struct
{
	char name[12];
	unsigned int scl;
	unsigned int sda;
	unsigned long speed;
} CfgI2c;
/*#####################################################*/
/*#####################################################*/
namespace GI
{
namespace Dev
{
class I2c
{
public:
	I2c(ioSettings *cfg);
	~I2c();
	SysErr writeRead(unsigned char addr, unsigned char *buffSend,
			unsigned int lenSend, unsigned char *buffReceive,
			unsigned int lenReceive);
	int readBytes(unsigned char addr, unsigned char *buff, unsigned int len);
	int writeBytes(unsigned char addr, unsigned char *buff, unsigned int len);

	unsigned char unitNr;
	volatile unsigned char flag;
	volatile unsigned char error_flag;
	volatile unsigned char result;
	volatile unsigned char status;
	unsigned int tCount;
	unsigned int rCount;
	unsigned int numOfBytes;
	unsigned int bytesToWrite;
	unsigned int bytesToRead;
	unsigned long BusyTimeOut;
	bool noSendWriteOnRead;
	ioSettings *cfg;
	void *udata;
	SysErr err;
private:
	SysErr WR(unsigned char addr, unsigned char *buff_send,
			unsigned int TransmitBytes, unsigned char *buff_receive,
			unsigned int ReceiveBytes);
};
}
}
/*#####################################################*/
#endif /* TWI_DEF_H_ */
/*#####################################################*/
