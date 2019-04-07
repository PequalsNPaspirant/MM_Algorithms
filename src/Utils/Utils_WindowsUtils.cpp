#include <tchar.h>
#include <windows.h>

#include "Utils/Utils_WindowsUtils.h"

namespace mm {

	void MaximizeWindow()
	{

//#ifdef _MSC_VER

		SetConsoleTitle(_T("Everything :-)"));
		HWND hWnd = FindWindow(NULL, _T("Everything :-)"));
		ShowWindow(hWnd, SW_SHOWNORMAL);
		SetWindowPos(hWnd, 0, 0, 0, 50, 50, 0); //SWP_NOSIZE -  Ignore cx, cy //SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE
		ShowWindow(hWnd, SW_MAXIMIZE);
		SetForegroundWindow(hWnd); //Show focus on this window
		/*
		CONSOLE_SCREEN_BUFFER_INFO info;
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hOut, &info);
		COORD NewSBSize = GetLargestConsoleWindowSize(hOut);
		SetConsoleScreenBufferSize(hOut, NewSBSize);

		SMALL_RECT DisplayArea = { 0, 0, 0, 0 };
		DisplayArea.Left = DisplayArea.Top = 0;
		DisplayArea.Right = NewSBSize.X - 1;
		DisplayArea.Bottom = NewSBSize.Y - 1;
		SetConsoleWindowInfo(hOut, true, &DisplayArea);
		*/

//#elif __GNUC__

//#endif

	}
}