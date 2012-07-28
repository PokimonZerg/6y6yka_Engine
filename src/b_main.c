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
#include "b_config.h"
/*------------------------------------------------------------------------------------------------*/
bint WINAPI wWinMain(HINSTANCE _hinst, HINSTANCE _hp, bchar *_arg, bint _show)
{
	bConfig *cfg;
	int d;

	UNREFERENCED_PARAMETER(_hinst);
	UNREFERENCED_PARAMETER(_hp);
	UNREFERENCED_PARAMETER(_arg);
	UNREFERENCED_PARAMETER(_show);

	cfg = bConfig_Create(L"options.bconfig");

	bConfig_GetInt(&d, L"Video->Height", cfg);
	bConfig_SetInt(10, L"Video->Height", cfg);
	bConfig_SetInt(15, L"Video->Heigh", cfg);
	
	bConfig_Save(L"opt.bconfig", cfg);


	bLog_Start(L"test.html");

	bLog_Print(L"Programm starting . . .", bLog_Normal);

	bCore_Create();

	bCore_Delete();

	bLog_Print(L"Programm closing. goodbye!", bLog_Normal);

	bLog_Stop();
 
	return 0;
}