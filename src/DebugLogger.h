#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

class DebugLogger
{
public:
	static void Log(const std::string& message) 
	{
		size_t count = ++messageCount[message];
		std::cout << "[" << count << "] " << message << std::endl;
	}

	static void LogWarning(const std::string& message) 
	{
		size_t count = ++messageCount[message];
		std::cerr << "[WARNING][" << count << "] " << message << std::endl;
	}
private:
	static inline std::unordered_map<std::string, size_t> messageCount;
	
};

//if (status.availablePhysicalMemoryBytes < safetyMarginBytes)
//{
//	DebugLogger::LogWarning("Low physical memory available!");
//	DebugLogger::Log("Total Physical memory: " + std::to_string(status.totalPhysicalMemoryBytes / (1024 * 1024 * 1024)) + " GB");
//	DebugLogger::Log("Available Physical Memory: " + std::to_string(status.availablePhysicalMemoryBytes / (1024 * 1024 * 1024)) + " GB");
//}
//else
//{
//	DebugLogger::Log("Memory Status: All good.");
//	DebugLogger::Log("Total Physical memory: " + std::to_string(status.totalPhysicalMemoryBytes / (1024 * 1024 * 1024)) + " GB");
//	DebugLogger::Log("Available Physical Memory: " + std::to_string(status.availablePhysicalMemoryBytes / (1024 * 1024 * 1024)) + " GB");
//}