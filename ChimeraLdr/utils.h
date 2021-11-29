#pragma once

BOOL IsLibrary(HMODULE SelfModule); 
BOOL SetDllFlag(PIMAGE_DOS_HEADER pDosHead); 
BOOL GenerateRandomString(PWCHAR pRandomString, DWORD dwRandomLen); 