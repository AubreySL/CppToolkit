#include "framework.h"
#include "SOCKETOBJ.h"

#pragma comment(lib, "Ws2_32")

//const
const int BUF_SIZE = 1024;

//global
HWND g_hwnd;
HWND g_hListContent;
HWND g_hEditMsg;
HWND g_hBtnSend;

SOCKET g_socketListen = INVALID_SOCKET;//for listen
SOCKET g_socketAccept = INVALID_SOCKET;//for accept

//function statement
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID OnInit(HWND hwndDlg);
VOID OnStart();
VOID OnSend();
DWORD WINAPI AcceptProc(LPVOID lpParam);
DWORD WINAPI RecvProc(LPVOID lpParam);

//main entry
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_SERVER_DIALOG), NULL, DialogProc, NULL);
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
			if (g_socketAccept != INVALID_SOCKET)
				closesocket(g_socketAccept);
			if (g_socketListen != INVALID_SOCKET)
				closesocket(g_socketListen);
			WSACleanup();
			EndDialog(hwndDlg, IDCANCEL);
			break;
		case IDC_BTN_START:
			OnStart();
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

	WSADATA wsa = { 0 };

	//init winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		MessageBox(g_hwnd, TEXT("初始化Winsock库失败！"), TEXT("WSAStartup Error"), MB_OK);
		return;
	}

	//init critical section
	InitializeCriticalSection(&g_cs);

	g_hwnd = hwndDlg;
	g_hListContent = GetDlgItem(hwndDlg, IDC_LIST_CONTENT);
	g_hEditMsg = GetDlgItem(hwndDlg, IDC_EDIT_MSG);
	g_hBtnSend = GetDlgItem(hwndDlg, IDC_BTN_SEND);

	EnableWindow(g_hBtnSend, FALSE);

	return;
}
VOID OnStart() {
	//create socket for listen
	g_socketListen = socket(AF_INET, SOCK_STREAM, 0);
	if (g_socketListen == INVALID_SOCKET)
	{
		MessageBox(g_hwnd, TEXT("创建监听套接字失败！"), TEXT("socket Error"), MB_OK);
		WSACleanup();
		return;
	}

	//bind ip and port to socket
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(8000);
	sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(g_socketListen, (sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
	{
		MessageBox(g_hwnd, TEXT("将监听套接字与指定的IP地址、端口号捆绑失败！"), TEXT("bind Error"), MB_OK);
		closesocket(g_socketListen);
		WSACleanup();
		return;
	}

	//make socket to listen:max conncect count 
	if (listen(g_socketListen, SOMAXCONN) == SOCKET_ERROR)
	{
		MessageBox(g_hwnd, TEXT("使监听套接字进入监听（等待被连接）失败！"), TEXT("listen Error"), MB_OK);
		closesocket(g_socketListen);
		WSACleanup();
		return;
	}

	//server start listening
	MessageBox(g_hwnd, TEXT("服务器监听中..."), TEXT("服务启动成功"), MB_OK);
	EnableWindow(GetDlgItem(g_hwnd, IDC_BTN_START), FALSE);

	//create thread to accept client data
	HANDLE hThread;
	hThread = CreateThread(NULL, 0, AcceptProc, NULL, 0, NULL);
	if (hThread != NULL)
	{
		CloseHandle(hThread);
		hThread = NULL;
	}
	return;
}
VOID OnSend() {

	CHAR szBuf[BUF_SIZE] = { 0 };
	CHAR szShow[BUF_SIZE] = { 0 };
	GetWindowTextA(g_hEditMsg, szBuf, BUF_SIZE);
	wsprintfA(szShow, "服务器说：%s", szBuf);
	SendMessageA(g_hListContent, LB_ADDSTRING, 0, (LPARAM)szShow);
	send(g_socketAccept, szShow, strlen(szShow), 0);
	SetWindowText(g_hEditMsg, TEXT(""));

	//send to other socketobj
	PSOCKETOBJ p = g_pSocketObjHeader;
	while (p != NULL)
	{
		send(p->m_socket, szShow, strlen(szShow), 0);
		p = p->m_pNext;
	}

	return;
}

DWORD WINAPI AcceptProc(LPVOID lpParam)
{
	SOCKET socketAccept = INVALID_SOCKET;
	sockaddr_in sockAddrClient;
	int nAddrLen = sizeof(sockaddr_in);
	CHAR szBuf[BUF_SIZE] = { 0 };

	while (TRUE)
	{
		socketAccept = accept(g_socketListen, (sockaddr*)&sockAddrClient, &nAddrLen);
		if (socketAccept == INVALID_SOCKET)
		{
			Sleep(100);
			continue;
		}

		ZeroMemory(szBuf, BUF_SIZE);

		//create socketobj to save ip and port
		PSOCKETOBJ pSocketObj = CreateSocketObj(socketAccept);
		inet_ntop(AF_INET, &sockAddrClient.sin_addr.S_un.S_addr, pSocketObj->m_szIP, _countof(pSocketObj->m_szIP));
		pSocketObj->m_usPort = ntohs(sockAddrClient.sin_port);
		wsprintfA(szBuf, "客户端[%s:%d]已连接！", pSocketObj->m_szIP, pSocketObj->m_usPort);
		SendMessageA(g_hListContent, LB_ADDSTRING, 0, (LPARAM)szBuf);
		EnableWindow(g_hBtnSend, TRUE);

		//create recv thread
		CloseHandle(CreateThread(NULL, 0, RecvProc, (LPVOID)socketAccept, 0, NULL));

	}

	return 0;
}

DWORD WINAPI RecvProc(LPVOID lpParam)
{
	SOCKET socketAccept = (SOCKET)lpParam;
	PSOCKETOBJ pSocketObj = FindSocketObj(socketAccept);

	CHAR szBuf[BUF_SIZE] = { 0 };
	CHAR szMsg[BUF_SIZE] = { 0 };
	int nRet;
	PSOCKETOBJ p;

	while (TRUE)
	{
		ZeroMemory(szBuf, BUF_SIZE);
		nRet = recv(socketAccept, szBuf, BUF_SIZE, 0);
		if (nRet > 0)
		{
			ZeroMemory(szMsg, BUF_SIZE);
			wsprintfA(szMsg, "客户端[%s:%d]说：%s", pSocketObj->m_szIP, pSocketObj->m_usPort, szBuf);
			SendMessageA(g_hListContent, LB_ADDSTRING, 0, (LPARAM)szMsg);

			//send to other socketobj
			p = g_pSocketObjHeader;
			while (p != NULL)
			{
				if (p->m_socket != socketAccept)
					send(p->m_socket, szMsg, strlen(szMsg), 0);

				p = p->m_pNext;
			}
		}
		else
		{//connect is closed
			ZeroMemory(szMsg, BUF_SIZE);
			wsprintfA(szMsg, "客户端[%s:%d]已退出！", pSocketObj->m_szIP, pSocketObj->m_usPort);
			SendMessageA(g_hListContent, LB_ADDSTRING, 0, (LPARAM)szMsg);
			FreeSocketObj(pSocketObj);
			
			if (g_nTotalClient == 0)
			{
				EnableWindow(g_hBtnSend, FALSE);
			}
			return 0;
		}
	}
	return 0;
}
