/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include <stdlib.h>
#include "b_assert.h"
#include "b_error.h"
#include "b_log.h"
#include "b_unicode.h"
#include "b_file.h"
/*------------------------------------------------------------------------------------------------*/
/* bFile - file representation structure.                                                         */
/*------------------------------------------------------------------------------------------------*/
struct bFile
{
	HANDLE handle; /* win32 file handle */
};
/*------------------------------------------------------------------------------------------------*/
/* bFile_Pos - file cursir position.                                                              */
/*------------------------------------------------------------------------------------------------*/
enum bFile_Pos
{
	bFile_Begin,
	bFile_Current,
	bFile_End
};
/*------------------------------------------------------------------------------------------------*/
DWORD bFile_CalcRWMode(bbool _read, bbool _write);
/*------------------------------------------------------------------------------------------------*/
bUnicode_Type bFile_CheckBOM(bbyte *_header);
/*------------------------------------------------------------------------------------------------*/
/* bFile_CalcRWMode                                                                               */
/*------------------------------------------------------------------------------------------------*/
DWORD bFile_CalcRWMode(bbool _read, bbool _write)
{
	DWORD rw_mode = 0;

	rw_mode |= _read  ? GENERIC_READ  : 0;
	rw_mode |= _write ? GENERIC_WRITE : 0;

	return rw_mode;
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_Open                                                                                     */
/*------------------------------------------------------------------------------------------------*/
bFile *bFile_Open(bchar *_name, bbool _read, bbool _write)
{
	bFile *file;

	bAssert(_name);           /* check name ptr */
	bAssert(_read || _write); /* check r/w mode */

	file = malloc(sizeof(bFile));

	if(!file)                 /* check memory alloc */
		bError(bError_Info, L"Memory allocation error");

	file->handle = CreateFile(_name, bFile_CalcRWMode(_read, _write), 0, 0, 
		                      OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if(file->handle == INVALID_HANDLE_VALUE) /* check file handle */
		bError(bError_Info, L"Can't open file %s.", _name);

	return  file;
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_Create                                                                                   */
/*------------------------------------------------------------------------------------------------*/
bFile *bFile_Create(bchar *_name, bbool _override, bbool _read, bbool _write)
{
	bFile *file;

	bAssert(_name);           /* check name ptr */
	bAssert(_read || _write); /* check r/w mode */

	file = malloc(sizeof(bFile));

	if(!file)                 /* check memory alloc */
		bError(bError_Info, L"Memory allocation error");

	file->handle = CreateFile(_name, bFile_CalcRWMode(_read, _write), 0, 0, 
		                      _override ? CREATE_ALWAYS : CREATE_NEW,
		                      FILE_ATTRIBUTE_NORMAL, 0);

	if(file->handle == INVALID_HANDLE_VALUE) /* check file handle */
		bError(bError_Info, L"Can't create file %s.", _name);

	return file;
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_Close                                                                                    */
/*------------------------------------------------------------------------------------------------*/
bvoid bFile_Close(bFile *_file)
{
	if(!CloseHandle(_file->handle))
	{
		bLog_Print(L"Error while closing file", bLog_Warning);
	}

	free(_file);
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_Read                                                                                     */
/*------------------------------------------------------------------------------------------------*/
buint bFile_Read(bvoid *_buf, buint _size, bFile *_file)
{
	buint read_size;

	if(!ReadFile(_file->handle, _buf, _size, (unsigned long *)&read_size, 0))
		bError(bError_Info, L"Reading file error.");

	return read_size;
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_SetPos                                                                                   */
/*------------------------------------------------------------------------------------------------*/
bvoid bFile_SetPos(buint _offset, bFile_Pos _origin, bFile *_file)
{
	SetFilePointer(_file->handle, _offset, 0, _origin == bFile_Begin   ? FILE_BEGIN   : 
		                                      _origin == bFile_Current ? FILE_CURRENT : FILE_END);
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
	bFile        *file;
	bbyte         header[4] = {255, 255, 255, 255};

	file = bFile_Open(_name, btrue, bfalse);

	bFile_Read(header, sizeof(header), file);

	utype = bFile_CheckBOM(header);

	if(utype == bUnicode_Utf32)
	{
		bFile_Close(file);
		bError(bError_Info, L"UTF32 is unsupported unicode format");
	}

	if(utype == bUnicode_Utf16)
	{
		bFile_SetPos(bUnicode_Utf16_Bom_Size - sizeof(header), bFile_Current, file);

		size = bFile_Size(file) - bUnicode_Utf16_Bom_Size + sizeof(L'\0');

		text = malloc(size);

		bFile_Read(text, size, file);

		text[(size - sizeof(L'\0')) / 2] = '\0';
	}

	if(utype == bUnicode_Utf8)
	{
		bFile_SetPos(bUnicode_Utf8_Bom_Size - sizeof(header), bFile_Current, file);

		size = bFile_Size(file) - bUnicode_Utf8_Bom_Size;

		buffer = malloc(size + 1); /* for null */

		bFile_Read(buffer, size, file);

		buffer[size] = '\0';

		size = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)buffer, -1, 0, 0) * sizeof(bchar);

		text = malloc(size);

		size = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)buffer, -1, text, size);

		free(buffer);
	}

	if(utype == bUnicode_Ansi)
	{
		bFile_SetPos(0, bFile_Begin, file);

		size = bFile_Size(file);

		buffer = malloc(size + 1); /* for null */

		bFile_Read(buffer, size, file);

		buffer[size] = '\0';

		size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)buffer, -1, 0, 0);

		text = malloc(size *= sizeof(bchar));

		size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)buffer, -1, text, size);

		free(buffer);
	}

	bFile_Close(file);

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
buint bFile_Write(bvoid *_buf, buint _size, bFile *_file)
{
	buint write_size;

	if(!WriteFile(_file->handle, _buf, _size, (unsigned long *)&write_size, 0))
		bError(bError_Info, L"Writing file error.");

	return write_size;
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_Size                                                                                     */
/*------------------------------------------------------------------------------------------------*/
buint bFile_Size(bFile *_file)
{
	LARGE_INTEGER file_size;

	if(!GetFileSizeEx(_file->handle, &file_size))
		bError(bError_Info, L"Unknown file size error.");

	return (buint)file_size.QuadPart;
}