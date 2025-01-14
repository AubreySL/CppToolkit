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

int main()
{
	// permit access web
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// get windows desktop
	HWND hDeskWin = GetDesktopWindow();
	// get paint device
	HDC hDeskDc = GetWindowDC(hDeskWin);

	// get desktop width and height
	int nDeskWidth = GetDeviceCaps(hDeskDc, HORZRES);
	int nDeskHeight = GetDeviceCaps(hDeskDc, VERTRES);

	// create a image to store desktop image
	IMAGE backgroundImage = IMAGE(nDeskWidth,nDeskHeight);

	// get hdc
	HDC hImageDc = GetImageHDC(&backgroundImage);
	
	// create a socket
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == client_socket)
	{
		printf("create socket failed !!!\n");
		return -1;
	}

	//connect server
	struct sockaddr_in target = { 0 };
	target.sin_family = AF_INET;
	target.sin_port = htons(8080);
	target.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (-1 == connect(client_socket, (struct sockaddr*)&target, sizeof(struct sockaddr)))
	{
		printf("connect server failed!!! \n");
		return -1;
	}


	while (true)
	{
		//copy desktop image to image
		BitBlt(hImageDc, 0, 0, nDeskWidth, nDeskHeight, hDeskDc, 0, 0, SRCCOPY);
		//save image
		saveimage(TEXT("1.jpg"), &backgroundImage);
		/*loadimage(&backgroundImage, L"1.jpg", 1280, 720);
		putimage(0, 0, 1280, 720, &backgroundImage, 0, 0);*/

		//send data
		struct pack_header hdr={ 1 };
		FILE* fp = fopen("1.jpg", "rb");
		if (nullptr == fp) continue;
		fseek(fp, 0, SEEK_END);
		//get file size
		hdr.size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		if (hdr.size <= 0)
		{
			fclose(fp);
			continue;
		}
		//send header
		int ret = send(client_socket, (const char*)&hdr, sizeof(hdr), 0);
		if (ret <= 0)break;

		//send data
		char buffer[1024] = { 0 };
		while (true)
		{
			ret = fread(buffer, 1, 1024, fp);
			if (ret <= 0)break;

			ret = send(client_socket, buffer, ret, 0);

			if (ret <= 0) {
				fclose(fp);
				goto end;
			}

		}
		fclose(fp);
	}
end:
	closesocket(client_socket);
	return 0;
}