#include <windows.h>
#include <intrin.h>
#include "resource.h"
#include <stdio.h>

// 函数声明
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
char* reverseCharArrayNew(const char* str);
char* hexToAsciiArraySafe(const char* hex, int len);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, NULL);
    return 0;
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int arrCpuInfo[4] = { 0 };
    TCHAR szbuf[32] = { 0 };
    TCHAR szbuf2[32] = { 0x54, 0 };
    CHAR szbuf3[64] = { 0 };
    CHAR szbuf4[64] = { 0x54, 0 };
    switch (uMsg)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        {
            __cpuid(arrCpuInfo, 1);
            wsprintfA(szbuf3, ("%08X%08X"), arrCpuInfo[3], arrCpuInfo[0]);
            SetDlgItemTextA(hwndDlg, IDC_EDIT1, szbuf3);

            //show cpu info
            __cpuid(arrCpuInfo, 0x80000002);
            wsprintfA(szbuf3,("%08X%08X%08X%08X"), arrCpuInfo[3], arrCpuInfo[2], arrCpuInfo[1], arrCpuInfo[0]);
            LPSTR plainText = hexToAsciiArraySafe(szbuf3, strlen(szbuf3));
            SetDlgItemTextA(hwndDlg, IDC_EDIT2, plainText);
            /*SendMessageA(GetDlgItem(hwndDlg, IDC_EDIT2), EM_SETSEL, WPARAM(-2), LPARAM(-1));
            SendMessageA(GetDlgItem(hwndDlg, IDC_EDIT2), EM_REPLACESEL, WPARAM(TRUE), LPARAM(plainText));*/
            
            __cpuid(arrCpuInfo, 0x80000003);
            wsprintfA(szbuf3, ("%08X%08X%08X"), arrCpuInfo[2], arrCpuInfo[1], arrCpuInfo[0]);
            plainText = hexToAsciiArraySafe(szbuf3, strlen(szbuf3));
            SendMessageA(GetDlgItem(hwndDlg, IDC_EDIT2), EM_SETSEL, WPARAM(-2), LPARAM(-1));
            SendMessageA(GetDlgItem(hwndDlg, IDC_EDIT2), EM_REPLACESEL, WPARAM(TRUE), LPARAM(plainText));
           
            //__cpuid(arrCpuInfo, 0x80000004);
            //wsprintfA(szbuf3, ("%08X%08X"), arrCpuInfo[1], arrCpuInfo[0]);
            //plainText = hexToAsciiArraySafe(szbuf3, strlen(szbuf3));
            //SendMessageA(GetDlgItem(hwndDlg, IDC_EDIT2), EM_SETSEL, WPARAM(-2), LPARAM(-1));
            //SendMessageA(GetDlgItem(hwndDlg, IDC_EDIT2), EM_REPLACESEL, WPARAM(TRUE), LPARAM(plainText));
            break;
        }
        case IDCANCEL:
            EndDialog(hwndDlg, 0);
            break;
        }
        return TRUE;

    }

    return FALSE;
}

char* reverseCharArrayNew(const char* str) {
    if (str == nullptr) return nullptr;
    size_t len = strlen(str);
    char* reversed = new char[len + 1]; // 分配新内存（含终止符）
    reversed[len] = '\0';               // 提前设置终止符

    for (size_t i = 0; i < len; i++) {
        reversed[i] = str[len - i - 1]; // 反向填充字符‌
    }
    return reversed;
}

char* hexToAsciiArraySafe(const char* hex, int len) {
    if (len % 2 != 0) return nullptr; // 输入长度需为偶数
    int outputLen = len / 2 + 1;       // 结果长度（含终止符）

    char* result = new char[outputLen];
    memset(result, 0, outputLen);     // 初始化清零‌

    for (int i = 0, j = 0; i < len; i += 2, j++) {
        unsigned int byte;
        // 使用 sscanf_s 替代 sscanf，避免缓冲区溢出风险‌
        if (sscanf_s(hex + i, "%02X", &byte) != 1) {
            delete[] result;
            return nullptr; // 解析失败则释放内存‌
        }
        result[j] = static_cast<char>(byte); // 转换为 ASCII 
    }
    result[outputLen - 1] = '\0'; // 添加终止符‌
    return reverseCharArrayNew(result);
    //return result;
}

