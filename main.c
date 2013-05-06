#include <stdio.h>
#include <string.h>
#include <windows.h>

#pragma comment(lib, "wsock32.lib")

#define PORT	8324

void myrecv(int connected_socket)
{
	char in_buf[4096];
	int ret;

	ret = recv(connected_socket, in_buf, sizeof(in_buf), 0);
	if (ret < 0)
	{
		printf("*** ERROR - recv() failed \n");
		exit(-1);
	}
	printf("Received from client: %s \n", in_buf);
}

void mysend(int connected_socket)
{
	int ret;

	ret = send(connected_socket, buf.psz, buf.size, 0);
	if (ret < 0)
	{
		printf("*** ERROR - send() failed \n");
		exit(-1);
	}
}

void main(void)
{
	WSADATA					wsaData;
	int						listening_socket;
	struct sockaddr_in	server_addr;
	int						connected_socket;
	struct sockaddr_in	client_addr;
	struct in_addr			client_ip_addr;
	int						addr_len;
	char						out_buf[4096];
	int						ret;
	int						con;
	WORD wVersionRequested = MAKEWORD(1,1);

	WSAStartup(wVersionRequested, &wsaData);

	listening_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listening_socket < 0)
	{
		printf("E: socket() failed \n");
		exit(-1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	ret = bind(listening_socket, (struct sockaddr *)&server_addr,
		sizeof(server_addr));

	if (ret < 0)
	{
		printf("E: bind() failed \n");
		exit(-1);
	}

	listen(listening_socket, 1);

	printf("accept()... \n");
	addr_len = sizeof(client_addr);
	connected_socket = accept(listening_socket, (struct sockaddr *)&client_addr, &addr_len);
	if (connected_socket < 0)
	{
		printf("E: accept() failed \n");
		exit(-1);
	}
	memcpy(&client_ip_addr, &client_addr.sin_addr.s_addr, 4);
	printf("Accepted: IP = %s  port = %d \n", inet_ntoa(client_ip_addr), ntohs(client_addr.sin_port));

	do
	{
		scanf("%d", &con);

		if (con==1)
			mysend(connected_socket);
		else if (con==2)
			myrecv(connected_socket);

	} while ( con != 0 );

	ret = closesocket(listening_socket);
	if (ret < 0)
	{
		printf("E: closesocket() failed \n");
		exit(-1);
	}
	ret = closesocket(connected_socket);
	if (ret < 0)
	{
		printf("E: closesocket() failed \n");
		exit(-1);
	}

	WSACleanup();
}
