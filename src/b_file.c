/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include <stdlib.h>
#include <assert.h>
#include "b_error.h"
#include "b_log.h"
#include "b_unicode.h"
#include "b_file.h"
/*------------------------------------------------------------------------------------------------*/
/* bFile_CalcMode                                                                                 */
/*------------------------------------------------------------------------------------------------*/
DWORD bFile_CalcMode(bFile_Mode _mode);
/*------------------------------------------------------------------------------------------------*/
/* bFile_CheckBOM                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bUnicode_Type bFile_CheckBOM(bbyte *_header);
/*------------------------------------------------------------------------------------------------*/
/* bFile_CalcMode                                                                                 */
/*------------------------------------------------------------------------------------------------*/
DWORD bFile_CalcMode(bFile_Mode _mode)
{
	if(_mode == bFile_ReadMode)  return GENERIC_READ;
	if(_mode == bFile_WriteMode) return GENERIC_WRITE;
	
	return GENERIC_READ | GENERIC_WRITE;
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_Open                                                                                     */
/*------------------------------------------------------------------------------------------------*/
bFile bFile_Open(bchar *_name, bFile_Mode _mode)
{
	bFile file;

	assert(_name); /* check name ptr */

	file = CreateFile(_name, bFile_CalcMode(_mode), 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if(file == INVALID_HANDLE_VALUE)
	{
		bLog_Print(L"Can't open file", bLog_Warning);
		return 0;
	}

	return file;
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_Create                                                                                   */
/*------------------------------------------------------------------------------------------------*/
bFile bFile_Create(bchar *_name, bbool _override, bFile_Mode _mode)
{
	bFile file;

	assert(_name);           /* check name ptr */

	file = CreateFile(_name, bFile_CalcMode(_mode), 0, 0, _override ? CREATE_ALWAYS : CREATE_NEW,
		              FILE_ATTRIBUTE_NORMAL, 0);

	if(file == INVALID_HANDLE_VALUE)
	{
		bLog_Print(L"Can't create file", bLog_Warning);
		return 0;
	}

	return file;
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_Close                                                                                    */
/*------------------------------------------------------------------------------------------------*/
bbool bFile_Close(bFile _file)
{
	if(!CloseHandle(_file))
	{
		bLog_Print(L"Error while closing file", bLog_Warning);
		return bfalse;
	}

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_Read                                                                                     */
/*------------------------------------------------------------------------------------------------*/
bbool bFile_Read(bvoid *_buf, buint _size, buint *_rsize, bFile _file)
{
	return ReadFile(_file, _buf, _size, (unsigned long *)_rsize, 0) != 0;
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_SetPos                                                                                   */
/*------------------------------------------------------------------------------------------------*/
bbool bFile_SetPos(buint _offset, bFile_Pos _origin, bFile _file)
{
	SetFilePointer(_file, _offset, 0, _origin == bFile_Begin   ? FILE_BEGIN   : 
		                              _origin == bFile_Current ? FILE_CURRENT : FILE_END);
	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_WriteText                                                                                */
/*------------------------------------------------------------------------------------------------*/
bbool bFile_WriteText(bchar *_name, bchar *_text)
{
	bFile file;

	assert(_name);
	assert(_text);

	file = bFile_Create(_name, btrue, bFile_WriteMode);

	if(!file) return bfalse;

	bFile_Write(bUnicode_Utf16_Bom, bUnicode_Utf16_Bom_Size, 0, file);
	bFile_Write(_text, wcslen(_text) * sizeof(bchar), 0, file);

	bFile_Close(file);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_ReadText                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bchar *bFile_ReadText(bchar *_name)
{
	bchar        *text;
	bbyte        *buffer;
	buint         size;
	bUnicode_Type utype;
	bFile         file;
	bbyte         header[4] = {255, 255, 255, 255};

	assert(_name);

	file = bFile_Open(_name, bFile_ReadMode);

	if(!file) return 0;

	if(!bFile_Read(header, sizeof(header), 0, file)) return 0;

	utype = bFile_CheckBOM(header);

	if(utype == bUnicode_Utf32)
	{
		CloseHandle(file);
		bLog_Print(L"UTF32 is unsupported unicode format", bLog_Warning);
		return 0;
	}

	if(utype == bUnicode_Utf16)
	{
		bFile_SetPos(bUnicode_Utf16_Bom_Size - sizeof(header), bFile_Current, file);

		size = bFile_GetSize(file) - bUnicode_Utf16_Bom_Size + sizeof(L'\0');

		text = malloc(size);

		bFile_Read(text, size, 0, file);

		text[(size - sizeof(L'\0')) / 2] = '\0';
	}

	if(utype == bUnicode_Utf8)
	{
		bFile_SetPos(bUnicode_Utf8_Bom_Size - sizeof(header), bFile_Current, file);

		size = bFile_GetSize(file) - bUnicode_Utf8_Bom_Size;

		buffer = malloc(size + 1); /* for null */

		bFile_Read(buffer, size, 0, file);

		buffer[size] = '\0';

		size = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)buffer, -1, 0, 0) * sizeof(bchar);

		text = malloc(size);

		size = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)buffer, -1, text, size);

		free(buffer);
	}

	if(utype == bUnicode_Ansi)
	{
		bFile_SetPos(0, bFile_Begin, file);

		size = bFile_GetSize(file);

		buffer = malloc(size + 1); /* for null */

		bFile_Read(buffer, size, 0, file);

		buffer[size] = '\0';

		size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)buffer, -1, 0, 0);

		text = malloc(size *= sizeof(bchar));

		size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)buffer, -1, text, size);

		free(buffer);
	}

	CloseHandle(file);

	return text;
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_CheckBOM                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bUnicode_Type bFile_CheckBOM(bbyte *_header)
{
	if(memcmp(_header, bUnicode_Utf32_Bom, bUnicode_Utf32_Bom_Size) == 0)
		return bUnicode_Utf32;

	if(memcmp(_header, bUnicode_Utf16_Bom, bUnicode_Utf16_Bom_Size) == 0)
		return bUnicode_Utf16;

	if(memcmp(_header, bUnicode_Utf8_Bom, bUnicode_Utf8_Bom_Size) == 0)
		return bUnicode_Utf8;

	return bUnicode_Ansi;
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_Write                                                                                    */
/*------------------------------------------------------------------------------------------------*/
bbool bFile_Write(bvoid *_buf, buint _size, buint *_wsize, bFile _file)
{
	return WriteFile(_file, _buf, _size, (unsigned long *)_wsize, 0) != 0;
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_Size                                                                                     */
/*------------------------------------------------------------------------------------------------*/
buint bFile_GetSize(bFile _file)
{
	return GetFileSize(_file, 0);
}