/*


#pragma warning (disable:4146)
#import "c:\Program Files (x86)\Common Files\Microsoft Shared\Office10\mso.dll"
#import "c:\Program Files (x86)\Common Files\Microsoft Shared\VBA\VBA6\VBE6EXT.olb"
#import "C:\Program Files (x86)\Microsoft Office\root\Office16\excel.exe" \
	rename("DialogBox", "ExDialogBox") rename("RGB", "ExRGB")

#include <windows.h>
#include <iostream>



int main() {

	CoInitialize(0);

	try {
		Excel::_ApplicationPtr lpApp("Excel.Application");
		Excel::_WorksheetPtr lpWkst = 0;

		lpApp->PutVisible(0, TRUE);

		lpApp->Workbooks->Add();
		lpWkst = lpApp->ActiveSheet;

		lpWkst->Range["A1"]->Value2 = "Forename";
		lpWkst->Range["B1"]->Value2 = "Score";
		lpWkst->Range["A2"]->Value2 = "Dave";
		lpWkst->Range["B2"]->Value2 = 40L;
		lpWkst->Range["A3"]->Value2 = "Andrew";
		lpWkst->Range["B3"]->Value2 = 35L;
		lpWkst->Range["A4"]->Value2 = "Simon";
		lpWkst->Range["B4"]->Value2 = 29L;

		lpWkst = 0;
		lpApp = 0;

	}
	catch (_com_error& e) {

		std::cout << "Error ";
		std::cout << static_cast<char*>
			((e.Description().length() ? e.Description() : ""));
		return 1;
	}

	CoUninitialize();

	return 0;
}



*/