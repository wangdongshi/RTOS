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

const char htmlHead[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
const char htmlBody[] = "<html>\
	<head>\
	<title>HTTP Server</title>\
	<style type=\"text/css\">h3{display:inline;}</style>\
	</head>\
	<body>\
	<h1>Welcome to STM32F746G-DISCO HTTP server !</h1>\
	<hr align=left width=700 size=1></hr>\
	<p>Thank you for using STM32F746G-DISCO.</p>\
	<br>\
	<p>1. <a href=\"System-info.html\">System Information</a></p>\
	<p>2. <a href=\"Error-list.html\">Error Log Viewer</a></p>\
	</body>\
	</html>";

const char htmlError[] = "\
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

// Here httpRxBuffer just need 1500 bytes, but for alignment it should be increased to 2048 bytes.
char __attribute__( ( section(".sdram" ) ) ) __attribute__( ( aligned(16) ) ) httpRxBuffer[2048];

static void httpServer(int conn);
static void makeHomePage(int conn);
static void make404Page(int conn);

void httpTask(void *pvParameters)
{
	int sock, conn, size;
	int timeout;
	struct sockaddr_in address, remoteHost;

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

	size = sizeof(remoteHost);

	while (1) {
		conn = accept(sock, (struct sockaddr *)&remoteHost, (socklen_t *)&size);
		timeout = HTTP_RECV_TIMEOUT;
		setsockopt(conn, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
		timeout = HTTP_SEND_TIMEOUT;
		setsockopt(conn, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
		httpServer(conn);
	}

	close(sock);
}

static void httpServer(int conn)
{
	int buflen = 1500;
	int ret;

	// Read in the request
	ret = read(conn, httpRxBuffer, buflen);
	if(ret <= 0) {
		// Avoid too quickly request
		vTaskDelay(300);
		close(conn);
		return;
	}

	if(strncmp((char *)httpRxBuffer, "GET /default", 12) == 0) { // home page
		makeHomePage(conn);
	}
	else { // 404 page(Error page)
		make404Page(conn);
	}

	// Avoid too quickly request
	vTaskDelay(300);

	// Close connection socket
	close(conn);
}

static void makeHomePage(int conn)
{
	// Send the HTML header
	write(conn, htmlHead, sizeof(htmlHead)-1);
	// Send our HTML page
	write(conn, htmlBody, sizeof(htmlBody)-1);
}

static void make404Page(int conn)
{
	// Send the HTML header
	write(conn, htmlHead, sizeof(htmlHead)-1);
	// Send the HTML page (404 error)
	write(conn, htmlError, sizeof(htmlError)-1);
}
