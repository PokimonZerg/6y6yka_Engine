/*----------------------------------------------------------------------------*/
/*                              6y6yka Engine                                 */
/*----------------------------------------------------------------------------*/
#ifndef _B_ERROR_H_
#define _B_ERROR_H_
/*----------------------------------------------------------------------------*/
#include "b_type.h"
#include "b_define.h"
/*----------------------------------------------------------------------------*/
bvoid bError(bchar *_info, bchar *_text, ...);
/*----------------------------------------------------------------------------*/
#define bError_Info L"Line: " ToWStr(ToStr(__LINE__)) L"\r\n" L" File: " \
	                ToWStr(__FILE__) L"\r\n" L" Func: " ToWStr(__FUNCTION__)
/*----------------------------------------------------------------------------*/
#endif /* _B_ERROR_H_ */