#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f7xx_hal_eth.h"

extern ETH_HandleTypeDef heth;
extern void LWIP_Init(void);

#ifdef __cplusplus
}
#endif

#endif // __ETHERNETIF_H__
