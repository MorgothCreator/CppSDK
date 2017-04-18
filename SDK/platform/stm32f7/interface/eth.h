/*
 * interface/eth.h
 */

#ifndef AM33_INTERFACE_LAN_INTERFACE_H_
#define AM33_INTERFACE_LAN_INTERFACE_H_

#include "driver/stm32f7xx_hal_eth.h"
#include "driver/stm32f7xx_hal.h"
#include "interface/LwIp/src/include/lwip/netif.h"
#include "interface/LwIp/src/include/lwip/init.h"
#include "interface/LwIp/ethernetif.h"
#include "interface/LwIp/src/include/netif/etharp.h"
#include "interface/LwIp/src/include/lwip/lwip_timers.h"
#include "interface/LwIp/src/include/lwip/opt.h"
#include "interface/LwIp/src/include/lwip/dhcp.h"
#include <interface/uart.h>
#include <lib/string.h>
#include <board.h>


typedef enum {
	DHCP_START = 1,
	DHCP_WAIT_ADDRESS,
	DHCP_ADDRESS_ASSIGNED,
	DHCP_TIMEOUT,
	DHCP_LINK_DOWN,
}DHCP_STATE;


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
void periodicHandle(GI::Dev::Eth *heandle, struct netif *netif);
#endif /* AM33_INTERFACE_LAN_INTERFACE_H_ */
