/*----------------------------------------------------------------------------*/
/*                              6y6yka Engine                                 */
/*----------------------------------------------------------------------------*/
#ifndef _B_LISP_H_
#define _B_LISP_H_
/*----------------------------------------------------------------------------*/
#include "b_type.h"
/*----------------------------------------------------------------------------*/
typedef struct bLisp_Script bLisp_Script;
typedef struct bLisp_Stack  bLisp_Stack;
typedef struct bLisp_Type   bLisp_Type;
/*----------------------------------------------------------------------------*/
typedef bLisp_Type *(* bLisp_CallBack)(bLisp_Stack *_stack);
/*----------------------------------------------------------------------------*/
bLisp_Script *bLisp_OpenScript(bchar *_text);
/*----------------------------------------------------------------------------*/
bLisp_Script *bLisp_OpenCode(bchar *_filename);
/*----------------------------------------------------------------------------*/
bbool bLisp_Close(bLisp_Script *_script);
/*----------------------------------------------------------------------------*/
bbool bLisp_Save(bchar *_filename, bLisp_Script *_script);
/*----------------------------------------------------------------------------*/
bbool bLisp_Run(bLisp_Script *_script, bchar **_result);
/*----------------------------------------------------------------------------*/
#endif /* _B_LISP_H_ */