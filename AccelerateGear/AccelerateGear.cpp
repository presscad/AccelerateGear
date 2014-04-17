// AccelerateGear.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#pragma data_seg (".shared")
UINT	WM_HOOKEX = 0;
HHOOK	g_hHook = NULL;
HWND	g_hWnd = NULL;
#pragma data_seg ()
#pragma comment(linker,"/SECTION:.shared,RWS")

#define ENABLE_ACCELERATE TRUE
#define DISABLE_ACCELERATE FALSE

BOOL isAccelerate = FALSE;

#define pCW ((CWPSTRUCT*)lParam)
LRESULT CALLBACK HookProc (
						   int code,
						   WPARAM wParam,
						   LPARAM lParam
						   )
{
	::UnhookWindowsHookEx(g_hHook);
	if (pCW->message == WM_HOOKEX && pCW->lParam == ENABLE_ACCELERATE)
	{
		if (!isAccelerate)
		{
			char libName[MAX_PATH]; 
			::GetModuleFileName(::GetModuleHandle("AccelerateGear.dll"), libName, MAX_PATH);
			if (::LoadLibrary(libName))
			{
				isAccelerate = TRUE;
			}
		}
	}
	else if (pCW->message == WM_HOOKEX && pCW->lParam == DISABLE_ACCELERATE)
	{
		if (isAccelerate)
		{
			isAccelerate = FALSE;
			Sleep(200);
			::FreeLibrary(::GetModuleHandle("AccelerateGear.dll"));
		}
	}
	return ::CallNextHookEx(NULL, code, wParam, lParam);
}


extern "C" _declspec(dllexport) BOOL __stdcall StartAccelerate(HWND hWnd, BOOL isEnableAccelerate)
{
	if (isEnableAccelerate == ENABLE_ACCELERATE)
	{
		MessageBox(NULL, "StartAccelerate", NULL, NULL);
	}
	else
	{
		MessageBox(NULL, "StopAccelerate", NULL, NULL);
	}
	return TRUE;
	
	g_hWnd = hWnd;
	if (WM_HOOKEX == NULL)
	{
		WM_HOOKEX = ::RegisterWindowMessage("WM_HOOKEX_RK");
	}
	g_hHook = ::SetWindowsHookEx(WH_CALLWNDPROC, HookProc, ::GetModuleHandle("AccelerateGear.dll"), GetWindowThreadProcessId(hWnd, NULL));
	if (g_hHook)
	{
		SendMessage(hWnd, WM_HOOKEX, NULL, isEnableAccelerate);
		return TRUE;
	}
	return FALSE;
}