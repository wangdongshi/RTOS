/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : http_task.c
 * Project  : Minimum RTOS platform
 * Date     : 2021/10/16
 * Author   : WangYu
 *
 **********************************************************************/
#include <math.h>
#include "platform.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "ethernetif.h"
#include "http_task.h"

#define HTTP_RECV_TIMEOUT	30000   // 30s
#define HTTP_SEND_TIMEOUT	60000   // 60s

const char html_head[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
const char html_body_home[] = "<html>\
	<head>\
	<title>TW230 HTTP Server</title>\
	<style type=\"text/css\">h3{display:inline;}</style>\
	</head>\
	<body>\
	<h1>Welcome to TW230 HTTP server !</h1>\
	<hr align=left width=550 size=1></hr>\
	<p>Thank you for purchasing and using TW230 Multi-function Power Transducer.</p>\
	<br>\
	<p>1. <a href=\"tw230-system-info.html\">TW230 System Information</a></p>\
	<p>2. <a href=\"tw230-wavelog.html\">TW230 Wave Log Viewer</a></p>\
	<p>3. <a href=\"tw230-error-list.html\">TW230 Error Log Viewer</a></p>\
	<p>4. <a href=\"tw230-cal-param.html\">TW230 Calibration Parameter</a></p>\
	</body>\
	</html>";

const char html_body_error[] = "\
	<html>\
	<head>\
	<title>Error Page</title>\
	</head>\
	<body>\
	<h1>404 - Page not found</h1>\
	<p></p>\
	<p>Sorry, the page you are requesting was not found on this server.</p>\
	</body>\
	</html>";

// Here in_buffer just need 1500 bytes, but for alignment it should be increased to 2048 bytes.
char __attribute__( ( section(".sdram" ) ) ) __attribute__( ( aligned(16) ) ) in_buffer[2048];

static void http_server(int conn);
static void MakeHomePage(int conn);
static void Make404Page(int conn);

void httpTask(void *pvParameters)
{
	int sock, conn, size;
	int timeout;
	struct sockaddr_in address, remotehost;

	// avoid connect failure after power on
	vTaskDelay(2000);

	// create a TCP socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		TRACE("Can not create socket.\r\n");
		return;
	}

	// bind to port 80 at any interface
	address.sin_family = AF_INET;
	address.sin_port = htons(80);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0) {
		TRACE("Can not bind socket.\r\n");
		close(sock);
		return;
	}

	// listen for incoming connections (TCP listen backlog = 1)
	listen(sock, 1);

	size = sizeof(remotehost);

	while (1) {
		conn = accept(sock, (struct sockaddr *)&remotehost, (socklen_t *)&size);
		timeout = HTTP_RECV_TIMEOUT;
		setsockopt(conn, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
		timeout = HTTP_SEND_TIMEOUT;
		setsockopt(conn, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
		http_server(conn);
	}

	close(sock);
}

static void http_server(int conn)
{
	int buflen = 1500;
	int ret;

	// Read in the request
	ret = read(conn, in_buffer, buflen);
	if(ret <= 0) {
		// Avoid too quickly request
		vTaskDelay(300);
		close(conn);
		return;
	}

	if(strncmp((char *)in_buffer, "GET /default", 12) == 0) { // home page
		MakeHomePage(conn);
	}
	else { // 404 page(Error page)
		Make404Page(conn);
	}

	// Avoid too quickly request
	vTaskDelay(300);

	// Close connection socket
	close(conn);
}

static void MakeHomePage(int conn)
{
	// Send the HTML header
	write(conn, html_head, sizeof(html_head)-1);
	// Send our HTML page
	write(conn, html_body_home, sizeof(html_body_home)-1);
}

static void Make404Page(int conn)
{
	// Send the HTML header
	write(conn, html_head, sizeof(html_head)-1);
	// Send the HTML page (404 error)
	write(conn, html_body_error, sizeof(html_body_error)-1);
}
