/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include <stdlib.h>
#include <process.h>
#include "b_define.h"
#include "b_error.h"
#include "b_log.h"
#include "b_window.h"
#include "b_world.h"
#include "b_core.h"
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
/* bCore_EventHandler                                                                             */
/*------------------------------------------------------------------------------------------------*/
LRESULT __stdcall bCore_EventHandler(HWND _hwnd, UINT _msg, WPARAM _wp, LPARAM _lp);
/*------------------------------------------------------------------------------------------------*/
/* Global engine variables                                                                        */
/*------------------------------------------------------------------------------------------------*/
bCore_AppInfo *bCore_App         = 0;
bWorld        *bCore_RawWorld    = 0;
bWorld        *bCore_LivingWorld = 0;
/*------------------------------------------------------------------------------------------------*/
/* bCore_Create                                                                                   */
/*------------------------------------------------------------------------------------------------*/
bbool bCore_Create()
{
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

	/* render thread init done */
	WaitForSingleObject(bCore_App->render_event, INFINITE);
	//ResetEvent(bCore_App->render_event);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_Delete                                                                                   */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_Delete()
{
	bWindow_Delete(bCore_App->hwnd);

	bCore_UnloadSettings(bCore_App->settings);

	free(bCore_App);
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_Start                                                                                    */
/*------------------------------------------------------------------------------------------------*/
bvoid bCore_Start()
{
	while(L"i love c")
	{
		SetEvent(bCore_App->main_event);
		/* пока выполнится */
		WaitForSingleObject(bCore_App->render_event, INFINITE);
		ResetEvent(bCore_App->render_event);
	
		/* build new world */
		bCore_RawWorld;

		bLog_Print(L"Thread 1 running . . .", bLog_Normal);
		bCore_SwapWorlds(&bCore_RawWorld, &bCore_LivingWorld);
	}
}
/*------------------------------------------------------------------------------------------------*/
/* bCore_RenderThread                                                                             */
/*------------------------------------------------------------------------------------------------*/
void bCore_RenderThread(void *_arg)
{
	HANDLE r = OpenEvent(EVENT_ALL_ACCESS, 0, L"6y6ykaRenderThread");
	HANDLE m = OpenEvent(EVENT_ALL_ACCESS, 0, L"6y6ykaMainThread"  );

	bLog_Print(L"Thread 2 running . . .", bLog_Normal);
	/* open gl init */

	/* render loop */
	while(L"i love c")
	{
		SetEvent(r);
		WaitForSingleObject(m, INFINITE);
		ResetEvent(m);

		bLog_Print(L"Thread 2 running . . .", bLog_Warning);
		/* render it */
	    bCore_LivingWorld;
	}
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
/* bCore_Window_EventHandler                                                                      */
/*------------------------------------------------------------------------------------------------*/
LRESULT __stdcall bCore_EventHandler(HWND _hwnd, UINT _msg, WPARAM _wp, LPARAM _lp)
{
	switch (_msg)
	{
    case WM_DESTROY:
      PostQuitMessage(0);
      break;

    default:
      return DefWindowProc(_hwnd, _msg, _wp, _lp);
	}

	return 0;
}