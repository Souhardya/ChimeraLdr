#pragma once


class GlobalMemory
{
private:
	DWORD dwSize;
	DWORD dwReserved;
	PBYTE pStart = NULL; // ��������� �� ���
	PBYTE pTop = NULL; // ��������� �� ������� ����������������� ������
public:
	GlobalMemory(DWORD MemSize); // �������� ������ �������� dwSize ��� ����������� ����������
	~GlobalMemory(); // ������������ ������
	LPVOID localloc(DWORD MemSize); // �������������� ������ �������� size
};