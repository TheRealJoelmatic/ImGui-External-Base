#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <vector>

#include "../header Files/functions/memory.h"
#include "../header Files/functions/colored_cout.h"

//
// Memory mangement class
//
void readMemoryFromPointer(HANDLE processHandle, DWORD baseAddress, const std::vector<DWORD>& offsets, void* valueBuffer, size_t size) {
    DWORD currentAddress = baseAddress;

    for (size_t i = 0; i < offsets.size(); ++i) {
        if (!ReadProcessMemory(processHandle, (LPVOID)(currentAddress + offsets[i]), &currentAddress, sizeof(currentAddress), NULL)) {
            // Handle error
            DWORD error = GetLastError();
            std::cout << clr::red << "Error reading memory at offset " << i << ". Error code: " << error << std::endl;
            return;
        }

        //std::cout << clr::yellow << "Value at offset " << i << " = 0x" << std::hex << currentAddress << std::dec << std::endl;
    }

    // Use std::memcpy with the correct size
    std::memcpy(valueBuffer, &currentAddress, sizeof(size));
}

bool writeMemoryToPointer(HANDLE processHandle, DWORD baseAddress, const std::vector<DWORD>& offsets, float value) {
    DWORD currentAddress = baseAddress;

    // Traverse the pointer chain to reach the final address
    for (size_t i = 0; i < offsets.size() - 1; ++i) {
        if (!ReadProcessMemory(processHandle, (LPVOID)(currentAddress + offsets[i]), &currentAddress, sizeof(currentAddress), NULL)) {
            // Handle error
            DWORD error = GetLastError();
            std::cerr << clr::red << "Error reading memory at offset " << i << ". Error code: " << error << std::endl;
            return false;
        }
    }

    // Calculate the final address by adding the last offset
    DWORD finalAddress = currentAddress + offsets.back();

    // Write the float value to the specified memory address
    if (!WriteProcessMemory(processHandle, (LPVOID)finalAddress, &value, sizeof(value), NULL)) {
        // Handle error
        DWORD error = GetLastError();
        std::cerr << clr::red << "Error writing memory. Error code: " << error << std::endl;
        return false;
    }

    std::cout << clr::green << "Successfully wrote value " << value << " to address 0x" << std::hex << finalAddress << std::dec << std::endl;

    return true;
}

bool writeMemory(HANDLE processHandle, DWORD address, float value) {
    if (!WriteProcessMemory(processHandle, (LPVOID)address, &value, sizeof(value), NULL)) {
        // Handle error
        DWORD error = GetLastError();
        std::cerr << "Error writing memory. Error code: " << error << std::endl;
        return false;
    }
    //std::cout << clr::green << "Successfully wrote value " << value << " to address 0x" << std::hex << address << std::dec << std::endl;

    return true;
}

void readMemory(HANDLE processHandle, DWORD address, void* buffer, size_t size) {
    ReadProcessMemory(processHandle, (LPCVOID)address, buffer, size, NULL);
}
//
// Process mangement class
//
HANDLE GetProcessHandleByName(const std::string& processName) {
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create process snapshot" << std::endl;
        return nullptr;
    }

    if (Process32First(snapshot, &processEntry)) {
        do {
            if (processName == processEntry.szExeFile) {
                CloseHandle(snapshot);
                return OpenProcess(PROCESS_ALL_ACCESS, FALSE, processEntry.th32ProcessID);
            }
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return nullptr;
}

DWORD GetModuleBaseAddress(HANDLE processHandle, const std::string& moduleName) {
    MODULEENTRY32 moduleEntry;
    moduleEntry.dwSize = sizeof(MODULEENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessId(processHandle));

    if (snapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create module snapshot" << std::endl;
        return 0;
    }

    if (Module32First(snapshot, &moduleEntry)) {
        do {
            if (moduleName == moduleEntry.szModule) {
                CloseHandle(snapshot);
                return (DWORD)moduleEntry.modBaseAddr;
            }
        } while (Module32Next(snapshot, &moduleEntry));
    }

    CloseHandle(snapshot);
    return 0;
}

DWORD getBaseAddress(HANDLE processHandle, std::string& processName) {

    if (processHandle == nullptr) {
        std::cerr << "Failed to get process handle" << std::endl;
        return  NULL;
    }

    DWORD baseAddress = GetModuleBaseAddress(processHandle, processName);

    if (baseAddress == 0) {
        std::cerr << "Failed to get module base address" << std::endl;
        CloseHandle(processHandle);
        return NULL;
    }

    return baseAddress;
}

//
// Misc
//
bool FindProcess(const std::string& processName) {
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        MessageBoxA(NULL, "Failed to create process snapshot", "Error", MB_OK | MB_ICONERROR);
        return false;
    }

    if (Process32First(snapshot, &processEntry)) {
        do {
            if (processName == processEntry.szExeFile) {
                CloseHandle(snapshot);
                return true;
            }
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return false;
}