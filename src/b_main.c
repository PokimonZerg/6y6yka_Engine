/*----------------------------------------------------------------------------*/
/*                              6y6yka Engine                                 */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include <float.h>
#include "b_core.h"
#include "b_log.h"
#include "b_file.h"
/*----------------------------------------------------------------------------*/
bint WINAPI wWinMain(HINSTANCE _hinst, HINSTANCE _hp, bchar *_arg, bint _show)
{
	bLog_Start(L"test.html");

	bLog_Print(L"Programm starting . . .", bLog_Normal);

	bLog_Print(L"Programm closing. goodbye!", bLog_Normal);

	bLog_Stop();
 
	return 0;
}