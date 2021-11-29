#pragma once


HANDLE OpenFile(LPWSTR pFile, DWORD dwAccess, DWORD dwAttrib); // открывает файл
HANDLE GetFileMapping(HANDLE hFile); // создает WRITE-COPY мэппинг
LPVOID MapToMemory(HANDLE hMap); // Ну думаю понятно
BOOL BlockCopy(LPWSTR pFrom, LPWSTR pTo); // копирует PE-файл из pFrom в pTo, устанавливая IMAGE_DLL в заголовке
BOOL CreateDllProcess(LPWSTR pDllPath); 