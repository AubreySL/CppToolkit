#include "framework.h"

#pragma comment(lib, "Ws2_32")

//const
const int BUF_SIZE = 1024;

//global
HWND g_hwnd;
HWND g_hListContent;
HWND g_hEditMsg;
HWND g_hBtnSend;

SOCKET g_socketClient = INVALID_SOCKET;//for client

//function statement
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID OnInit(HWND hwndDlg);
VOID OnConnect();
VOID OnSend();
DWORD WINAPI RecvProc(LPVOID lpParam);

//main entry
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_CLIENT_DIALOG), NULL, DialogProc, NULL);
    return 0;
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		OnInit(hwndDlg);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			//close socked, free lib
			if (g_socketClient != INVALID_SOCKET)
				closesocket(g_socketClient);
			WSACleanup();
			EndDialog(hwndDlg, IDCANCEL);
			break;
		case IDC_BTN_CONNECT:
			OnConnect();
			break;
		case IDC_BTN_SEND:
			OnSend();
			break;

		}
		return TRUE;
	}
	return FALSE;
}
VOID OnInit(HWND hwndDlg)
{
	g_hwnd = hwndDlg;
	g_hListContent = GetDlgItem(hwndDlg, IDC_LIST_CONTENT);
	g_hEditMsg = GetDlgItem(hwndDlg, IDC_EDIT_MSG);
	g_hBtnSend = GetDlgItem(hwndDlg, IDC_BTN_SEND);

	EnableWindow(g_hBtnSend, FALSE);

	return;
}
VOID OnConnect() {

	WSADATA wsa = { 0 };
	sockaddr_in sockAddrServer;
	//init winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		MessageBox(g_hwnd, TEXT("初始化Winsock库失败！"), TEXT("WSAStartup Error"), MB_OK);
		return;
	}
	//create socket for listen
	g_socketClient = socket(AF_INET, SOCK_STREAM, 0);
	if (g_socketClient == INVALID_SOCKET)
	{
		MessageBox(g_hwnd, TEXT("创建与服务器的通信套接字失败！"), TEXT("socket Error"), MB_OK);
		WSACleanup();
		return;
	}

	//connect server socket
	sockAddrServer.sin_family = AF_INET;
	sockAddrServer.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &sockAddrServer.sin_addr.S_un.S_addr);
	if (connect(g_socketClient, (sockaddr*)&sockAddrServer, sizeof(sockAddrServer)) == SOCKET_ERROR)
	{
		MessageBox(g_hwnd, TEXT("与服务器建立连接失败！"), TEXT("connect Error"), MB_OK);
		closesocket(g_socketClient);
		WSACleanup();
		return;

	}

	//create client connect and send data
	SendMessageA(g_hListContent, LB_ADDSTRING, 0, (LPARAM)"已连接到服务器！");
	EnableWindow(GetDlgItem(g_hwnd, IDC_BTN_CONNECT), FALSE);
	EnableWindow(g_hBtnSend, TRUE);
	//create thread to receive server data
	CreateThread(NULL, 0, RecvProc, NULL, 0, NULL);
	return;
}

VOID OnSend()
{
	
	CHAR szBuf[BUF_SIZE] = { 0 };
	CHAR szShow[BUF_SIZE] = { 0 };

	GetWindowTextA(g_hEditMsg, szBuf, BUF_SIZE);
	wsprintfA(szShow, "客户端说：%s", szBuf);
	SendMessageA(g_hListContent, LB_ADDSTRING, 0, (LPARAM)szShow);
	send(g_socketClient, szShow, strlen(szShow), 0);

	SetWindowText(g_hEditMsg, TEXT(""));

	return;
}

DWORD WINAPI RecvProc(LPVOID lpParam)
{
	CHAR szBuf[BUF_SIZE] = { 0 };
	int nRet;

	while (TRUE)
	{
		ZeroMemory(szBuf, BUF_SIZE);
		nRet = recv(g_socketClient, szBuf, BUF_SIZE, 0);
		if (nRet > 0)
		{
			SendMessageA(g_hListContent, LB_ADDSTRING, 0, (LPARAM)szBuf);
		}
	}
	return 0;
}
