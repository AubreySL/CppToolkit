#include <Windows.h>
#include <tchar.h>


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    TCHAR szbuff[MAX_PATH] = {0};
    LPBYTE lpAddress = NULL;
    MEMORY_BASIC_INFORMATION mbi = { 0 };//page info
    int nLen;
    TCHAR szModName[MAX_PATH] = { 0 };
    HWND hwndRemoteApp;

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        //find target window
        hwndRemoteApp = FindWindow(TEXT("#32770"), TEXT("RemoteInjectApp"));
        
        while (VirtualQuery(lpAddress, &mbi, sizeof(mbi)) == sizeof(mbi))
        {
            //virtual page state is free
            if (mbi.State == MEM_FREE)
            {
                mbi.AllocationBase = mbi.BaseAddress;
            }
            if ((mbi.AllocationBase == NULL) || (mbi.AllocationBase == hModule) ||
                (mbi.BaseAddress != mbi.AllocationBase))
            {
                nLen = 0;
            }
            else {
                nLen = GetModuleFileName(HMODULE(mbi.AllocationBase), szModName, _countof(szModName));
            }

            if (nLen)
            {
                wsprintf(szbuff, TEXT("%p\t%s\r\n"), mbi.AllocationBase, szModName);
                //edit compoent show module name
                SendDlgItemMessage(hwndRemoteApp, 1005, EM_SETSEL, -1, -1);
                SendDlgItemMessage(hwndRemoteApp, 1005, EM_REPLACESEL, TRUE, (LPARAM)szbuff);
            }
            lpAddress += mbi.RegionSize;
        }
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
    default:
        break;
    }
    return true;
}