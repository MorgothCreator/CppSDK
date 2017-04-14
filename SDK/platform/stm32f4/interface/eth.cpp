/*
 * lan_interface.c
 *
 *  Created on: Jul 3, 2016
 *      Author: John Smith
 */

#include "eth.h"

#if (USE_LWIP == 1)
#include "driver/stm32f4xx_hal_eth.h"
#include "driver/stm32f4xx_hal.h"
#include "interface/LwIp/src/include/lwip/netif.h"
#include "interface/LwIp/src/include/lwip/init.h"
#include "interface/LwIp/ethernetif.h"
#include "interface/LwIp/src/include/netif/etharp.h"
#include "interface/LwIp/src/include/lwip/lwip_timers.h"
#include "interface/LwIp/src/include/lwip/opt.h"
#include "interface/LwIp/src/include/lwip/dhcp.h"

struct netif gnetif;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if (USE_DHCP == 1)
unsigned long DHCPfineTimer = 0;
#endif
unsigned long IPaddress = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Notify the User about the nework interface config status
 * @param  netif: the network interface
 * @retval None
 */
void User_notification(GI::Dev::Eth *heandle, struct netif *netif)
{
	if (netif_is_up(netif))
	{
#if (USE_DHCP == 1)
		/* Update DHCP state machine */
		heandle->dhcpState = DHCP_START;
#else
#if (LWIP_DEBUG_EN == 1)
		//uint8_t iptxt[20];

		//sprintf((char*)iptxt, "%d.%d.%d.%d", IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);

		uart.printf(DebugCom, "Static IP address: %d.%d.%d.%d\n", IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);

#else
		/* Turn On LED 1 to indicate ETH and LwIP init success*/
		//BSP_LED_On(LED1);
#endif /* LWIP_DEBUG_EN */
#endif /* USE_DHCP */
	}
	else
	{
#if (USE_DHCP == 1)
		/* Update DHCP state machine */
		heandle->dhcpState = DHCP_LINK_DOWN;
#endif  /* USE_DHCP */
#if (LWIP_DEBUG_EN == 1)
		uart.puts(DebugCom, "The network cable is not connected \n", -1);
#else
		/* Turn On LED 2 to indicate ETH and LwIP init error */
		//BSP_LED_On(LED2);
#endif /* LWIP_DEBUG_EN */
	}
}

/**
 * @brief  This function notify user about link status changement.
 * @param  netif: the network interface
 * @retval None
 */
void ethernetif_notify_conn_changed(GI::Dev::Eth *heandle, struct netif *netif)
{
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;

	if (netif_is_link_up(netif))
	{
#if (USE_DHCP == 1)
#if (LWIP_DEBUG_EN == 1)
		uart.puts(DebugCom, "The network cable is now connected \n", -1);
#else
		//BSP_LED_Off(LED2);
		//BSP_LED_On(LED1);
#endif /* LWIP_DEBUG_EN */
		/* Update DHCP state machine */
		heandle->dhcpState = DHCP_START;
#else
		IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
		IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
		IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
		dhcpState = DHCP_ADDRESS_ASSIGNED;
#if (LWIP_DEBUG_EN == 1)
		//uint8_t iptxt[20];

		//sprintf((char*)iptxt, "%d.%d.%d.%d", IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);

		uart.printf(DebugCom, "Static IP address: %d.%d.%d.%d\n", IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
#else
		//BSP_LED_Off(LED2);
		//BSP_LED_On(LED1);
#endif /* LWIP_DEBUG_EN */
#endif /* USE_DHCP */

		netif_set_addr(netif, &ipaddr, &netmask, &gw);

		/* When the netif is fully configured this function must be called.*/
		netif_set_up(netif);
	}
	else
	{
#if (USE_DHCP == 1)
		/* Update DHCP state machine */
		heandle->dhcpState = DHCP_LINK_DOWN;
#endif /* USE_DHCP */

		/*  When the netif link is down this function must be called.*/
		netif_set_down(netif);

#if (LWIP_DEBUG_EN == 1)
		uart.puts(DebugCom, "The network cable is not connected \n", -1);
#else
		//BSP_LED_Off(LED1);
		//BSP_LED_On(LED2);
#endif /* LWIP_DEBUG_EN */
	}
}

#if (USE_DHCP == 1)
/**
 * @brief  DHCP_Process_Handle
 * @param  None
 * @retval None
 */
static void DHCP_Process(GI::Dev::Eth *heandle, struct netif *netif)
{
	ip_addr_t netmask;
	ip_addr_t gw;

	switch (heandle->dhcpState)
	{
	case DHCP_START :
	{
		netif->ip_addr.addr = 0;
		netif->netmask.addr = 0;
		netif->gw.addr = 0;
		IPaddress = 0;
		heandle->dhcpState = DHCP_WAIT_ADDRESS;
		dhcp_start(netif);
#if (LWIP_DEBUG_EN == 1)
		uart.puts(DebugCom, "  State: Looking for DHCP sever ...\n", -1);
#endif
	}
		break;

	case DHCP_WAIT_ADDRESS :
	{
		/* Read the new IP address */
		heandle->ipAddr = netif->ip_addr.addr;

		if (heandle->ipAddr != 0)
		{
			heandle->dhcpState = DHCP_ADDRESS_ASSIGNED;

			/* Stop DHCP */
			dhcp_stop(netif);
			heandle->ipAssigned = true;

#if (LWIP_DEBUG_EN == 1)
			uint8_t iptab[4];
			//uint8_t iptxt[20];

			iptab[0] = (uint8_t)(IPaddress >> 24);
			iptab[1] = (uint8_t)(IPaddress >> 16);
			iptab[2] = (uint8_t)(IPaddress >> 8);
			iptab[3] = (uint8_t)(IPaddress);

			//sprintf((char*)iptxt, "%d.%d.%d.%d", iptab[3], iptab[2], iptab[1], iptab[0]);

			uart.printf(DebugCom, "IP address assigned by a DHCP server: %d.%d.%d.%d\n", iptab[3], iptab[2], iptab[1], iptab[0]);
#else
			//BSP_LED_On(LED1);
#endif
		}
		else
		{
			/* DHCP timeout */
			if (netif->dhcp->tries > MAX_DHCP_TRIES)
			{
				heandle->dhcpState = DHCP_TIMEOUT;

				/* Stop DHCP */
				dhcp_stop(netif);

				/* Static address used */
				IP4_ADDR((ip4_addr_t*)&heandle->ipAddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
				IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2,
						NETMASK_ADDR3);
				IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
				netif_set_addr(netif, (const ip4_addr_t*)&heandle->ipAddr, &netmask, &gw);
				heandle->ipAssigned = true;

#if (LWIP_DEBUG_EN == 1)
				//uint8_t iptxt[20];

				//sprintf((char*)iptxt, "%d.%d.%d.%d", IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
				uart.puts(DebugCom, "DHCP timeout !!\n", -1);
				uart.printf(DebugCom, "Static IP address  : %d.%d.%d.%d\n", IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
#else
				//BSP_LED_On(LED1);
#endif
			}
		}
	}
		break;
	default:
		break;
	}
}

/**
 * @brief  DHCP periodic check
 * @param  localtime the current LocalTime value
 * @retval None
 */
static void periodicHandle(GI::Dev::Eth *heandle, struct netif *netif)
{
	/* Fine DHCP periodic process every 500ms */
	if (HAL_GetTick() - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
	{
		DHCPfineTimer = HAL_GetTick();
		if ((heandle->dhcpState != DHCP_ADDRESS_ASSIGNED )
				&& (heandle->dhcpState != DHCP_TIMEOUT )
				&& (heandle->dhcpState != DHCP_LINK_DOWN ))
		{
			/* process DHCP state machine */
			DHCP_Process(heandle, netif);
		}
	}
}
#endif


GI::Dev::Eth::Eth(unsigned char instance, unsigned long ip) :
		ipAssigned(false),
		ipAddr(0)
{
    /* Enable MFX interrupt for ETH MII pin */
    //BSP_IO_ConfigPin(MII_INT_PIN, IO_MODE_IT_FALLING_EDGE);
    /* do lwip library init only once */
    lwip_init();
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;

    IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
    IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

    /* Add the network interface */
    netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

    /*  Registers the default network interface */
    netif_set_default(&gnetif);

    if (netif_is_link_up(&gnetif))
    {
      /* When the netif is fully configured this function must be called */
      netif_set_up(&gnetif);
    }
    else
    {
      /* When the netif link is down this function must be called */
      netif_set_down(&gnetif);
    }

    /* Set the link callback function, this function is called on change of link status*/
    netif_set_link_callback(&gnetif, ethernetif_update_config);

    /* Notify user about the network interface config */
    User_notification(this, &gnetif);
}

unsigned char GI::Dev::Eth::idle(unsigned char instance)
{
    /* Read a received packet from the Ethernet buffers and send it
       to the lwIP for handling */
    ethernetif_input(&gnetif);
    /* Handle timeouts */
    sys_check_timeouts();
    periodicHandle(this, &gnetif);
    return dhcpState;
}

void GI::Dev::Eth::getIp(GI::String *string)
{
	char buff[24];
	sprintf(buff, "%u.%u.%u.%u",
			(unsigned int)ipAddr& 0xFF,
			(unsigned int)(ipAddr >> 8)& 0xFF,
			(unsigned int)(ipAddr >> 16)& 0xFF,
			(unsigned int)(ipAddr >> 24)& 0xFF);
	string->set(buff);
}
#endif
