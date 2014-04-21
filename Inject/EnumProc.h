#pragma once

#include <Windows.h>
#include <list>

using namespace std;

struct ProcInfo
{
	DWORD procID;
	CString procName;
};

class CEnumProc
{
public:
	list<ProcInfo> m_procList;

public:
	CEnumProc();
	void EnumProc();
	void RefreshProc();
};