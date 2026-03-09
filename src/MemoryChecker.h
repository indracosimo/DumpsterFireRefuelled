#pragma once
#include <cstdint>

struct MemoryChecker 
{
	uint64_t totalPhysicalMemoryBytes;
	uint64_t availablePhysicalMemoryBytes;
	uint64_t totalVirtualMemoryBytes;
	uint64_t availableVirtualMemoryBytes;
};

MemoryChecker queryMemoryStatus();
bool bCanAllocateBytes(uint64_t bytesToAllocate);