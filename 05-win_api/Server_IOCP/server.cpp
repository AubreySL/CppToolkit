#include "framework.h"
#include "SOCKETOBJ.h"

#pragma comment(lib, "Ws2_32")
#pragma comment(lib, "Mswsock")


//const
const int BUF_SIZE = 1024;

//I/O type
enum IOOPERATION
{
	IO_ACCEPT, IO_READ, IO_WRITE  
};
//OVERLAPPED struct and IO data
typedef struct _PERIODATA
{
	OVERLAPPED m_overlapped;
	SOCKET m_socket;
	WSABUF m_wsaBuf;
	CHAR m_szBuffer[BUF_SIZE];
	IOOPERATION m_ioOperation;
}PERIODATA, *PPERIODATA;

//global
HWND g_hwnd;
HWND g_hListContent;
HWND g_hEditMsg;
HWND g_hBtnSend;

SOCKET g_socketListen = INVALID_SOCKET;//for listen
HANDLE g_hCompletionPort;//handle of IO completion port

//function statement
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID OnInit(HWND hwndDlg);
VOID OnStart();
VOID CALLBACK WorkerThreadProc(PTP_CALLBACK_INSTANCE Instance, PVOID Context);
BOOL PostAccept();
BOOL PostSend(SOCKET s, LPSTR pStr, int nLen);
BOOL PostRecv(SOCKET s);
VOID OnSend();
ULARGE_INTEGER GetProcessorInformation();


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
			//close 
			if (g_hCompletionPort)
				CloseHandle(g_hCompletionPort);
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

	ULARGE_INTEGER uli = { 0 };

	//create socket for listen
	g_socketListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
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

	//create IO completion port
	g_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	//set thread pool callback
	TrySubmitThreadpoolCallback(WorkerThreadProc, NULL, NULL);


	//add listen socket node
	PSOCKETOBJ pSocketObj = CreateSocketObj(g_socketListen);
	//relate socket and io completion port 
	CreateIoCompletionPort((HANDLE)g_socketListen, g_hCompletionPort, (ULONG_PTR)pSocketObj, 0);

	uli = GetProcessorInformation();
	for (DWORD i = 0; i < uli.HighPart*2; ++i)
	{
		PostAccept();
	}
}
//TrySubmitThreadpoolCallback callback
VOID WorkerThreadProc(PTP_CALLBACK_INSTANCE Instance, PVOID Context)
{
	sockaddr_in* pRemoteSockaddr;
	sockaddr_in* pLocalSockaddr;
	int nAddrlen = sizeof(sockaddr_in);
	PSOCKETOBJ pSocketObj = NULL;
	PPERIODATA pPerIOData = NULL;
	DWORD dwTrans;
	PSOCKETOBJ pSocket;
	BOOL bRet;
	PSOCKETOBJ p;
	CHAR szBuf[BUF_SIZE] = { 0 };

	while (TRUE)
	{
		bRet = GetQueuedCompletionStatus(g_hCompletionPort, &dwTrans,
			(PULONG_PTR)&pSocketObj, (LPOVERLAPPED*)&pPerIOData, INFINITE);
		if (!bRet)
		{
			if (GetLastError() == ERROR_ABANDONED_WAIT_0)//io port is closed
			{
				break;
			}
			else
			{
				ZeroMemory(szBuf, BUF_SIZE);
				wsprintfA(szBuf, "客户端[%s:%d]已退出！", pSocketObj->m_szIP, pSocketObj->m_usPort);
				SendMessageA(g_hListContent, LB_ADDSTRING, 0, (LPARAM)szBuf);
			
				FreeSocketObj(pSocketObj);
				delete pPerIOData;

				if (g_nTotalClient == 1)
				{
					EnableWindow(g_hBtnSend, FALSE);
				}
				continue;
			}
		}

		// handle completed io operation
		switch (pPerIOData->m_ioOperation)
		{
		case IO_ACCEPT:
		{
			pSocket = CreateSocketObj(pPerIOData->m_socket);

			CreateIoCompletionPort((HANDLE)pSocket->m_socket, g_hCompletionPort, (ULONG_PTR)pSocket, 0);
			
			ZeroMemory(szBuf, BUF_SIZE);
			GetAcceptExSockaddrs(pPerIOData->m_szBuffer, 0, sizeof(sockaddr_in) + 16,
				sizeof(sockaddr_in) + 16, (LPSOCKADDR*)&pLocalSockaddr, &nAddrlen,
				(LPSOCKADDR*)&pRemoteSockaddr, &nAddrlen);
			inet_ntop(AF_INET, &pRemoteSockaddr->sin_addr.S_un.S_addr,
				pSocket->m_szIP, _countof(pSocket->m_szIP));
			pSocket->m_usPort = ntohs(pRemoteSockaddr->sin_port);
			wsprintfA(szBuf, "客户端[%s:%d]已连接！", pSocket->m_szIP, pSocket->m_usPort);
			SendMessageA(g_hListContent, LB_ADDSTRING, 0, (LPARAM)szBuf);
			EnableWindow(g_hBtnSend, TRUE);

			//free
			delete pPerIOData;

			PostRecv(pSocket->m_socket);

			PostAccept();
			break;
		}
		case IO_READ:
		{
			ZeroMemory(szBuf, BUF_SIZE);
			wsprintfA(szBuf, "客户端[%s:%d]说：%s", pSocketObj->m_szIP, pSocketObj->m_usPort, pPerIOData->m_szBuffer);
			SendMessageA(g_hListContent, LB_ADDSTRING, 0, (LPARAM)szBuf);

			//free
			delete pPerIOData;

			//dispatch to every client
			p = g_pSocketObjHeader;
			while (p != NULL)
			{
				if (p->m_socket != g_socketListen && p->m_socket != pSocketObj->m_socket)
				{
					PostSend(p->m_socket, szBuf, strlen(szBuf));
				}

				p = p->m_pNext;
			}

			PostRecv(pSocketObj->m_socket);
			break;
		}
		case IO_WRITE:
		{
			delete pPerIOData;
			break;
		}
		}
	}
	return VOID();
}
//post accept io request
BOOL PostAccept()
{
	PPERIODATA pPerIOData = new PERIODATA;
	ZeroMemory(&pPerIOData->m_overlapped, sizeof(OVERLAPPED));
	pPerIOData->m_ioOperation = IO_ACCEPT;

	SOCKET socketAccept = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	pPerIOData->m_socket = socketAccept;

	BOOL bRet = AcceptEx(g_socketListen, socketAccept, pPerIOData->m_szBuffer, 0,
		sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, (LPOVERLAPPED)pPerIOData);
	if (!bRet)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			return FALSE;
	}
	return TRUE;
}
//post send io request
BOOL PostSend(SOCKET s, LPSTR pStr, int nLen)
{
	DWORD dwFlags = 0;
	PPERIODATA pPerIOData = new PERIODATA;
	ZeroMemory(&pPerIOData->m_overlapped, sizeof(OVERLAPPED));
	pPerIOData->m_ioOperation = IO_WRITE;
	ZeroMemory(pPerIOData->m_szBuffer, BUF_SIZE);
	StringCchCopyA(pPerIOData->m_szBuffer, BUF_SIZE, pStr);
	pPerIOData->m_wsaBuf.buf = pPerIOData->m_szBuffer;
	pPerIOData->m_wsaBuf.len = BUF_SIZE;

	int nRet = WSASend(s, &pPerIOData->m_wsaBuf, 1, NULL, dwFlags, (LPOVERLAPPED)pPerIOData, NULL);
	if (nRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			return FALSE;
	}
	return TRUE;
}
//post recv io request
BOOL PostRecv(SOCKET s)
{
	DWORD dwFlags = 0;
	PPERIODATA pPerIOData = new PERIODATA;
	ZeroMemory(&pPerIOData->m_overlapped, sizeof(OVERLAPPED));
	pPerIOData->m_ioOperation = IO_READ;
	ZeroMemory(pPerIOData->m_szBuffer, BUF_SIZE);
	pPerIOData->m_wsaBuf.buf = pPerIOData->m_szBuffer;
	pPerIOData->m_wsaBuf.len = BUF_SIZE;

	int nRet = WSARecv(s, &pPerIOData->m_wsaBuf, 1, NULL, &dwFlags, (LPOVERLAPPED)pPerIOData, NULL);
	if (nRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			return FALSE;
	}
	return TRUE;
}
VOID OnSend() {

	CHAR szBuf[BUF_SIZE] = { 0 };
	CHAR szShow[BUF_SIZE] = { 0 };

	GetWindowTextA(g_hEditMsg, szBuf, BUF_SIZE);
	wsprintfA(szShow, "服务器说：%s", szBuf);
	SendMessageA(g_hListContent, LB_ADDSTRING, 0, (LPARAM)szShow);
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

/*
* reference:
*	https://learn.microsoft.com/zh-cn/windows/win32/api/sysinfoapi/nf-sysinfoapi-getlogicalprocessorinformation
*/
// Helper function to count set bits in the processor mask.
DWORD CountSetBits(ULONG_PTR bitMask)
{
	DWORD LSHIFT = sizeof(ULONG_PTR) * 8 - 1;
	DWORD bitSetCount = 0;
	ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
	DWORD i;

	for (i = 0; i <= LSHIFT; ++i)
	{
		bitSetCount += ((bitMask & bitTest) ? 1 : 0);
		bitTest /= 2;
	}

	return bitSetCount;
}

ULARGE_INTEGER GetProcessorInformation()
{
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION pBuf = NULL;    // 缓冲区指针
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION pTemp = NULL;   // 临时指针
	DWORD          dwReturnedLength = 0;       // GetLogicalProcessorInformation函数所用参数
	DWORD          dwLogicalProcessorCount = 0;// 逻辑处理器个数
	DWORD          dwProcessorCoreCount = 0;   // 物理处理器个数
	DWORD          dwByteOffset = 0;           // 
	ULARGE_INTEGER uli = { 0 };                // 返回值，低、高DWORD分别表示逻辑、物理处理器个数

	// 第1次调用
	GetLogicalProcessorInformation(pBuf, &dwReturnedLength);
	pBuf = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)new BYTE[dwReturnedLength];

	// 第2次调用
	GetLogicalProcessorInformation(pBuf, &dwReturnedLength);

	pTemp = pBuf;
	while (dwByteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= dwReturnedLength)
	{
		if (pTemp->Relationship == RelationProcessorCore)
		{
			// 物理处理器个数
			dwProcessorCoreCount++;
			// 逻辑处理器个数，一个超线程核心可以有多个逻辑处理器
			dwLogicalProcessorCount += CountSetBits(pTemp->ProcessorMask);
		}

		dwByteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
		pTemp++;
	}

	delete[] pBuf;
	uli.LowPart = dwLogicalProcessorCount;
	uli.HighPart = dwProcessorCoreCount;
	return uli;
}
