﻿// 01-draw_clock.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "draw_clock.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING]=TEXT("MyWindow");                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING]=TEXT("Clock");            // 主窗口类名

//define const
const int CLOCK_SIZE = 400;
const double TWOPI = 2 * 3.1415926;

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

VOID DrawDots(HDC hdc);
VOID DrawLine(HDC hdc, int nAngle, int nRadiusAdjust);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY01DRAWCLOCK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY01DRAWCLOCK));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY01DRAWCLOCK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    //wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY01DRAWCLOCK);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP|WS_SYSMENU|WS_VISIBLE,
      200, 100, CLOCK_SIZE, CLOCK_SIZE, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{   
    HDC hdc;
    PAINTSTRUCT ps;
    HRGN hRgn;
    SYSTEMTIME st;


    switch (message)
    {
    case WM_CREATE:
    {
        //draw ellipse area
        hRgn = CreateEllipticRgn(0, 0, CLOCK_SIZE, CLOCK_SIZE);
        SetWindowRgn(hWnd, hRgn, TRUE);
        //set top window
        SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        //set timer
        SetTimer(hWnd, 1, 1000, NULL);
        return 0;
    }
    case WM_TIMER:
    {
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_LBUTTONDOWN: {
        //drag window
        SetCursor(LoadCursor(NULL, IDC_HAND));
        SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        SetCursor(LoadCursor(NULL, IDC_ARROW));
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // draw clock
            GetLocalTime(&st);
            //set stock
            SelectObject(hdc, GetStockObject(BLACK_BRUSH));
            DrawDots(hdc);
            //
            SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
            DrawLine(hdc, st.wSecond * 6, 10);

            DeleteObject(SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(0, 0, 0))));
            DrawLine(hdc, st.wMinute * 6, 20);

            DeleteObject(SelectObject(hdc, CreatePen(PS_SOLID, 3, RGB(0, 0, 0))));
            DrawLine(hdc, (st.wHour%12)*30+st.wMinute/2, 30);
            DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));

            SelectObject(hdc, GetStockObject(WHITE_BRUSH));
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_CHAR:
    {
        switch (wParam)
        {
        case '1'://change window backround
            SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND,
                (LONG)CreatePatternBrush((HBITMAP)LoadImage(NULL, TEXT("images/black1.bmp"),
                    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)));
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case '2':
            SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND,
                (LONG)CreatePatternBrush((HBITMAP)LoadImage(NULL, TEXT("images/black2.bmp"),
                    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)));
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 0x1b://esc
            SendMessage(hWnd, WM_CLOSE, 0, 0);
            break;
        }
        return 0;
    }
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

VOID DrawDots(HDC hdc)
{
    int x, y;
    int nRadius;

    for (int nAngle=0; nAngle < 360; nAngle+=6)
    {
        x = CLOCK_SIZE / 2 + (int)((CLOCK_SIZE/2-4)*sin(TWOPI*nAngle/360));
        y = CLOCK_SIZE / 2 - (int)((CLOCK_SIZE/2-4)*cos(TWOPI*nAngle/360));
        nRadius = nAngle % 5 ? 2 : 4;
        Ellipse(hdc, x - nRadius, y - nRadius, x + nRadius, y + nRadius);
    }
}
VOID DrawLine(HDC hdc, int nAngle, int nRadiusAdjust) 
{
    int x1, x2, y1, y2;
    x1 = CLOCK_SIZE / 2 + (int)(((CLOCK_SIZE / 2 - 4) - nRadiusAdjust) * sin(TWOPI * nAngle / 360));
    y1 = CLOCK_SIZE / 2 - (int)(((CLOCK_SIZE / 2 - 4) - nRadiusAdjust) * cos(TWOPI * nAngle / 360));
    x2 = CLOCK_SIZE / 2 + (int)(10 * sin(TWOPI * ((double)nAngle + 180) / 360));
    y2 = CLOCK_SIZE / 2 - (int)(10 * cos(TWOPI * ((double)nAngle + 180) / 360));
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
}


