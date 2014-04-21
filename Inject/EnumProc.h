#pragma once

#include <Windows.h>
#include <vector>

using namespace std;

struct ProcInfo
{
	DWORD procID;
	CString procName;
	HWND hWnd;
	char wndName[255];
};

class CEnumProc
{
public:
	vector<ProcInfo> m_procList;

public:
	CEnumProc();
	void EnumProcess();
	static BOOL CALLBACK MyEnumWnd(HWND hWnd, LPARAM lParam);
	HWND GetMainWnd(DWORD dwProcessId);
	void EnumProcInfo();
	void RefreshProc();
};