/*------------------------------------------------------------------------------------------------*/
/*                                       6y6yka Engine test                                       */
/*------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include <crtdbg.h>
#include "b_config.h"
#include "b_align.c"
#include "b_error.c"
#include "b_log.c"
#include "b_file.c"
#include "b_unicode.c"
#include "b_config.c"
/*------------------------------------------------------------------------------------------------*/
int main()
{
	bint i;
	bfloat f;
	bchar *s;
	bbool b;
	bConfig *cfg;

	cfg = bConfig_Create(L"test.bconfig");

	if(!cfg) return 1;

	/* test 1 */
	if(!bConfig_GetInt(&i, L"Video->Width", cfg))
		return 1;
	if(i != 100)
		return 1;

	/* test 2 */
	if(!bConfig_GetFloat(&f, L"Video->Height", cfg))
		return 1;
	if(f != 200.2f)
		return 1;

	/* test 3 */
	if(!bConfig_GetString(&s, L"Video->Super->Mega->Hello", cfg))
		return 1;
	if(wcscmp(s, L"World") != 0)
		return 1;

	/* test 4 */
	if(!bConfig_GetBool(&b, L"Sound->Music", cfg))
		return 1;
	if(!b)
		return 1;

	/* test 5 */
	if(!bConfig_SetBool(bfalse, L"Sound->Music", cfg))
		return 1;

	/* test 6 */
	if(!bConfig_SetString(L"test string", L"Video->Super->Mega->Hello", cfg))
		return 1;

	if(!bConfig_Save(L"test_out.bconfig", cfg)) return 1;

	bConfig_Delete(L"test_out.bconfig", cfg);
	free(s);

	_ASSERTE(_CrtCheckMemory());
	_ASSERTE(!_CrtDumpMemoryLeaks());

	return 0;
}