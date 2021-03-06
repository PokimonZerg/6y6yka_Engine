/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include <stdarg.h>
#include <stdio.h>
#include "b_log.h"
#include "b_align.h"
#include "b_error.h"
/*------------------------------------------------------------------------------------------------*/
bbool bError_CreateDialog(bchar *_text);
/*------------------------------------------------------------------------------------------------*/
bint WINAPI bError_DlgFunc(HWND _dhwnd, UINT _msg, WPARAM _wp, LPARAM _lp);
/*------------------------------------------------------------------------------------------------*/
#define bError_DialogTitle  L"6y6yka Engine Error"
#define bError_ButtonTitle  L"OK"
#define bError_ButtonId     100
#define bError_EditBoxId    101
/*------------------------------------------------------------------------------------------------*/
bvoid bError(bchar *_info, bchar *_text, ...)
{
	bchar   err_text[512];
	bchar   fmt_text[256];
	va_list arg;

	va_start(arg, _text);
	vswprintf(fmt_text, 256, _text, arg);
	va_end(arg);

	swprintf(err_text, 512, L" Fatal error detected: \r\n %s \r\n Text: %s",
		     _info, fmt_text);

	bLog_Print(err_text, bLog_Error); /* print in log file */
	MessageBeep(MB_OK);               /* play error sound  */
	bError_CreateDialog(err_text);    /* show error window */

	exit(0);
}
/*------------------------------------------------------------------------------------------------*/
bbool bError_CreateDialog(bchar *_text)
{
	bbyte dialog_mem[sizeof(DLGTEMPLATE)         + 
					 sizeof(bError_DialogTitle)  +
					 sizeof(bError_ButtonTitle)  +
					 sizeof(DLGITEMTEMPLATE) * 2 +
					 sizeof(WORD) * 11           ]; 
	/* 11 = 2 to dialog, 3 to button, 3 to edit, 2 to align, 2 to edit text */

	DLGTEMPLATE     *err_dlg;  /* dialog ptr   */
	DLGITEMTEMPLATE *err_cmp;  /* controll ptr */
	bbyte           *dmem_ptr; /* common ptr   */
	buint           *uint_ptr;
	WORD            *word_ptr;

	/* create dialog */
	err_dlg = (DLGTEMPLATE *)dialog_mem;
	err_dlg->style = DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION;
	err_dlg->dwExtendedStyle = 0;
	err_dlg->cx = 256; err_dlg->x = 0; 
	err_dlg->cy = 128; err_dlg->y = 0;
	err_dlg->cdit = 2;

	dmem_ptr = (bbyte *)(err_dlg + 1);

	uint_ptr = (buint *)dmem_ptr;
	*uint_ptr++ = 0;                    /* no menu and standart dialog class */
	dmem_ptr = (bbyte *)uint_ptr;

	memcpy(dmem_ptr, bError_DialogTitle, sizeof(bError_DialogTitle));

	/* create button */
	err_cmp = bAlign4(dmem_ptr + sizeof(bError_DialogTitle));

	err_cmp->style = WS_CHILD | WS_VISIBLE | WS_BORDER;
	err_cmp->dwExtendedStyle = 0; err_cmp->id = bError_ButtonId;
	err_cmp->cx = 64;             err_cmp->x  = 192;
	err_cmp->cy = 16;             err_cmp->y  = 112;
	
	dmem_ptr = (bbyte *)(err_cmp + 1);

	word_ptr = (WORD *)dmem_ptr;
	*word_ptr++ = 0xFFFF;
	*word_ptr++ = 0x0080;
	dmem_ptr = (bbyte *)word_ptr;

	memcpy(dmem_ptr, bError_ButtonTitle, sizeof(bError_ButtonTitle));

	/* create text box */
	dmem_ptr += sizeof(bError_ButtonTitle);

	word_ptr = (WORD *)dmem_ptr;
	*word_ptr++ = 0;
	dmem_ptr = (bbyte *)word_ptr;

	err_cmp = bAlign4(dmem_ptr);
	err_cmp->style = WS_VISIBLE | ES_MULTILINE | ES_AUTOHSCROLL | ES_READONLY | WS_BORDER;
	err_cmp->dwExtendedStyle = 0; err_cmp->id = bError_EditBoxId;
	err_cmp->cx = 256;            err_cmp->x  = 0;
	err_cmp->cy = 112;            err_cmp->y  = 0;

	dmem_ptr = (bbyte *)(err_cmp + 1);

	word_ptr    = (WORD *)dmem_ptr;
	*word_ptr++ = 0xFFFF;
	*word_ptr++ = 0x0081;            /* edit box class */
	uint_ptr    = (buint *)word_ptr;
	*uint_ptr   = 0;                 /* no text        */

	DialogBoxIndirectParam(0, err_dlg, 0, bError_DlgFunc, (LPARAM)_text);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
bint WINAPI bError_DlgFunc(HWND _dhwnd, UINT _msg, WPARAM _wp, LPARAM _lp)
{
	switch(_msg)
	{
	case WM_INITDIALOG:
		SetWindowText(GetDlgItem(_dhwnd, bError_EditBoxId), (bchar *)_lp);
		return btrue;
		
	case WM_CLOSE:
		EndDialog(_dhwnd, 0);
		return btrue;

	case WM_COMMAND:
		if(LOWORD(_wp) == bError_ButtonId)
			EndDialog(_dhwnd, 0);
		return btrue;

	default:
		return bfalse;
	}
}