/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include "b_define.h"
#include "b_core.h"
#include "b_log.h"
/*------------------------------------------------------------------------------------------------*/
#define bMain_LogName L"6y6yka_log.html"
/*------------------------------------------------------------------------------------------------*/
/* WinMain                                                                                        */
/*------------------------------------------------------------------------------------------------*/
bint WINAPI wWinMain(HINSTANCE _hinst, HINSTANCE _hp, bchar *_arg, bint _show)
{
	bLostVariable(_hp);
	bLostVariable(_show);

	bLog_Start(bMain_LogName);
	bLog_Print(L"Programm starting . . .", bLog_Normal);

	bCore_Create(_hinst, _arg);

	bCore_Delete();

	bLog_Print(L"Programm closing. goodbye!", bLog_Normal);
	bLog_Stop();
 
	return 0;
}