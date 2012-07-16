/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include <float.h>
#include "b_core.h"
#include "b_log.h"
#include "b_file.h"
#include "b_error.h"
#include "b_lisp.h"
/*------------------------------------------------------------------------------------------------*/
bint WINAPI wWinMain(HINSTANCE _hinst, HINSTANCE _hp, bchar *_arg, bint _show)
{
	bchar *result;
	bLisp_Script *script;
	
	UNREFERENCED_PARAMETER(_hinst);
	UNREFERENCED_PARAMETER(_hp);
	UNREFERENCED_PARAMETER(_arg);
	UNREFERENCED_PARAMETER(_show);

	bLog_Start(L"test.html");
	bError(bError_Info, L"text");

	bLog_Print(L"Programm starting . . .", bLog_Normal);

	script = bLisp_OpenScript(L"(+ 2 3)", 0);
	bLisp_Run(script, &result, 0);


	bLog_Print(L"Programm closing. goodbye!", bLog_Normal);

	bLog_Stop();
 
	return 0;
}