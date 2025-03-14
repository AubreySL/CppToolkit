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
    return 0;
}
BOOL InjectDll(DWORD dwProcessId, LPTSTR lpDllPath)
{
    return true;
}
BOOL EjectDll(DWORD dwProcessId, LPTSTR lpDllPath)
{
    return true;
}