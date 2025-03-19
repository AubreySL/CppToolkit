#include "framework.h"
#include "resource.h"

HWND g_hwndDlg = NULL;

//function statement
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI ThreadProc(LPVOID lpParameter);
BOOL InjectDll(DWORD dwProcessId, LPTSTR lpDllPath);
BOOL EjectDll(DWORD dwProcessId, LPTSTR lpDllPath);

//main entry
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DialogProc, NULL);
    return 0;
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HANDLE hThread;
    DWORD dwProcessId;
    TCHAR szDllPath[MAX_PATH] = { 0 };

    switch (uMsg)
    {
    case WM_INITDIALOG:
        g_hwndDlg = hwndDlg;

        SetDlgItemText(hwndDlg, IDC_EDIT_PROCESSID, TEXT("«Î ‰»ÎΩ¯≥ÃID"));
        SetDlgItemText(hwndDlg, IDC_EDIT_DLLPATH,
            TEXT("RemoteInjectDll.dll"));
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BTN_INJECT:
            //create a new thread for injecting dll
            hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
            if (hThread)
            {
                CloseHandle(hThread);
            }
            break;
        case IDC_BTN_EJECT:
            dwProcessId = GetDlgItemInt(hwndDlg, IDC_EDIT_PROCESSID, NULL, FALSE);
            GetDlgItemText(hwndDlg, IDC_EDIT_DLLPATH, szDllPath, _countof(szDllPath));
            EjectDll(dwProcessId, szDllPath);
            break;
        case IDCANCEL:
            EndDialog(hwndDlg, 0);
        default:
            break;
        }
        return TRUE;
    default:
        break;
    }
    return FALSE;
}
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
    DWORD dwProcessId;
    TCHAR szDllPath[MAX_PATH] = { 0 };

    dwProcessId = GetDlgItemInt(g_hwndDlg, IDC_EDIT_PROCESSID, NULL, FALSE);
    GetDlgItemText(g_hwndDlg, IDC_EDIT_DLLPATH, szDllPath, _countof(szDllPath));
    return InjectDll(dwProcessId, szDllPath);
}
BOOL InjectDll(DWORD dwProcessId, LPTSTR lpDllPath)
{
    HANDLE hProcess = NULL;
    LPTSTR lpDllPathRemote = NULL;
    HANDLE hThread = NULL;

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_CREATE_PROCESS |
        PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, dwProcessId);
    if (!hProcess) return FALSE;

    int cbDllPath = (_tcslen(lpDllPath) + 1) * sizeof(TCHAR);
    lpDllPathRemote = (LPTSTR)VirtualAllocEx(hProcess, NULL, cbDllPath,
        MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!lpDllPathRemote) return FALSE;

    if (!WriteProcessMemory(hProcess, lpDllPathRemote, lpDllPath, cbDllPath, NULL))
        return FALSE;

    PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
        GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
    if (!pfnThreadRtn)
        return FALSE;

    hThread = CreateRemoteThread(hProcess, NULL, 0, pfnThreadRtn, lpDllPathRemote, 0, NULL);
    if (!hThread)
        return FALSE;
    
    WaitForSingleObject(hThread, INFINITE);

    if (!lpDllPathRemote)
        VirtualFreeEx(hProcess, lpDllPathRemote, 0, MEM_RELEASE);
    if (hThread)
        CloseHandle(hThread);
    if (hProcess)
        CloseHandle(hProcess);

    return TRUE;
}
BOOL EjectDll(DWORD dwProcessId, LPTSTR lpDllPath)
{
    HANDLE hSnapshot;
    MODULEENTRY32 me = { sizeof(MODULEENTRY32) };
    BOOL bRet;
    BOOL bFound = FALSE;
    HANDLE hProces = NULL;
    HANDLE hThread = NULL;

    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return FALSE;

    bRet = Module32First(hSnapshot, &me);
    while (bRet)
    {
        if (_tcsicmp(TEXT("RemoteInjectDll.dll"), me.szModule) == 0 || _tcsicmp(lpDllPath, me.szExePath) == 0)
        {
            bFound = TRUE;
            break;
        }
        bRet = Module32Next(hSnapshot, &me);
    }

    if (!bFound)
        return FALSE;

    hProces = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION,
        FALSE, dwProcessId);
    if (!hProces)
        return FALSE;

    PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
        GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "FreeLibrary");
    if (!pfnThreadRtn)
        return FALSE;

    hThread = CreateRemoteThread(hProces, NULL, 0, pfnThreadRtn, me.modBaseAddr, 0, NULL);
    if (!hThread)
        return FALSE;

    WaitForSingleObject(hThread, INFINITE);

    if (hSnapshot != INVALID_HANDLE_VALUE)
        CloseHandle(hSnapshot);
    if (hThread)
        CloseHandle(hThread);
    if (hProces)
        CloseHandle(hProces);
    
    return TRUE;
}