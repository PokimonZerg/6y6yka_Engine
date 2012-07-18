/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include "b_assert.h"
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
bvoid bLog_Start(bchar *_name)
{
	bAssert(_name);      /* check name string */
	bAssert(!bLog_File); /* check log file    */

	InitializeCriticalSection(&bLog_CriticalSection);

	bLog_File = bFile_Create(_name, btrue, bfalse, btrue);

	bFile_Write(bUnicode_Utf16_Bom, bUnicode_Utf16_Bom_Size, bLog_File);
	bFile_Write(bLog_Top, sizeof(bLog_Top) - sizeof(bchar), bLog_File);
};
/*------------------------------------------------------------------------------------------------*/
/* bLog_Stop                                                                                      */
/*------------------------------------------------------------------------------------------------*/
bvoid bLog_Stop()
{
	bAssert(bLog_File); /* check log file    */

	bFile_Write(bLog_Down, sizeof(bLog_Down), bLog_File);
	bFile_Close(bLog_File);

	DeleteCriticalSection(&bLog_CriticalSection);
}
/*------------------------------------------------------------------------------------------------*/
/* bLog_Print                                                                                     */
/*------------------------------------------------------------------------------------------------*/
bvoid bLog_Print(bchar *_text, bLog_Level _level)
{
	bchar      message[512];
	bchar      *color;
	buint      size;
	SYSTEMTIME t;

	bAssert(_text); /* check text string */
	bAssert(_level >= bLog_Normal && _level <= bLog_Error);

	color = _level == bLog_Normal  ? bLog_Green  :
			_level == bLog_Warning ? bLog_Yellow : bLog_Red;

	GetLocalTime(&t);

	size = swprintf(message, 512, L"<tr bgcolor=\"%s\"><td width=25%% align=\
					\"center\">%02d.%02d.%04d %02d:%02d:%02d</td><td  width=\
					75%%>%s</td></tr>", color, t.wDay, t.wMonth, t.wYear,
					t.wHour, t.wMinute, t.wSecond, _text);

	//EnterCriticalSection(&bLog_CriticalSection);

	if(bLog_File)
		bFile_Write(message, size * sizeof(bchar), bLog_File);

	//LeaveCriticalSection(&bLog_CriticalSection);
}