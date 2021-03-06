/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include <stdlib.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <process.h>
#include "b_api.h"
#include "b_define.h"
#include "b_error.h"
#include "b_log.h"
#include "b_window.h"
#include "b_camera.h"
#include "b_config.h"
#include "b_render.h"
#include "b_lisp.h"
#include "b_world.h"
#include "b_patch.h"
#include "b_input.h"
#include "b_timer.h"
#include "b_core.h"
/*------------------------------------------------------------------------------------------------*/
#define bCore_DefaultScript L"#include \"main.blisp\""
#define bCore_DefaultConfig L"options.bconfig"
/*------------------------------------------------------------------------------------------------*/
/* bCore_AppInfo                                                                                  */
/*------------------------------------------------------------------------------------------------*/
typedef struct
{
	HINSTANCE  hinstance;
	HWND       hwnd;
	HDC        hdc;
	HGLRC      hglrc;

	bSettings *settings;

	HANDLE     main_event;
	HANDLE     render_event;
}
bCore_AppInfo;
/*------------------------------------------------------------------------------------------------*/
/* bCore_RenderThread                                                                             */
/*------------------------------------------------------------------------------------------------*/
void bCore_RenderThread(void *_arg);
/*------------------------------------------------------------------------------------------------*/
/* bCore_MessageMonster                                                                           */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_MessageMonster();
/*------------------------------------------------------------------------------------------------*/
/* bCore_InitOpenGL                                                                               */
/*------------------------------------------------------------------------------------------------*/
bbool bCore_InitOpenGL();
/*------------------------------------------------------------------------------------------------*/
/* bCore_ResizeScreen                                                                             */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_ResizeScreen(buint _width, buint _height);
/*------------------------------------------------------------------------------------------------*/
/* bCore_BuildCameraPatch                                                                         */
/*------------------------------------------------------------------------------------------------*/
bPatch *bCore_BuildCameraPatch();
/*------------------------------------------------------------------------------------------------*/
/* bCore_ApplyPatch                                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_ApplyPatch(bWorld *_world, bPatch *_patch);
/*------------------------------------------------------------------------------------------------*/
/* bCore_EventHandler                                                                             */
/*------------------------------------------------------------------------------------------------*/
LRESULT __stdcall bCore_EventHandler(HWND _hwnd, UINT _msg, WPARAM _wp, LPARAM _lp);
/*------------------------------------------------------------------------------------------------*/
/* Global engine variables                                                                        */
/*------------------------------------------------------------------------------------------------*/
BOOL (APIENTRY * wglSwapIntervalEXT)(int);
bCore_AppInfo  *bCore_App         = 0;
bWorld         *bCore_RenderWorld = 0;
bWorld         *bCore_LivingWorld = 0;
bLisp_Script   *bCore_MainScript  = 0;
bCamera        *bCore_MainCamera  = 0;
bConfig        *bCore_MainConfig  = 0;
bPatch         *bCore_PatchCircle = 0;
/*------------------------------------------------------------------------------------------------*/
/* bCore_Create                                                                                   */
/*------------------------------------------------------------------------------------------------*/
bbool bCore_Create(HINSTANCE _hinst, bchar *_cmd)
{
	bchar *e; /* blisp error string */

	if(bCore_App)
	{
		bLog_Print(L"Double engine initialization detected", bLog_Warning);
		return bfalse;
	}

	bCore_App           = malloc(sizeof(bCore_AppInfo));
	bCore_App->settings = bSettings_Create();

	bCore_App->hwnd = bWindow_Create(bCore_EventHandler, bCore_App->settings);

	if(!bCore_App->hwnd)
	{
		bError(bError_Info, L"Error while creating game window . . .");
	}

	bCore_App->main_event   = CreateEvent(0, TRUE, FALSE, L"6y6ykaMainThread");
	bCore_App->render_event = CreateEvent(0, TRUE, FALSE, L"6y6ykaRenderThread");

	bCore_PatchCircle = bPatch_Circle_Create(10);

	_beginthread(bCore_RenderThread, 0, 0);

	/* some code */
	bCore_MainScript = bLisp_OpenScript(bCore_DefaultScript, &e);

	if(!bCore_MainScript)
	{
		bError(bError_Info, L"Error while loaing main script: \r\n\"%s\"", e);
	}

	bApi_Register(bCore_MainScript);

	bInput_Refresh();

	bCore_MainCamera  = bCamera_Create();
	bCore_LivingWorld = bWorld_Create();
	bCore_RenderWorld = bWorld_Create();

	/* render thread init done */
	WaitForSingleObject(bCore_App->render_event, INFINITE);

	ShowWindow(bCore_App->hwnd, SW_SHOW);
	UpdateWindow(bCore_App->hwnd);

	if(!bLisp_Run(bCore_MainScript, 0, &e))
	{
		bError(bError_Info, L"Error while running script: \r\n\"%s\"", e);
	}

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_Delete                                                                                   */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_Delete()
{
	bLisp_Close(bCore_MainScript);

	bWindow_Delete(bCore_App->hwnd);

	bSettings_Delete(bCore_App->settings);

	free(bCore_App);
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_Start                                                                                    */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_Start()
{
	DWORD result;

	bCore_MessageMonster();

	bTimer_Refresh();
	bInput_Refresh();

	result = WaitForSingleObject(bCore_App->render_event, 5);

	if(result == WAIT_OBJECT_0)
	{
		bCamera_GetMatrix(bCore_PatchCircle->data.matrix, bCore_MainCamera);
		bCore_PatchCircle->type = bPatch_Camera;
		bCore_PatchCircle->empty = bfalse;
		if(!bCore_PatchCircle->next->empty)
		{
			bPatch_Circle_Insert(bCore_PatchCircle);
		}
		bCore_PatchCircle = bCore_PatchCircle->next;

		bCore_PatchCircle->type = bPatch_Frame;
		bCore_PatchCircle->empty = bfalse;
		if(!bCore_PatchCircle->next->empty)
		{
			bPatch_Circle_Insert(bCore_PatchCircle);
		}
		bCore_PatchCircle = bCore_PatchCircle->next;

		ResetEvent(bCore_App->render_event);
		SetEvent(bCore_App->main_event);
	}
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_RenderThread                                                                             */
/*------------------------------------------------------------------------------------------------*/
void bCore_RenderThread(void *_arg)
{
	bPatch *patch = bCore_PatchCircle;
	HANDLE r      = OpenEvent(EVENT_ALL_ACCESS, 0, L"6y6ykaRenderThread");
	HANDLE m      = OpenEvent(EVENT_ALL_ACCESS, 0, L"6y6ykaMainThread"  );

	bLostVariable(_arg);

	if(!bCore_InitOpenGL())
	{
		bError(bError_Info, L"OpenGL initialization FAIL!!!");
	}

	wglSwapIntervalEXT = (BOOL (APIENTRY *)(int))wglGetProcAddress("wglSwapIntervalEXT");
	wglSwapIntervalEXT(1);

	//bCore_ResizeScreen(1280, 720);

	/* render loop */
	while(L"i love c")
	{
		SetEvent(r);
		WaitForSingleObject(m, INFINITE);
		ResetEvent(m);

		while(patch->type != bPatch_Frame)
		{
			bCore_ApplyPatch(bCore_RenderWorld, patch);
			patch->empty = btrue;
			patch = patch->next;
		}

		patch = patch->next;
		
		bRender_DrawWorld(bCore_RenderWorld);
		SwapBuffers(bCore_App->hdc);
	}
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_GetMainCamera                                                                            */
/*------------------------------------------------------------------------------------------------*/
bCamera *bCore_GetMainCamera()
{
	return bCore_MainCamera;
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_BuildCameraPatch                                                                         */
/*------------------------------------------------------------------------------------------------*/
bPatch *bCore_BuildCameraPatch()
{
	bPatch *patch;

	patch = malloc(sizeof(bPatch));

	bCamera_GetMatrix(patch->data.matrix, bCore_MainCamera);

	patch->type = bPatch_Camera;
	
	return patch;
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_ApplyPatch                                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_ApplyPatch(bWorld *_world, bPatch *_patch)
{
	switch(_patch->type)
	{
	case bPatch_Camera:
		bWorld_SetCameraMatrix(_world, _patch->data.matrix);
		break;

	case bPatch_Resize:
		bCore_ResizeScreen(_patch->data.point.x, _patch->data.point.y);
		break;
	}
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_ResizeScreen                                                                             */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_ResizeScreen(buint _width, buint _height)
{
	glViewport(0, 0, _width, _height);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
 
    gluPerspective(45.0f, (GLfloat)_width/(GLfloat)_height, 0.1f, 100.0f);
 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_MessageMonster                                                                           */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_MessageMonster()
{
	MSG msg;

	while(PeekMessage(&msg, bCore_App->hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_InitOpenGL                                                                               */
/*------------------------------------------------------------------------------------------------*/
bbool bCore_InitOpenGL()
{
	bint pixel_format = 0;

	static PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1, 
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
		32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0 };

	bCore_App->hdc = GetDC(bCore_App->hwnd);
	if(!bCore_App->hdc)
	{
		bLog_Print(L"Problems while getting HDC", bLog_Warning);
		return bfalse;
	}

	pixel_format = ChoosePixelFormat(bCore_App->hdc, &pfd);
	if(!pixel_format)
	{
		bLog_Print(L"Problems while choosing pixel format", bLog_Warning);
		return bfalse;
	}

	if(!SetPixelFormat(bCore_App->hdc, pixel_format, &pfd))
	{
		bLog_Print(L"Problems while setting pixel format", bLog_Warning);
		return bfalse;
	}

	bCore_App->hglrc = wglCreateContext(bCore_App->hdc);
	if(!bCore_App->hglrc)
	{
		bLog_Print(L"Problems while context creation process", bLog_Warning);
		return bfalse;
	}

	if(!wglMakeCurrent(bCore_App->hdc, bCore_App->hglrc))
	{
		bLog_Print(L"Problems with function 'wglMakeCurrent'", bLog_Warning);
		return bfalse;
	}

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_Window_EventHandler                                                                      */
/*------------------------------------------------------------------------------------------------*/
LRESULT __stdcall bCore_EventHandler(HWND _hwnd, UINT _msg, WPARAM _wp, LPARAM _lp)
{
	switch (_msg)
	{
    case WM_DESTROY:
	case WM_CLOSE:
	case WM_QUIT:
		exit(0);
		break;

	case WM_SIZE:
		bCore_PatchCircle->type = bPatch_Resize;
		bCore_PatchCircle->data.point.x = LOWORD(_lp);
		bCore_PatchCircle->data.point.y = HIWORD(_lp);
		bCore_PatchCircle->empty = bfalse;
		if(!bCore_PatchCircle->next->empty)
		{
			bPatch_Circle_Insert(bCore_PatchCircle);
		}
		bCore_PatchCircle = bCore_PatchCircle->next;
		break;

	case WM_KEYDOWN:
		if(LOWORD(_wp) == VK_ESCAPE)
			exit(0);
		bInput_PressIt(btrue, (bbyte)(LOWORD(_wp)));
		break;

	case WM_KEYUP:
		bInput_PressIt(bfalse, (bbyte)(LOWORD(_wp)));
		break;

    default:
		return DefWindowProc(_hwnd, _msg, _wp, _lp);
	}

	return 0;
}