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

static void mqtt_init(void);
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags);
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len);
static void mqtt_request_cb(void *arg, err_t err);
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);

void mqttTask(void *pvParameters)
{
	// avoid connect failure after power on
	vTaskDelay(2000);

#if LWIP_TCP
	mqtt_init();
#endif
#if 0
	char message_test[] = "Hello mqtt server";
	for (int i = 0; i < 10; i++) {
		mqtt_incoming_publish_cb(s__mqtt_client_instance, "/public/TEST/AidenHinGwenWong_pub",\
						 message_test, sizeof(message_test), 1, 0);
		vTaskDelay(1000);
	}
#endif
	while(1) {
		vTaskDelay(1000);
	}
}

#if LWIP_TCP

static ip_addr_t mqtt_server_ip;
static mqtt_client_t* mqtt_client;

static const struct mqtt_connect_client_info_t mqtt_client_info = {
	"myMQTT",
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

static void mqtt_init(void)
{
	ip4_addr_set_u32(&mqtt_server_ip, ipaddr_addr("192.168.1.2"));
	mqtt_client = mqtt_client_new();
	mqtt_set_inpub_callback(mqtt_client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, LWIP_CONST_CAST(void*, &mqtt_client_info));
	mqtt_client_connect(mqtt_client, &mqtt_server_ip, MQTT_PORT, mqtt_connection_cb, LWIP_CONST_CAST(void*, &mqtt_client_info), &mqtt_client_info);
}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
	const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;
	LWIP_UNUSED_ARG(data);
	TRACE("MQTT client \"%s\" data cb: len %d, flags %d\r\n", client_info->client_id, (int)len, (int)flags);
}

static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
	const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;
	TRACE("MQTT client \"%s\" publish cb: topic %s, len %d\r\n", client_info->client_id, topic, (int)tot_len);
}

static void mqtt_request_cb(void *arg, err_t err)
{
	const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;
	TRACE("MQTT client \"%s\" request cb: err %d\r\n", client_info->client_id, (int)err);
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
	const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;
	LWIP_UNUSED_ARG(client);
	TRACE("MQTT client \"%s\" connection cb: status %d\r\n", client_info->client_id, (int)status);
	if (status == MQTT_CONNECT_ACCEPTED) {
		mqtt_sub_unsub(client, "topic_qos1", 1, mqtt_request_cb, LWIP_CONST_CAST(void*, client_info), 1);
		mqtt_sub_unsub(client, "topic_qos0", 0, mqtt_request_cb, LWIP_CONST_CAST(void*, client_info), 1);
	}
}
#endif // LWIP_TCP
