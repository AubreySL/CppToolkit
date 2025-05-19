#pragma once
import <string>;
#include <Windows.h>;

class Dialog
{
public:
	Dialog() = default;
	~Dialog() = default;

	void createDialog();
	
	void setTitle(const std::string& title);

	static HWND getComponent(int id);
	static void setContent(int id, LPCSTR txt);
	static HWND getDialog();
	static inline HWND m_dialog;
private:
	std::string m_title;
	static INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static std::string WcharToChar(const wchar_t* wp, size_t m_encode = CP_UTF8);
};

