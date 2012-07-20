/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include <stdlib.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <process.h>
#include "b_define.h"
#include "b_error.h"
#include "b_log.h"
#include "b_window.h"
#include "b_render.h"
#include "b_lisp.h"
#include "b_world.h"
#include "b_core.h"
/*------------------------------------------------------------------------------------------------*/
#define bCore_DefaultScript L"#include \"main.blisp\""
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
/* bCore_LoadSettings                                                                             */
/*------------------------------------------------------------------------------------------------*/
bSettings *bCore_LoadSettings();
/*------------------------------------------------------------------------------------------------*/
/* bCore_UnloadSettings                                                                           */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_UnloadSettings(bSettings *_settings);
/*------------------------------------------------------------------------------------------------*/
/* bCore_RenderThread                                                                             */
/*------------------------------------------------------------------------------------------------*/
void bCore_RenderThread(void *_arg);
/*------------------------------------------------------------------------------------------------*/
/* bCore_SwapWorlds                                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_SwapWorlds(bWorld **_w1, bWorld **_w2);
/*------------------------------------------------------------------------------------------------*/
/* bCore_MessageMonster                                                                           */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_MessageMonster();
/*------------------------------------------------------------------------------------------------*/
/* bCore_StartWrapper                                                                             */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_StartWrapper(bvoid *_stack);
/*------------------------------------------------------------------------------------------------*/
/* bCore_InitOpenGL                                                                               */
/*------------------------------------------------------------------------------------------------*/
bbool bCore_InitOpenGL();
/*------------------------------------------------------------------------------------------------*/
/* bCore_ResizeScreen                                                                             */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_ResizeScreen(buint _width, buint _height);
/*------------------------------------------------------------------------------------------------*/
/* bCore_EventHandler                                                                             */
/*------------------------------------------------------------------------------------------------*/
LRESULT __stdcall bCore_EventHandler(HWND _hwnd, UINT _msg, WPARAM _wp, LPARAM _lp);
/*------------------------------------------------------------------------------------------------*/
/* Global engine variables                                                                        */
/*------------------------------------------------------------------------------------------------*/
bCore_AppInfo *bCore_App         = 0;
bWorld        *bCore_RawWorld    = 0;
bWorld        *bCore_LivingWorld = 0;
bLisp_Script  *bCore_MainScript  = 0;
/*------------------------------------------------------------------------------------------------*/
/* bCore_Create                                                                                   */
/*------------------------------------------------------------------------------------------------*/
bbool bCore_Create()
{
	bchar *e;

	if(bCore_App)
	{
		bLog_Print(L"Double engine initialization detected", bLog_Warning);
		return bfalse;
	}

	bCore_App           = malloc(sizeof(bCore_AppInfo));
	bCore_App->settings = bCore_LoadSettings();

	bCore_App->hwnd = bWindow_Create(bCore_EventHandler, bCore_App->settings);

	if(!bCore_App->hwnd)
	{
		bError(bError_Info, L"Error while creating game window . . .");
	}

	bCore_App->main_event   = CreateEvent(0, TRUE, FALSE, L"6y6ykaMainThread");
	bCore_App->render_event = CreateEvent(0, TRUE, FALSE, L"6y6ykaRenderThread");

	_beginthread(bCore_RenderThread, 0, 0);

	/* some code */
	bCore_MainScript = bLisp_OpenScript(bCore_DefaultScript, &e);

	if(!bCore_MainScript)
	{
		bError(bError_Info, L"Error while loaing main script: \r\n\"%s\"", e);
	}

	bLisp_RegisterFunction(bCore_MainScript, L"bCore_Start", bCore_StartWrapper);

	/* render thread init done */
	WaitForSingleObject(bCore_App->render_event, INFINITE);

	ShowWindow(bCore_App->hwnd, SW_SHOW);
	UpdateWindow(bCore_App->hwnd);

	/* render thread init done */
	//WaitForSingleObject(bCore_App->render_event, INFINITE);

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

	bCore_UnloadSettings(bCore_App->settings);

	free(bCore_App);
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_Start                                                                                    */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_Start()
{
	/* пока выполнится */
	WaitForSingleObject(bCore_App->render_event, INFINITE);
	ResetEvent(bCore_App->render_event);
	bCore_MessageMonster();
	bCore_SwapWorlds(&bCore_RawWorld, &bCore_LivingWorld);
	SetEvent(bCore_App->main_event);

	
	/* build new world */
	bCore_RawWorld;

	//bLog_Print(L"Thread 1 running . . .", bLog_Normal);
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_RenderThread                                                                             */
/*------------------------------------------------------------------------------------------------*/
void bCore_RenderThread(void *_arg)
{
	HANDLE r = OpenEvent(EVENT_ALL_ACCESS, 0, L"6y6ykaRenderThread");
	HANDLE m = OpenEvent(EVENT_ALL_ACCESS, 0, L"6y6ykaMainThread"  );

	//bLog_Print(L"Thread 2 running . . .", bLog_Normal);
	/* open gl init */
	if(!bCore_InitOpenGL())
	{
		bError(bError_Info, L"OpenGL initialization FAIL!!!");
	}

	/* render loop */
	while(L"i love c")
	{
		SetEvent(r);
		WaitForSingleObject(m, INFINITE);
		ResetEvent(m);

		//bLog_Print(L"Thread 2 running . . .", bLog_Warning);
		/* render it */
		bRender_DrawWorld(bCore_LivingWorld);
		SwapBuffers(bCore_App->hdc);
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

	bCore_ResizeScreen(800, 600);

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_SwapWorlds                                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_SwapWorlds(bWorld **_world1, bWorld **_world2)
{
	bvoid *black_hole;

	black_hole = *_world1;
	*_world1   = *_world2;
	*_world2   = black_hole;
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_LoadSettings                                                                             */
/*------------------------------------------------------------------------------------------------*/
bSettings *bCore_LoadSettings()
{
	bSettings *settings = malloc(sizeof(bSettings));

	settings->audio.music  = btrue;
	settings->audio.volume = 50;

	settings->game.level   = 0;

	settings->video.fullscreen = bfalse;
	settings->video.width      = 800;
	settings->video.height     = 600;

	return settings;
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_UnloadSettings                                                                           */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_UnloadSettings(bSettings *_settings)
{
	free(_settings);
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_StartWrapper                                                                             */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_StartWrapper(bvoid *_stack)
{
	bCore_Start();
	bLisp_ReturnNumber(_stack, 0);
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
		bCore_ResizeScreen(LOWORD(_lp), HIWORD(_lp));
		break;

    default:
		return DefWindowProc(_hwnd, _msg, _wp, _lp);
	}

	return 0;
}