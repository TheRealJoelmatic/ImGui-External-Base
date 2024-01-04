#pragma once

#ifndef MEM_H
#define MEM_H

#include "../globals.hh"

//Memory mangement class
float readMemoryFromPointer(HANDLE processHandle, DWORD baseAddress, const std::vector<DWORD>& offsets);
bool writeMemoryToPointer(HANDLE processHandle, DWORD baseAddress, const std::vector<DWORD>& offsets, float value);

//Process mangement
HANDLE GetProcessHandleByName(const std::string& processName);
DWORD GetModuleBaseAddress(HANDLE processHandle, const std::string& moduleName);
DWORD getBaseAddress(HANDLE processHandle, std::string& processName);

//Misc
bool FindProcess(const std::string& processName);

#endif