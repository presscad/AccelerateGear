#include "stdafx.h"
#include <tlhelp32.h>
#include "EnumProc.h"

CEnumProc::CEnumProc()
{
	EnumProcess();
	EnumProcInfo();
}

void CEnumProc::EnumProcess()
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap)
	{
		if (Process32First(hProcessSnap, &pe32))
		{
			do 
			{
				ProcInfo procInfo;
				procInfo.procID = pe32.th32ProcessID;
				procInfo.procName = pe32.szExeFile;
				memset(procInfo.wndName, 0, 255);
				m_procList.push_back(procInfo);
			} while (Process32Next(hProcessSnap, &pe32));
		}
	}
	CloseHandle(hProcessSnap);
}

void CEnumProc::RefreshProc()
{
	m_procList.clear();
	EnumProcess();
	EnumProcInfo();
}

struct WndInfo
{
	DWORD dwProcessId; 
	HWND hWnd; 
};
BOOL CALLBACK CEnumProc::MyEnumWnd(HWND hWnd, LPARAM lParam)
{
	BOOL res = TRUE;
	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	WndInfo *wndinfo = (WndInfo *)lParam;
	if (dwProcessId == wndinfo->dwProcessId)
	{
		wndinfo->hWnd = hWnd;
		res = FALSE;
	}
	return res;
}

HWND CEnumProc::GetMainWnd(DWORD dwProcessId)
{
	WndInfo winInfo;
	winInfo.dwProcessId = dwProcessId;
	winInfo.hWnd = NULL;
	EnumWindows(MyEnumWnd,(LPARAM)&winInfo);
	return winInfo.hWnd;
}

void CEnumProc::EnumProcInfo()
{
	vector<ProcInfo>::iterator ivec;
	for (ivec = m_procList.begin(); ivec != m_procList.end(); ivec++)
	{
		ivec->hWnd = GetMainWnd(ivec->procID);
		if (ivec->hWnd)
		{
			::GetWindowText(ivec->hWnd, ivec->wndName, 255);
		}
	}
}