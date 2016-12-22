#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include "interface/LwIp/src/include/lwip/err.h"
#include "interface/LwIp/src/include/lwip/netif.h"

//#define DP83848_PHY_ADDRESS             0x01U
#define LAN8742A_PHY_ADDRESS            0x00U
/* Exported types ------------------------------------------------------------*/
void ethernetif_update_config(struct netif *netif);
err_t ethernetif_init(struct netif *netif);
void ethernetif_input(struct netif *netif);
void ethernetif_notify_conn_changed(struct netif *netif);
#endif
