#include "global.h"


GlobalMemory::GlobalMemory(DWORD MemSize)
{
	if ((!this->pStart) && (!this->pTop))
	{
		this->dwSize = MemSize;
		this->pStart = (PBYTE)VirtualAlloc(0, MemSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		this->pTop = this->pStart;
		this->dwReserved = 0;
		if (!this->pStart)
			dbg("Memory manager: error allocating memory page");
	}
	else dbg("Memory manager: memory already allocated in this scope");
}

GlobalMemory::~GlobalMemory()
{
	VirtualFree(this->pStart, 0, MEM_RELEASE);
}

LPVOID GlobalMemory::localloc(DWORD MemSize)
{
	if ((this->pStart) && ((this->dwReserved + MemSize) < this->dwSize)) 
	{
		LPVOID ret = this->pTop;
		this->pTop += MemSize;
		this->dwReserved += MemSize;
		return ret;
	}
	else dbg("Memory manager: there is no allocated memory to reserve");
	return NULL;
}