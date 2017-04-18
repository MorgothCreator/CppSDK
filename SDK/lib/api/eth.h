/*
 *  api/eth.h
 */
/*#####################################################*/
#ifndef LWIP_DEF_H_
#define LWIP_DEF_H_
/*#####################################################*/
#include <stdbool.h>
#include <interface/eth.h>
/*#####################################################*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* DHCP process states */

typedef struct lan_s
{
	void (*init)(unsigned char instance, unsigned long static_ip);
	unsigned char (*idle)(unsigned char instance);
} lan_t;

/*#####################################################*/
#endif /* ADC_DEF_H_ */
/*#####################################################*/
