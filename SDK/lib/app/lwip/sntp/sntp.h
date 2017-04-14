/*
 * sntp.h
 *
 *  Created on: Dec 1, 2016
 *      Author: John Smith
 */

#ifndef LIB_APP_SNTP_SNTP_H_
#define LIB_APP_SNTP_SNTP_H_

#include <main.h>
#if (USE_SNTP == 1 && USE_LWIP == 1)
//#include "api/uart_api.h"
#include <lib/string.h>


//Custom configuration
//#define SNTP_SERVER_ADDRESS "82.197.164.46" /* google */
//#define SNTP_UPDATE_DELAY 90000 /* SNTP update every 90 seconds */
//ChibiOS RTC drivers
#define SNTP_SET_SYSTEM_TIME(sec) /*uart.printf(DebugCom, "Time id %u\n\r", sec)*/
#define SNTP_GET_SYSTEM_TIME(sec, us) \
    do{uint64_t time = rtcGetTimeUnixUsec(&RTCD1);\
       (sec) = time / 1000000;\
       (us) = time % 1000000;}while(0)

namespace GI
{
namespace App
{
class SntpClient {
public:
	SntpClient();
	~SntpClient();
	time_t time;
	GI::String *time_str;
	unsigned int time_update;
};
}
}

void sntp_init(void);
void sntp_stop(void);

#endif
#endif /* LIB_APP_SNTP_SNTP_H_ */
