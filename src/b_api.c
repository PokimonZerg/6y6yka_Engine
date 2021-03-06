/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include "b_core.h"
#include "b_input.h"
#include "b_camera.h"
#include "b_timer.h"
#include "b_api.h"
/*------------------------------------------------------------------------------------------------*/
/* bApi_bCore_Start                                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bCore_Start(bvoid *_stack);
/*------------------------------------------------------------------------------------------------*/
/* bApi_bInput_KeyDown                                                                            */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bInput_KeyDown(bvoid *_stack);
/*------------------------------------------------------------------------------------------------*/
/* bApi_bInput_MouseMoveX                                                                         */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bInput_MouseMoveX(bvoid *_stack);
/*------------------------------------------------------------------------------------------------*/
/* bApi_bInput_MouseMoveY                                                                         */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bInput_MouseMoveY(bvoid *_stack);
/*------------------------------------------------------------------------------------------------*/
/* bApi_bCamera_MoveForward                                                                       */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bCamera_MoveForward(bvoid *_stack);
/*------------------------------------------------------------------------------------------------*/
/* bApi_bCamera_Strafe                                                                            */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bCamera_Strafe(bvoid *_stack);
/*------------------------------------------------------------------------------------------------*/
/* bApi_bCamera_RotateHorizontal                                                                  */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bCamera_RotateHorizontal(bvoid *_stack);
/*------------------------------------------------------------------------------------------------*/
/* bApi_bCamera_RotateVertical                                                                    */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bCamera_RotateVertical(bvoid *_stack);
/*------------------------------------------------------------------------------------------------*/
/* bApi_bCore_GetMainCamera                                                                       */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bCore_GetMainCamera(bvoid *_stack);
/*------------------------------------------------------------------------------------------------*/
/* bApi_bTimer_GetTime                                                                            */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bTimer_GetTime(bvoid *_stack);
/*------------------------------------------------------------------------------------------------*/
/* bApi_Register                                                                                  */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_Register(bLisp_Script *_script)
{
	bLisp_RegisterFunction(_script, L"bCore_Start",              bApi_bCore_Start);
	bLisp_RegisterFunction(_script, L"bInput_KeyDown",           bApi_bInput_KeyDown);
	bLisp_RegisterFunction(_script, L"bInput_MouseMoveX",        bApi_bInput_MouseMoveX);
	bLisp_RegisterFunction(_script, L"bInput_MouseMoveY",        bApi_bInput_MouseMoveY);
	bLisp_RegisterFunction(_script, L"bCamera_MoveForward",      bApi_bCamera_MoveForward);
	bLisp_RegisterFunction(_script, L"bCamera_Strafe",           bApi_bCamera_Strafe);
	bLisp_RegisterFunction(_script, L"bCamera_RotateHorizontal", bApi_bCamera_RotateHorizontal);
	bLisp_RegisterFunction(_script, L"bCamera_RotateVertical",   bApi_bCamera_RotateVertical);
	bLisp_RegisterFunction(_script, L"bCore_GetMainCamera",      bApi_bCore_GetMainCamera);
	bLisp_RegisterFunction(_script, L"bTimer_GetTime",           bApi_bTimer_GetTime);
}
/*------------------------------------------------------------------------------------------------*/
/* bApi_bCore_Start                                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bCore_Start(bvoid *_stack)
{
	bCore_Start();
	bLisp_ReturnNumber(_stack, 0);
}
/*------------------------------------------------------------------------------------------------*/
/* bApi_bInput_KeyDown                                                                            */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bInput_KeyDown(bvoid *_stack)
{
	bLisp_ReturnNumber(_stack, bInput_KeyDown((bbyte)bLisp_GetNumberArg(_stack, 0)));
}
/*------------------------------------------------------------------------------------------------*/
/* bApi_bInput_MouseMoveX                                                                         */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bInput_MouseMoveX(bvoid *_stack)
{
	bLisp_ReturnNumber(_stack, bInput_MouseMoveX());
}
/*------------------------------------------------------------------------------------------------*/
/* bApi_bInput_MouseMoveY                                                                         */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bInput_MouseMoveY(bvoid *_stack)
{
	bLisp_ReturnNumber(_stack, bInput_MouseMoveY());
}

/*------------------------------------------------------------------------------------------------*/
/* bApi_bCamera_MoveForward                                                                       */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bCamera_MoveForward(bvoid *_stack)
{
	bCamera_MoveForward(bLisp_GetNumberArg(_stack, 0),
						bLisp_GetDataArg(_stack, 1));
	bLisp_ReturnNumber(_stack, 0);
}
/*------------------------------------------------------------------------------------------------*/
/* bApi_bCamera_Strafe                                                                            */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bCamera_Strafe(bvoid *_stack)
{
	bCamera_Strafe(bLisp_GetNumberArg(_stack, 0),
				   bLisp_GetDataArg(_stack, 1));
	bLisp_ReturnNumber(_stack, 0);
}
/*------------------------------------------------------------------------------------------------*/
/* bApi_bCamera_RotateHorizontal                                                                  */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bCamera_RotateHorizontal(bvoid *_stack)
{
	bCamera_RotateHorizontal(bLisp_GetNumberArg(_stack, 0),
				             bLisp_GetDataArg(_stack, 1));
	bLisp_ReturnNumber(_stack, 0);
}
/*------------------------------------------------------------------------------------------------*/
/* bApi_bCamera_RotateVertical                                                                    */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bCamera_RotateVertical(bvoid *_stack)
{
	bCamera_RotateVertical(bLisp_GetNumberArg(_stack, 0),
				           bLisp_GetDataArg(_stack, 1));
	bLisp_ReturnNumber(_stack, 0);
}
/*------------------------------------------------------------------------------------------------*/
/* bApi_bCore_GetMainCamera                                                                       */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bCore_GetMainCamera(bvoid *_stack)
{
	bLisp_ReturnData(_stack, bCore_GetMainCamera());
}
/*------------------------------------------------------------------------------------------------*/
/* bApi_bTimer_GetTime                                                                            */
/*------------------------------------------------------------------------------------------------*/
bvoid bApi_bTimer_GetTime(bvoid *_stack)
{
	bfloat t = (bfloat)bTimer_GetTime();

	//if(t == 0)
	//{
	//	t = 0.001f;
	//}

	bLisp_ReturnNumber(_stack, t);
}