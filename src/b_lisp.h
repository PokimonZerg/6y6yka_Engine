/*------------------------------------------------------------------------------------------------*/
/*                              6y6yka Engine                                                     */
/*------------------------------------------------------------------------------------------------*/
#ifndef _B_LISP_H_
#define _B_LISP_H_
/*------------------------------------------------------------------------------------------------*/
#include "b_type.h"
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Script                                                                                   */
/*------------------------------------------------------------------------------------------------*/
typedef struct bLisp_Script bLisp_Script;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_CallBack - wrapper template                                                              */
/*------------------------------------------------------------------------------------------------*/
typedef void (* bLisp_CallBack)(void *_stack);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_OpenScript - open and compile script.                                                    */
/*                                                                                                */
/* arguments:                                                                                     */
/*     _text  - pointer to script source code.                                                    */
/*     _error - pointer to pointer to string for error reporting. can be null.                    */
/* return:                                                                                        */
/*     new script structure or 0.                                                                 */
/*------------------------------------------------------------------------------------------------*/
bLisp_Script *bLisp_OpenScript(bchar *_text, bchar **_error);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_OpenCode - red file with lisp bytecode.                                                  */
/*                                                                                                */
/* arguments:                                                                                     */
/*     _filename - yes! it is filename.                                                           */
/* return:                                                                                        */
/*     new script structure or 0.                                                                 */
/*------------------------------------------------------------------------------------------------*/
bLisp_Script *bLisp_OpenCode(bchar *_filename);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Close - close and free script structure.                                                 */
/*                                                                                                */
/* arguments:                                                                                     */
/*     _script - pointer to valid script structure.                                               */
/* return:                                                                                        */
/*     usually returns btrue.                                                                     */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_Close(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Save - save script bytecode in file.                                                     */
/*                                                                                                */
/* arguments:                                                                                     */
/*     _filename - typically filename.                                                            */
/*     _script   - pointer to valid script structure.                                             */
/* return:                                                                                        */
/*     usually returns btrue.                                                                     */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_Save(bchar *_filename, bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Run - execute script.                                                                    */
/*                                                                                                */
/* arguments:                                                                                     */
/*     _script - pointer to valid script.                                                         */
/*     _result - pointer to pointer to result string. must be delete after use. can be null.      */
/*     _error  - pointer to pointer to error string. can be null.                                 */
/* return:                                                                                        */
/*     btrue if execute success and bfalse in over situations.                                    */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_Run(bLisp_Script *_script, bchar **_result, bchar **_error);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_RegisterFunction - register callback function.                                           */
/*                                                                                                */
/* arguments:                                                                                     */
/*     _script - pointer to valid script.                                                         */
/*     _name   - function name. must be declare in native section in script.                      */
/*     _ptr    - callback ptr.                                                                    */
/* return:                                                                                        */
/*     btrue if function was registered.                                                          */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_RegisterFunction(bLisp_Script *_script, bchar *_name, bLisp_CallBack _ptr);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_GetArg - get argument from stack. check his type and other stuff.                        */
/*                                                                                                */
/* arguments:                                                                                     */
/*     _stack  - stack pointer.                                                                   */
/*     _arg    - argument position.                                                               */
/* return:                                                                                        */
/*     argument value.                                                                            */
/*------------------------------------------------------------------------------------------------*/
bfloat bLisp_GetNumberArg (bvoid *_stack, buint _arg);
/*------------------------------------------------------------------------------------------------*/
bchar *bLisp_GetStringArg(bvoid *_stack, buint _arg);
/*------------------------------------------------------------------------------------------------*/
bvoid *bLisp_GetDataArg(bvoid *_stack, buint _arg);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Return - put result value on stack.                                                             */
/*                                                                                                */
/* arguments:                                                                                     */
/*     _stack  - stack pointer.                                                                   */
/*     _ret    - result value.                                                                    */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_ReturnNumber(bvoid *_stack, bfloat _ret);
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_ReturnString(bvoid *_stack, bchar *_ret);
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_ReturnData(bvoid *_stack, bvoid *_ret);
/*------------------------------------------------------------------------------------------------*/
#endif /* _B_LISP_H_ */