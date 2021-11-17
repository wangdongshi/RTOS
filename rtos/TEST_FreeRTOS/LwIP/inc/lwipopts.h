/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : lwipopts.h
 * Project  : Minimum RTOS platform
 * Date     : 2021/11/5
 * Author   : WangYu
 *
 **********************************************************************/

#ifndef __LWIPOPTS__H__
#define __LWIPOPTS__H__

#ifdef __cplusplus
extern "C" {
#endif

#define WITH_RTOS					1
#define CHECKSUM_BY_HARDWARE		1

// LwIP Stack Parameters (modified compared to initialization value in opt.h)
#define MEM_ALIGNMENT				4
//#define MEMP_MEM_MALLOC			1
#define MQTT_DEBUG					LWIP_DBG_ON
#define LWIP_ETHERNET				1
#define LWIP_DNS_SECURE				7
#define TCP_SND_QUEUELEN			9
#define TCP_SNDLOWAT				1071	// LWIP_MIN(LWIP_MAX(((TCP_SND_BUF)/2), (2 * TCP_MSS) + 1), (TCP_SND_BUF) - 1)
#define TCP_SNDQUEUELOWAT			5		// LWIP_MAX(TCP_SND_QUEUELEN)/2, 5)
#define TCP_WND_UPDATE_THRESHOLD	536		// LWIP_MIN(TCP_WND/4, TCP_MSS*4)
#define LWIP_NETIF_LINK_CALLBACK	1
#define TCPIP_THREAD_STACKSIZE		1024
#define TCPIP_THREAD_PRIO			0
#define TCPIP_MBOX_SIZE				6
#define SLIPIF_THREAD_STACKSIZE		1024
#define SLIPIF_THREAD_PRIO			3
#define DEFAULT_THREAD_STACKSIZE	1024
#define DEFAULT_THREAD_PRIO			3
#define DEFAULT_UDP_RECVMBOX_SIZE	6
#define DEFAULT_TCP_RECVMBOX_SIZE	6
#define DEFAULT_ACCEPTMBOX_SIZE		6
#define RECV_BUFSIZE_DEFAULT		2000000000
#define LWIP_STATS					0
#define CHECKSUM_GEN_IP				0
#define CHECKSUM_GEN_UDP			0
#define CHECKSUM_GEN_TCP			0
#define CHECKSUM_GEN_ICMP			0
#define CHECKSUM_GEN_ICMP6			0
#define CHECKSUM_CHECK_IP			0
#define CHECKSUM_CHECK_UDP			0
#define CHECKSUM_CHECK_TCP			0
#define CHECKSUM_CHECK_ICMP			0
#define CHECKSUM_CHECK_ICMP6		0

#ifdef __cplusplus
}
#endif
#endif // __LWIPOPTS__H__
