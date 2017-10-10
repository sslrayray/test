/* 
yulei, 2017.9.22
description: be a client, and use TCP to test echo function on server
*/
#include "stdafx.h"
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <string.h>

#pragma comment(lib,"ws2_32.lib")

#define SERVO_PORT 10023
#define MAX_BUFFER_LENGTH 512
#define VMWARE_IP "192.168.193.132"
#define GOOGL_IP "192.168.1.2"

char g_send_buffer[MAX_BUFFER_LENGTH];

/* Class Point */
class Robot_point {
#define JOINT_NUM 6
public:
	double joint_degree[JOINT_NUM];

	Robot_point(double* joint_array) 
	{
		for (int i = 0; i < JOINT_NUM; i++)
			joint_degree[i] = joint_array[i];
	}

	Robot_point(double joint0, double joint1, double joint2, double joint3, double joint4, double joint5)
	{
		joint_degree[0] = joint0;
		joint_degree[1] = joint1;
		joint_degree[2] = joint2;
		joint_degree[3] = joint3;
		joint_degree[4] = joint4;
		joint_degree[5] = joint5;

	}
};

static int client_process()
{
	WSADATA wsa;
	SOCKET sock;
	SOCKADDR_IN sockaddr;
	/*print start information*/
	printf("==================\n");
	printf("googl client test program\n");
	printf("==================\n");

	/*initialize windows network environment*/
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("wsa startup failed\n");
		return -1;
	}

	/*create socket*/
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	/*designate server information*/
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(SERVO_PORT);
	sockaddr.sin_addr.S_un.S_addr = inet_addr(GOOGL_IP);

	/*connect to server*/
	if (connect(sock, (SOCKADDR*)&sockaddr, sizeof(sockaddr)) < 0) {
		printf("ERR: connect to server error\n");
		goto CLIENT_EXIT;
	}

	sprintf(g_send_buffer, "11");

	

	Sleep(2000);
	
	/*send hello message*/
	if (send(sock, g_send_buffer, 2, 0) < 0) {
		printf("ERR: send packet error");
		goto CLIENT_EXIT;
	}
	else {
		printf("sent string successfully\n");
	}
	printf("after sleep\n");
CLIENT_EXIT:
	WSACleanup();
	closesocket(sock);
	return 0;
}

static int server_process()
{
	WSADATA wsa;
	SOCKET listen_sock, connect_sock;
	SOCKADDR_IN sockaddr, client_addr;
	int addr_len = 0;
	/*print start information*/
	printf("==================\n");
	printf("googl server test program\n");
	printf("==================\n");

	/*initialize windows network environment*/
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("wsa startup failed\n");
		return -1;
	}

	/*create socket*/
	listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	/*designate server information*/
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(SERVO_PORT);
	sockaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	/*bind socket with sockaddr*/
	if (bind(listen_sock, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
		printf("ERR: socket bind error \n");
		goto SERVER_EXIT;
	}

	printf("before listening\n");

	/*listen connect request from client*/
	if (listen(listen_sock, 1) < 0) {
		printf("ERR: listen error\n");
		goto SERVER_EXIT;
	}

	printf("after listening\n");

	addr_len = sizeof(client_addr);
	if ((connect_sock = accept(listen_sock, (struct sockaddr*)&client_addr, &addr_len)) <= 0) {
		printf("accept error\n");
	}

	printf("after accept\n");
	/*send hello message*/
	if (recv(connect_sock, g_send_buffer, 512, 0) < 0) {
		printf("ERR: send packet error");
		goto SERVER_EXIT;
	}
	else {
		printf("recv string '%s' successfully\n", g_send_buffer);
	}

	/*send hello message*/
	if (send(connect_sock, g_send_buffer, 3, 0) < 0) {
		printf("ERR: send packet error");
		goto SERVER_EXIT;
	}
	else {
		printf("sent string successfully\n");
	}

	goto SERVER_EXIT;

	for (int i = 0; i < 30; i++) {
		/*send hello message*/
		if (recv(connect_sock, g_send_buffer, 512, 0) < 0) {
			printf("ERR: send packet error");
			goto SERVER_EXIT;
		}
		else {
			printf("recv string %s successfully\n", g_send_buffer);
		}
	}

SERVER_EXIT:
	WSACleanup();
	closesocket(listen_sock);
	closesocket(connect_sock);
}

int main()
{
	//client_process();
	server_process();
	for (int i = 0; i < 10; i++) {
		//moveto();
	}
	getchar();
    return 0;
}

