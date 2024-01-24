#pragma once

#ifndef MEM_H
#define MEM_H

#include <d3d9.h>
#include <vector>

//Memory mangement class
void readMemory(HANDLE processHandle, DWORD address, void* buffer, size_t size);
void readMemoryFromPointer(HANDLE processHandle, DWORD baseAddress, const std::vector<DWORD>& offsets, void* valueBuffer, size_t size);

bool writeMemory(HANDLE processHandle, DWORD address, float value);
bool writeMemoryToPointer(HANDLE processHandle, DWORD baseAddress, const std::vector<DWORD>& offsets, float value);

//Process mangement
HANDLE GetProcessHandleByName(const std::string& processName);
DWORD GetModuleBaseAddress(HANDLE processHandle, const std::string& moduleName);
DWORD getBaseAddress(HANDLE processHandle, std::string& processName);

//Misc
bool FindProcess(const std::string& processName);

#endif