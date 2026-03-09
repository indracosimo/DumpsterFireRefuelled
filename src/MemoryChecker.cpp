#include "MemoryChecker.h"
#include <iostream>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

MemoryChecker queryMemoryStatus()
{
	MemoryChecker status;
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(memInfo);
	if (GlobalMemoryStatusEx(&memInfo)) 
	{
		status.totalPhysicalMemoryBytes = memInfo.ullTotalPhys;
		status.availablePhysicalMemoryBytes = memInfo.ullAvailPhys;
		status.totalVirtualMemoryBytes = memInfo.ullTotalVirtual;
		status.availableVirtualMemoryBytes = memInfo.ullAvailVirtual;
	}
	return status;
}
#endif

bool bCanAllocateBytes(uint64_t bytesToAllocate)
{
	MemoryChecker status = queryMemoryStatus();
	const uint64_t safetyMarginBytes = 50 * 1024 * 1024; // 50 MB safety margin
	if (status.availablePhysicalMemoryBytes == 0) return true; 
	return bytesToAllocate + safetyMarginBytes < status.availablePhysicalMemoryBytes;
}