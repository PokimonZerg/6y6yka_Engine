/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#ifndef _B_TYPE_H_
#define _B_TYPE_H_
/*------------------------------------------------------------------------------------------------*/
#include <wchar.h>
/*------------------------------------------------------------------------------------------------*/
/* bool - basic boolean type defination                                                           */
/*------------------------------------------------------------------------------------------------*/
#define bbool  unsigned char
#define btrue  1
#define bfalse 0
/*------------------------------------------------------------------------------------------------*/
/* bchar - basic unicode string type                                                              */
/*------------------------------------------------------------------------------------------------*/
typedef wchar_t bchar;
/*------------------------------------------------------------------------------------------------*/
/* bint - basic signed int type (32 bit)                                                          */
/*------------------------------------------------------------------------------------------------*/
typedef int bint;
/*------------------------------------------------------------------------------------------------*/
/* buint - basic unsigned int type (32 bit)                                                       */
/*------------------------------------------------------------------------------------------------*/
typedef unsigned int buint;
/*------------------------------------------------------------------------------------------------*/
/* bshort - basic short type (16 bit)                                                             */
/*------------------------------------------------------------------------------------------------*/
typedef short bshort;
/*------------------------------------------------------------------------------------------------*/
/* bushort - basic unsigned short type (16 bit)                                                   */
/*------------------------------------------------------------------------------------------------*/
typedef unsigned short bushort;
/*------------------------------------------------------------------------------------------------*/
/* bbyte - basic byte type 0 - 255                                                                */
/*------------------------------------------------------------------------------------------------*/
typedef unsigned char bbyte;
/*------------------------------------------------------------------------------------------------*/
/* bfloat - basic float type                                                                      */
/*------------------------------------------------------------------------------------------------*/
typedef float bfloat;
/*------------------------------------------------------------------------------------------------*/
/* bdouble - basic double type                                                                    */
/*------------------------------------------------------------------------------------------------*/
typedef double bdouble;
/*------------------------------------------------------------------------------------------------*/
/* bvoid - basic void type                                                                        */
/*------------------------------------------------------------------------------------------------*/
typedef void bvoid;
/*------------------------------------------------------------------------------------------------*/
/* blong - basic long integer type (64 bit)                                                       */
/*------------------------------------------------------------------------------------------------*/
typedef __int64 blong;
/*------------------------------------------------------------------------------------------------*/
#endif /* _B_TYPE_H_ */