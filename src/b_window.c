/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include "b_log.h"
#include "b_define.h"
#include "b_window.h"
/*------------------------------------------------------------------------------------------------*/
/* bWindow_ClassName                                                                              */
/*------------------------------------------------------------------------------------------------*/
#define bWindow_ClassName L"6y6yka Engine"
/*------------------------------------------------------------------------------------------------*/
/* bWindow_BuildClass                                                                             */
/*------------------------------------------------------------------------------------------------*/
bvoid bWindow_BuildClass(WNDCLASS *_w, WNDPROC _handler);
/*------------------------------------------------------------------------------------------------*/
/* bWindow_Create                                                                                 */
/*------------------------------------------------------------------------------------------------*/
HWND bWindow_Create(WNDPROC _handler, bSettings *_settings)
{
	WNDCLASS win_class;
	HWND     hwnd;

	bWindow_BuildClass(&win_class, _handler);

	if(!RegisterClass(&win_class))
	{
		bLog_Print(L"Problems while registering window class", bLog_Warning);
		return 0;
	}

	hwnd = CreateWindow(bWindow_ClassName, bCaption, WS_OVERLAPPEDWINDOW, 0, 0, 
		_settings->video.window_width, _settings->video.window_height, 0, 0, GetModuleHandle(0), 0);

	if(!hwnd)
	{
		bLog_Print(L"Problems while calling 'CreateWindow' procedure", bLog_Warning);
		return 0;
	}

	return hwnd;
}
/*------------------------------------------------------------------------------------------------*/
/* bWindow_Delete                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bvoid bWindow_Delete(HWND _hwnd)
{
	if(!DestroyWindow(_hwnd))
	{
		bLog_Print(L"Problems while calling 'DestroyWindow'", bLog_Warning);
		return;
	}

	if(!UnregisterClass(bWindow_ClassName, GetModuleHandle(0)))
	{
		bLog_Print(L"Problems while unregistering window class", bLog_Warning);
		return;
	}
}
/*------------------------------------------------------------------------------------------------*/
/* bWindow_BuildClass                                                                             */
/*------------------------------------------------------------------------------------------------*/
bvoid bWindow_BuildClass(WNDCLASS *_w, WNDPROC _handler)
{
    _w->cbClsExtra    = 0;
	_w->cbWndExtra    = 0;
	_w->hbrBackground = (HBRUSH)COLOR_GRAYTEXT;   /* gray color   */
	_w->hCursor       = LoadCursor(0, IDC_ARROW); /* usual cursor */
	_w->hIcon         = LoadIcon(0, IDI_WARNING); /* warning icon */
	_w->hInstance     = GetModuleHandle(0);
	_w->lpfnWndProc   = _handler;
	_w->lpszClassName = bWindow_ClassName;
	_w->lpszMenuName  = 0;
	_w->style         = CS_HREDRAW | CS_VREDRAW;
}