/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include <stdlib.h>
#include "b_assert.h"
#include "b_error.h"
#include "b_file.h"
/*------------------------------------------------------------------------------------------------*/
/* bFile - file representation structure                                                          */
/*------------------------------------------------------------------------------------------------*/
struct bFile
{
	HANDLE handle; /* win32 file handle */
};
/*------------------------------------------------------------------------------------------------*/
DWORD bFile_CalcRWMode(bbool _read, bbool _write);
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
/* bFile_Create                                                               */
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

	return  file;
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_Close                                                                */
/*------------------------------------------------------------------------------------------------*/
bvoid bFile_Close(bFile *_file)
{
	if(!CloseHandle(_file->handle))
	{
		/* warning */
	}
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_Read                                                                 */
/*------------------------------------------------------------------------------------------------*/
buint bFile_Read(bvoid *_buf, buint _size, bFile *_file)
{
	buint read_size;

	if(!ReadFile(_file->handle, _buf, _size, (unsigned long *)&read_size, 0))
		bError(bError_Info, L"Reading file error.");

	return read_size;
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_Write                                                                */
/*------------------------------------------------------------------------------------------------*/
buint bFile_Write(bvoid *_buf, buint _size, bFile *_file)
{
	buint write_size;

	if(!WriteFile(_file->handle, _buf, _size, (unsigned long *)&write_size, 0))
		bError(bError_Info, L"Writing file error.");

	return write_size;
}
/*------------------------------------------------------------------------------------------------*/
/* bFile_Size                                                                 */
/*------------------------------------------------------------------------------------------------*/
buint bFile_Size(bFile *_file)
{
	LARGE_INTEGER file_size;

	if(!GetFileSizeEx(_file->handle, &file_size))
		bError(bError_Info, L"Unknown file size error.");

	return (buint)file_size.QuadPart;
}