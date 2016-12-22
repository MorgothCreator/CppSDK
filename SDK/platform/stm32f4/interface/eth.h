/*
 * lan_interface.h
 *
 *  Created on: Jul 3, 2016
 *      Author: John Smith
 */

#ifndef AM33_INTERFACE_LAN_INTERFACE_H_
#define AM33_INTERFACE_LAN_INTERFACE_H_

#include <api/eth_def.h>
#include "driver/stm32f4xx_hal_eth.h"
#include "driver/stm32f4xx_hal.h"
#include "interface/LwIp/src/include/lwip/netif.h"
#include "interface/LwIp/src/include/lwip/init.h"
#include "interface/LwIp/ethernetif.h"
#include "interface/LwIp/src/include/netif/etharp.h"
#include "interface/LwIp/src/include/lwip/lwip_timers.h"
#include "interface/LwIp/src/include/lwip/opt.h"
#include "interface/LwIp/src/include/lwip/dhcp.h"
#include <interface/uart.h>
#include <lib/string.h>
#include <board/board.h>

#define LEN_IP_ADDR                        (4u)
extern struct netif gnetif;
namespace GI
{
namespace Dev
{
class Eth {
public:
	Eth(unsigned char instance, unsigned long ip);
	~Eth();
	unsigned char idle(unsigned char instance);
	void getIp(GI::String *string);
	DHCP_STATE dhcpState;
	bool ipAssigned;
	unsigned long ipAddr;
};
}
}

#endif /* AM33_INTERFACE_LAN_INTERFACE_H_ */
