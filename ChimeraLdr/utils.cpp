#include "global.h"



typedef BOOL(WINAPI * CryptGenRandom_proto)(HCRYPTPROV, DWORD, LPBYTE);

BOOL GenerateRandomString(PWCHAR pRandomString, DWORD dwRandomLen)
{
	BOOL bRet = FALSE;

	HCRYPTPROV hProv;


	if (CryptAcquireContext(&hProv, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		WCHAR Alphabet[27];
		for (DWORD i = 0; i < 26; i++)
			Alphabet[i] = L'a' + i;

		GlobalMemory MyMem(dwRandomLen + 1);

		LPBYTE pRandomIndexes = (LPBYTE)MyMem.localloc(dwRandomLen); 

		if (!pRandomIndexes)
		{
			dbg("No memory in random generator");
			CryptReleaseContext(hProv, 0);
			return FALSE;
		}
		CHAR CryptGenRandomName[] = { 'C', 'r', 'y', 'p', 't', 'G', 'e', 'n', 'R', 'a', 'n', 'd', 'o', 'm', '\0' };
		CHAR AdvapiName[] = { 'A', 'd', 'v', 'a', 'p', 'i', '3', '2', '.', 'd', 'l', 'l', '\0' };

		HMODULE hAdvapi = GetModuleHandleA(AdvapiName);
		if (!hAdvapi)
			hAdvapi = LoadLibraryA(AdvapiName);

		if (hAdvapi)
		{
			CryptGenRandom_proto CryptGenRandomFunc = (CryptGenRandom_proto)GetProcAddress(hAdvapi, CryptGenRandomName);
			if (CryptGenRandomFunc)
			{
				if (CryptGenRandomFunc(hProv, dwRandomLen, pRandomIndexes))
				{
					for (DWORD i = 0; i < dwRandomLen; i++)
					{
						pRandomString[i] = Alphabet[pRandomIndexes[i] % 26];
					}
					bRet = TRUE;
				}
				else
					dbg("CryptGenRandom failed");
			}
			else
				dbg("CryptGenRandom not found in advapi");
		}
		else
			dbg("Advapi32.dll not found");

		CryptReleaseContext(hProv, 0);
	}
	else
	{
		dbg("CryptAquireContext error");
	}


	return bRet;
}

BOOL isDllFlag(PIMAGE_DOS_HEADER pDosHead) 
{
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)((DWORD)pDosHead + (DWORD)pDosHead->e_lfanew);
	PIMAGE_FILE_HEADER pFileHead = (PIMAGE_FILE_HEADER)&pNt->FileHeader;
	return (pFileHead->Characteristics & IMAGE_FILE_DLL);
}

BOOL SetDllFlag(PIMAGE_DOS_HEADER pDosHead) 
{
	if (pDosHead->e_magic != 0x5A4D)
	{
		dbg("is not PE file");
		return FALSE;
	}
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)((DWORD)pDosHead + (DWORD)pDosHead->e_lfanew);
	PIMAGE_FILE_HEADER pFileHead = (PIMAGE_FILE_HEADER)&pNt->FileHeader;

	pFileHead->Characteristics |= IMAGE_FILE_DLL;
	return TRUE;
}

BOOL IsLibrary(HMODULE SelfModule) 
{
	if (SelfModule)
	{
		PIMAGE_DOS_HEADER pDosHead = (PIMAGE_DOS_HEADER)SelfModule;
		if (isDllFlag(pDosHead))
		{
			return TRUE;
		}
	}
	else
	{
		dbg("Error GetModuleHandleEx HMODULE NULL");
	}

	return FALSE;
}