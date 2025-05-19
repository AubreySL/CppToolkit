
import <iostream>;
import "Dialog.h";
import "UserConfig.h";
import <thread>;
#include<Windows.h>
#include "resource.h"

using namespace std;

int main()
{
	auto dialog{ make_unique<Dialog>() };
	dialog->createDialog();

}

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//{
//	auto dialog{ make_unique<Dialog>() };
//	dialog->createDialog();
//
//	return 0;
//}
