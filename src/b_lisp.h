/*------------------------------------------------------------------------------------------------*/
/*                              6y6yka Engine                                                     */
/*------------------------------------------------------------------------------------------------*/
#ifndef _B_LISP_H_
#define _B_LISP_H_
/*------------------------------------------------------------------------------------------------*/
#include "b_type.h"
/*------------------------------------------------------------------------------------------------*/
typedef struct bLisp_Script bLisp_Script;
/*------------------------------------------------------------------------------------------------*/
typedef void (* bLisp_CallBack)(void *_from, void *_to);
/*------------------------------------------------------------------------------------------------*/
bLisp_Script *bLisp_OpenScript(bchar *_text);
/*------------------------------------------------------------------------------------------------*/
bLisp_Script *bLisp_OpenCode(bchar *_filename);
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_Close(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_Save(bchar *_filename, bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_Run(bLisp_Script *_script, bchar **_result);
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_RegisterFunction(bLisp_Script *_script, bchar *_name, bLisp_CallBack _ptr, buint _arg);
/*------------------------------------------------------------------------------------------------*/
bint   bLisp_GetIntArg    (bvoid *_from, buint _arg);
bfloat bLisp_GetFloatArg  (bvoid *_from, buint _arg);
bchar *bLisp_GetStringArg (bvoid *_from, buint _arg);
bvoid *bLisp_GetDataArg   (bvoid *_from, buint _arg);
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_ReturnInt    (bvoid *_to, bint   _ret);
bvoid bLisp_ReturnFloat  (bvoid *_to, bfloat _ret);
bvoid bLisp_ReturnString (bvoid *_to, bchar *_ret);
bvoid bLisp_ReturnData   (bvoid *_to, bvoid *_ret);
/*------------------------------------------------------------------------------------------------*/
#endif /* _B_LISP_H_ */