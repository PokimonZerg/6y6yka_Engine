/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include "b_input.h"
/*------------------------------------------------------------------------------------------------*/
bbyte bInput_Keys[256];
/*------------------------------------------------------------------------------------------------*/
POINT bInput_MousePos;
POINT bInput_Center;
/*------------------------------------------------------------------------------------------------*/
bvoid bInput_Refresh()
{
	RECT screen;

	GetCursorPos(&bInput_MousePos);

	GetWindowRect(GetDesktopWindow() ,&screen);

	bInput_Center.x = screen.right  / 2;
	bInput_Center.y = screen.bottom / 2;

	SetCursorPos(bInput_Center.x, bInput_Center.y);
}
/*------------------------------------------------------------------------------------------------*/
bbool bInput_KeyDown(bbyte _key)
{
	return bInput_Keys[_key];
}
/*------------------------------------------------------------------------------------------------*/
bfloat bInput_MouseMoveX()
{
	return (bfloat)(bInput_MousePos.x - bInput_Center.x);
}
/*------------------------------------------------------------------------------------------------*/
bfloat bInput_MouseMoveY()
{
	return (bfloat)(bInput_MousePos.y - bInput_Center.y);
}
/*------------------------------------------------------------------------------------------------*/
bvoid bInput_PressIt(bbool _press, bbyte _key)
{
	bInput_Keys[_key] = _press;
}