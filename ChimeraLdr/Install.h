#pragma once


HANDLE OpenFile(LPWSTR pFile, DWORD dwAccess, DWORD dwAttrib); // ��������� ����
HANDLE GetFileMapping(HANDLE hFile); // ������� WRITE-COPY �������
LPVOID MapToMemory(HANDLE hMap); // �� ����� �������
BOOL BlockCopy(LPWSTR pFrom, LPWSTR pTo); // �������� PE-���� �� pFrom � pTo, ������������ IMAGE_DLL � ���������
BOOL CreateDllProcess(LPWSTR pDllPath); 