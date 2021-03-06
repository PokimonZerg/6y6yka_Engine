/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include "b_file.h"
#include "b_define.h"
#include "b_unicode.h"
#include "b_log.h"
/*------------------------------------------------------------------------------------------------*/
/* bLog variables                                                                                 */
/*------------------------------------------------------------------------------------------------*/
CRITICAL_SECTION bLog_CriticalSection;
bFile           *bLog_File = 0;
/*------------------------------------------------------------------------------------------------*/
/* HTML defines                                                                                   */
/*------------------------------------------------------------------------------------------------*/
#define bLog_Top L"<html><head><title>6y6yka Engine</title></head><body> \
<font size=4 color=\"Black\" face=\"Courier New\"><center><H2>6y6yka Engine log\
</H2><table border=1 bordercolor=\"Black\" width=800 cellspacing=0>"
#define bLog_Down   L"</table></center></font></body></html>"
#define bLog_Green  L"LightGreen"
#define bLog_Yellow L"Yellow"
#define bLog_Red    L"Red"
/*------------------------------------------------------------------------------------------------*/
/* bLog_Start                                                                                     */
/*------------------------------------------------------------------------------------------------*/
bbool bLog_Start(bchar *_name)
{
	assert(_name);      /* check name string */
	assert(!bLog_File); /* check log file    */

	InitializeCriticalSection(&bLog_CriticalSection);

	bLog_File = bFile_Create(_name, btrue, bFile_WriteMode);

	if(!bLog_File) return bfalse;

	bFile_Write(bUnicode_Utf16_Bom, bUnicode_Utf16_Bom_Size, 0, bLog_File);
	bFile_Write(bLog_Top, sizeof(bLog_Top) - sizeof(bchar), 0, bLog_File);

	return btrue;
};
/*------------------------------------------------------------------------------------------------*/
/* bLog_Stop                                                                                      */
/*------------------------------------------------------------------------------------------------*/
bbool bLog_Stop()
{
	if(!bLog_File) return bfalse;

	bFile_Write(bLog_Down, sizeof(bLog_Down), 0, bLog_File);
	bFile_Close(bLog_File);

	DeleteCriticalSection(&bLog_CriticalSection);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLog_Print                                                                                     */
/*------------------------------------------------------------------------------------------------*/
bbool bLog_Print(bchar *_text, bLog_Level _level)
{
	bchar      message[512];
	bchar      *color;
	buint      size;
	SYSTEMTIME t;

	assert(_text); /* check text string */
	assert(_level >= bLog_Normal && _level <= bLog_Error);

	color = _level == bLog_Normal  ? bLog_Green  :
			_level == bLog_Warning ? bLog_Yellow : bLog_Red;

	GetLocalTime(&t);

	size = swprintf(message, 512, L"<tr bgcolor=\"%s\"><td width=25%% align=\
					\"center\">%02d.%02d.%04d %02d:%02d:%02d</td><td  width=\
					75%%>%s</td></tr>", color, t.wDay, t.wMonth, t.wYear,
					t.wHour, t.wMinute, t.wSecond, _text);

	//EnterCriticalSection(&bLog_CriticalSection);

	if(!bLog_File) return bfalse;

	bFile_Write(message, size * sizeof(bchar), 0, bLog_File);

	return btrue;

	//LeaveCriticalSection(&bLog_CriticalSection);
}