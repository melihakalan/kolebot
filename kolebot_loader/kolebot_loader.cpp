//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include "_frmLoader.h"
//---------------------------------------------------------------------------
USEFORM("_frmLoader.cpp", frmLoader);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();

		EditDbgBreakPoint();

		if( !CheckCommandLine() )
		{
			Application->Terminate();
			return 0;
		}
		else
		{
			Application->MainFormOnTaskBar = true;
			Application->Title = "";
			Application->CreateForm(__classid(TfrmLoader), &frmLoader);
			Application->Run();
		}
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
