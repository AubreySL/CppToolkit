// KeyboardDll.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。
#pragma once
#ifdef DLL_EXPORT
#define DLL_API extern "C" __declspec(dllexport)
#else
#define DLL_API extern "C" __declspec(dllimport)
#endif // !DLL_EXPORT

//export function
DLL_API BOOL InstallHook(int idHook, DWORD dwThreadId, HWND hwnd);
DLL_API BOOL UninstallHook();

//inner function
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

