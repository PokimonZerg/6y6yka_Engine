/*----------------------------------------------------------------------------*/
/*                              6y6yka Engine                                 */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <float.h>
#include "b_core.h"
#include "b_log.h"
#include "b_file.h"
#include "b_lisp.h"
/*----------------------------------------------------------------------------*/
bint WINAPI wWinMain(HINSTANCE _hinst, HINSTANCE _hp, bchar *_arg, bint _show)
{
	bLisp_Script *script;

	bLog_Start(L"test.html");

	bLog_Print(L"Programm starting . . .", bLog_Normal);

	script = bLisp_OpenScript(L"(+ 2 3)");
	bLisp_Run(script, 0);

	bLog_Print(L"Programm closing. goodbye!", bLog_Normal);

	bLog_Stop();
 
	return 0;
}