import "UserConfig.h";
import "XFTTS.h";
import "Dialog.h";

#include <Windows.h>
#include "resource.h"

void Dialog::createDialog()
{
	DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, NULL);
}

void Dialog::setTitle(const std::string& title)
{
	m_title = title;
}

HWND Dialog::getComponent(int id)
{
	return GetDlgItem(m_dialog, id);
}

void Dialog::setContent(int id, LPCSTR txt)
{
	SetDlgItemTextA(m_dialog, id, txt);
}


HWND Dialog::getDialog()
{
	return m_dialog;
}


INT_PTR CALLBACK Dialog::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_dialog = hwndDlg;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			//close 
			EndDialog(hwndDlg, IDCANCEL);
			break;
		case IDC_BUTTON_PLAY:
		{
			HWND hWndEdit = getComponent(IDC_EDIT_CONTENT);
			int nLen = GetWindowTextLength(hWndEdit) + 1;
			if (nLen < 3)
			{
				MessageBox(NULL, TEXT("输入内容过少"), TEXT("警告"), MB_OK);
				break;
			}
			TCHAR* buffer = new TCHAR[nLen];
			SendMessage(hWndEdit, WM_GETTEXT, nLen, (LPARAM)buffer);

			auto tts{ std::make_unique<XFTTS>() };
			tts->textToSpeech(WcharToChar(buffer));

			delete[] buffer;
			buffer = nullptr;
			break;
		}
		}
		return TRUE;
	}
	return FALSE;
}

std::string Dialog::WcharToChar(const wchar_t* wp, size_t m_encode)
{
	std::string str;
	int len = WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
	char* m_char = new char[len + 1];
	WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	str = m_char;
	delete m_char;
	m_char = nullptr;
	return str;
}