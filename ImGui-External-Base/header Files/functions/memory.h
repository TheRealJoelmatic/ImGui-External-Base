#pragma once

#ifndef MEM_H
#define MEM_H

#include <d3d9.h>
#include <vector>
#include "../globals.hh"

//Memory mangement class
void readMemory(HANDLE processHandle, DWORD address, void* buffer, size_t size);
void readMemoryFromPointer(HANDLE processHandle, DWORD baseAddress, const std::vector<DWORD>& offsets, void* valueBuffer, size_t size);

bool writeMemory(HANDLE processHandle, DWORD address, float value);
bool writeMemoryToPointer(HANDLE processHandle, DWORD baseAddress, const std::vector<DWORD>& offsets, float value);

view_matrix_t readViewMatrix(HANDLE processHandle, uintptr_t address, uintptr_t nextOffset);

//Process mangement
HANDLE GetProcessHandleByName(const std::string& processName);
DWORD GetModuleBaseAddress(HANDLE processHandle, const std::string& moduleName);
DWORD getBaseAddress(HANDLE processHandle, std::string& processName);
BOOL CALLBACK enum_windows(HWND hwnd, LPARAM param);
DWORD GetPIDByModuleName(const char* moduleName);

//Misc
bool FindProcess(const std::string& processName);

#endif