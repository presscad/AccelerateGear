#include "stdafx.h"
#include <tlhelp32.h>
#include "EnumProc.h"

CEnumProc::CEnumProc()
{
	EnumProc();
}

void CEnumProc::EnumProc()
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap)
	{
		BOOL res = ::Process32First(hProcessSnap, &pe32);
		while (res)
		{
			ProcInfo procInfo;
			procInfo.procID = pe32.th32ProcessID;
			procInfo.procName = pe32.szExeFile;
			m_procList.push_back(procInfo);
			res = ::Process32Next(hProcessSnap, &pe32);
		}
	}
	CloseHandle(hProcessSnap);
}

void CEnumProc::RefreshProc()
{
	m_procList.clear();
	EnumProc();
}