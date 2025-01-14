#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <easyx.h>
#pragma comment(lib, "ws2_32.lib")
struct pack_header
{
	int type;//0:text, 1:image
	int size;//data size
};

SOCKET create_listen_socket(unsigned short port=8080)
{
	// create a socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock)
	{
		printf("create socket failed !!! \n");
		return -1;
	}
	//bind ip and port
	struct sockaddr_in local = { 0 };
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr("0.0.0.0");
	if (-1 == bind(sock, (struct sockaddr*)&local, sizeof(local)))
	{
		printf("bind socket failed !!!\n");
		return -1;
	}
	//listen 
	if (-1 == listen(sock, 10))
	{
		printf("listen socket failed !!!\n");
		return -1;
	}

	return sock;
}
int main()
{
	initgraph(1280, 720);
	// permit access web
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//create a listen socket
	SOCKET listen_socket = create_listen_socket();

	struct sockaddr_in client_addr;
	int socklen = sizeof(struct sockaddr_in);
	IMAGE img;
	while (true)
	{
		//wait clien connect
		SOCKET client_socket = accept(listen_socket, (struct sockaddr*)&client_addr, &socklen);
		if (INVALID_SOCKET == client_socket) continue;

		printf("new connect, ip: %s, port: %d \n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		while (true)
		{
			//loop to receive image info
			struct pack_header hdr;
			int ret = recv(client_socket, (char*)&hdr, sizeof(hdr), 0);
			if (ret <= 0)break;
			if (hdr.type == 0)
			{

			}
			else if (hdr.type == 1)
			{
				FILE* fp = fopen("s.jpg", "wb");
				
				int size = hdr.size;//rest of image size
				char buffer[4096] = { 0 };
				while (size > 0)
				{
					ret = recv(client_socket, buffer, size < 4096 ? size : 4096, 0);
					if (ret <= 0)
					{
						if (nullptr != fp)
						{
							fclose(fp);
						}
						goto end;
					}
					size -= ret;
					if (nullptr != fp)
					{
						fwrite(buffer, 1, ret, fp);
					}
				}
				if (nullptr != fp)
				{
					fclose(fp);
				}
				
			}
			//output info
			loadimage(&img, TEXT("s.jpg"), getwidth(), getheight());
			putimage(0, 0, &img);
		}
	end:
		closesocket(client_socket);
	}

	return 0;
}