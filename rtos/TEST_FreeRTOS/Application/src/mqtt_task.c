/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : mqtt_task.c
 * Project  : Minimum RTOS platform
 * Date     : 2021/11/16
 * Author   : WangYu
 *
 **********************************************************************/
#include "platform.h"
#include "lwip/apps/mqtt.h"

static void mqttInit(void);
static void mqttIncomingDataCb(void *arg, const u8_t *data, u16_t len, u8_t flags);
static void mqttIncomingPublishCb(void *arg, const char *topic, u32_t len);
static void mqttRequestCb(void *arg, err_t err);
static void mqttConnectionCb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);
static void mqttClientPubRequestCb(void *arg, err_t err);

static ip_addr_t mqttServerIP;
static mqtt_client_t* mqttClient;

void mqttTask(void *pvParameters)
{
	char topic[] = "/public/ST/sensor";
	char message[] = "Hello MQTT broker!";

	// avoid connect failure after power on
	vTaskDelay(2000);
	mqttInit();
	vTaskDelay(1000);

	// publish cyclicly (5s)
	while(1) {
		mqtt_publish(mqttClient, topic, message, sizeof(message), 1, 0, mqttClientPubRequestCb, (void*)mqttClient);
		vTaskDelay(5000);
	}
}

static const struct mqtt_connect_client_info_t mqttClientInfo = {
	"myClient1",
	NULL,	// user name
	NULL,	// password
	100,	// keep alive
	NULL,	// will_topic
	NULL,	// will_msg
	0,    	// will_qos
	0     	// will_retain
#if LWIP_ALTCP && LWIP_ALTCP_TLS
	, NULL
#endif
};

static void mqttInit(void)
{
	ip4_addr_set_u32(&mqttServerIP, ipaddr_addr("192.168.1.2"));
	mqttClient = mqtt_client_new();
	mqtt_client_connect(mqttClient, &mqttServerIP, MQTT_PORT, mqttConnectionCb, LWIP_CONST_CAST(void*, &mqttClientInfo), &mqttClientInfo);
	mqtt_set_inpub_callback(mqttClient, mqttIncomingPublishCb, mqttIncomingDataCb, LWIP_CONST_CAST(void*, &mqttClientInfo));
}

static void mqttIncomingDataCb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
	LWIP_UNUSED_ARG(len);
	LWIP_UNUSED_ARG(flags);
	const struct mqtt_connect_client_info_t* clientInfo = (const struct mqtt_connect_client_info_t*)arg;
	TRACE("MQTT client \"%s\" received message \"%s\".\r\n", clientInfo->client_id, data);
}

static void mqttIncomingPublishCb(void *arg, const char *topic, u32_t len)
{
	LWIP_UNUSED_ARG(len);
	const struct mqtt_connect_client_info_t* clientInfo = (const struct mqtt_connect_client_info_t*)arg;
	TRACE("MQTT client \"%s\" received from topic \"%s\".\r\n", clientInfo->client_id, topic);
}

static void mqttRequestCb(void *arg, err_t err)
{
	const struct mqtt_connect_client_info_t* clientInfo = (const struct mqtt_connect_client_info_t*)arg;
	TRACE("MQTT client \"%s\" request cb: err %d\r\n", clientInfo->client_id, (int)err);
}

static void mqttConnectionCb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
	const struct mqtt_connect_client_info_t* clientInfo = (const struct mqtt_connect_client_info_t*)arg;
	LWIP_UNUSED_ARG(client);
	TRACE("MQTT client \"%s\" connection cb: status %d\r\n", clientInfo->client_id, (int)status);
	if (status == MQTT_CONNECT_ACCEPTED) {
		mqtt_sub_unsub(client, "topic_qos1", 1, mqttRequestCb, LWIP_CONST_CAST(void*, clientInfo), 1);
		mqtt_sub_unsub(client, "topic_qos0", 0, mqttRequestCb, LWIP_CONST_CAST(void*, clientInfo), 1);
	}
}

static void mqttClientPubRequestCb(void *arg, err_t err)
{
	if (err != ERR_OK) {
		TRACE("STM32 MQTT client publish failed with an error (%s).\r\n", lwip_strerr(err));
	}
	else {
		TRACE("STM32 MQTT client publish once.\r\n");
	}
}
