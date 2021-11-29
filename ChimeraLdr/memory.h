#pragma once


class GlobalMemory
{
private:
	DWORD dwSize;
	DWORD dwReserved;
	PBYTE pStart = NULL; // указатель на дно
	PBYTE pTop = NULL; // указатель на вершину зарезервированной памяти
public:
	GlobalMemory(DWORD MemSize); // выделяет память размером dwSize для последующей резервации
	~GlobalMemory(); // освобождение памяти
	LPVOID localloc(DWORD MemSize); // резервирование памяти размером size
};