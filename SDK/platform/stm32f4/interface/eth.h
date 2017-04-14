/*
 * lan_interface.h
 *
 *  Created on: Jul 3, 2016
 *      Author: John Smith
 */

#ifndef AM33_INTERFACE_LAN_INTERFACE_H_
#define AM33_INTERFACE_LAN_INTERFACE_H_

#include <api/eth.h>
#include <interface/uart.h>
#include <lib/string.h>
#include <board.h>
#if (USE_LWIP == 1)

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
#endif
#endif /* AM33_INTERFACE_LAN_INTERFACE_H_ */
