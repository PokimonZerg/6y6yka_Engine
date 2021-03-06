/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include "b_file.h"
#include "b_log.h"
#include "b_config.h"
#include "b_settings.h"
/*------------------------------------------------------------------------------------------------*/
#define bSettings_ConfigName    L"options.bconfig"
#define bSettings_DefaultConfig L"\
Video = \r\n\
{\r\n\
	Window = \r\n\
	{\r\n\
		Width  = 1280\r\n\
		Height = 720\r\n\
	}\r\n\
\r\n\
	Fullscreen = 0\r\n\
	Width      = 1680\r\n\
   	Height     = 1050\r\n\
}\r\n\
\r\n\
Sound = \r\n\
{\r\n\
	Volume = 50\r\n\
	Music  = 1\r\n\
	Effect = 1\r\n\
}"
/*------------------------------------------------------------------------------------------------*/
/* bSettings_SupriseOptions                                                                       */
/*------------------------------------------------------------------------------------------------*/
bSettings *bSetting_SupriseOptions();
/*------------------------------------------------------------------------------------------------*/
/* bSettings_Create                                                                               */
/*------------------------------------------------------------------------------------------------*/
bSettings *bSettings_Create()
{
	bSettings *settings;
	bConfig   *config;

	bLog_Print(L"Loading settings . . .", bLog_Normal);

	config = bConfig_Create(bSettings_ConfigName);

	if(!config)
	{
		bLog_Print(L"Problem while opening config file", bLog_Warning);

		bFile_WriteText(bSettings_ConfigName, bSettings_DefaultConfig);

		config = bConfig_Create(bSettings_ConfigName);

		if(!config)
		{
			return bSetting_SupriseOptions();
		}
	}

	settings = malloc(sizeof(bSettings));

	if(!bConfig_GetInt (&settings->video.window_width,  L"Video->Window->Width",  config) ||
	   !bConfig_GetInt (&settings->video.window_height, L"Video->Window->Height", config) ||
	   !bConfig_GetBool(&settings->video.fullscreen,    L"Video->Fullscreen",     config) ||
	   !bConfig_GetInt (&settings->video.width,         L"Video->Width",          config) ||
	   !bConfig_GetInt (&settings->video.height,        L"Video->Height",         config) )
	{
		free(settings);
		bConfig_Delete(bSettings_ConfigName, config);

		return bSetting_SupriseOptions();
	}

	bConfig_Delete(bSettings_ConfigName, config);

	return settings;
}
/*------------------------------------------------------------------------------------------------*/
/* bSettings_Delete                                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid bSettings_Delete(bSettings *_settings)
{
	free(_settings);
}
/*------------------------------------------------------------------------------------------------*/
/* bSettings_SupriseOptions                                                                       */
/*------------------------------------------------------------------------------------------------*/
bSettings *bSetting_SupriseOptions()
{
	bSettings *settings;

	bLog_Print(L"Serious problem in options code", bLog_Warning);

	settings = malloc(sizeof(bSettings));

	settings->video.window_width  = 1280;
	settings->video.window_height = 720;
	settings->video.fullscreen    = bfalse;
	settings->video.width         = 1680;
	settings->video.height        = 1050;

	settings->game.level          = 0;

	settings->audio.music         = btrue;
	settings->audio.volume        = 50;

	return settings;
}