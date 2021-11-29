#pragma once

#include "global.h"

void _inline dbg(LPCWSTR str, LPCWSTR str2 = NULL)
{
#ifdef _DEBUG
	INT len2 = (str != NULL) ? lstrlenW(str2) : 0;
	LPWSTR s = (LPWSTR)VirtualAlloc(0, (lstrlenW(str) + len2 + lstrlenW(CONFIG::ProjectName)) * 2 + 10, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (s)
	{
		wsprintfW(s, L"%s: %s", CONFIG::ProjectName, str);

		if (str2)
		{
			lstrcatW(s, str2);
		}

		OutputDebugStringW(s);

		VirtualFree(s, 0, MEM_RELEASE);
	}
	else
		MessageBoxW(0, L"ERROR IN dbg ROUTINE", L"FATAL ERROR", MB_ICONERROR);


#endif
}

void _inline dbg(LPCSTR str, LPCSTR str2 = NULL)
{


	INT len2 = (str != NULL) ? lstrlenA(str2) : 0;
	LPWSTR s = (LPWSTR)VirtualAlloc(0, (lstrlenA(str) + len2 + 10) * 2, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (s)
	{
		LPWSTR s2 = (s + lstrlenA(str) * 2 + 1);
		
		wsprintfW(s, L"%S", str);
		if (str2)
			wsprintfW(s2, L"%S", str2);
		else
			s2 = NULL;

		dbg(s, s2);

		VirtualFree(s, 0, MEM_RELEASE);
	}

}

void _inline dbg(DWORD str)
{

	LPWSTR s = (LPWSTR)VirtualAlloc(0, 512, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	wsprintfW(s, L"%d", str);
	dbg(s);
	VirtualFree(s, 0, MEM_RELEASE);
	
}