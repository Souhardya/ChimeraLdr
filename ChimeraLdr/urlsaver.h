#pragma once

#include <windows.h>
#include <wininet.h>

class Downloader
{
public:
	Downloader();
	BOOL save(LPWSTR from, LPWSTR to);
	~Downloader();
private:
	HINTERNET hFile;
	HINTERNET hInternet;
};

