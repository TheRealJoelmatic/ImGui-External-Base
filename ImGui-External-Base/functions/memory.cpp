#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <vector>
#include <Psapi.h>

#include "../header Files/functions/memory.h"
#include "../header Files/functions/colored_cout.h"
#include "../header Files/globals.hh"

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

view_matrix_t readViewMatrix(HANDLE processHandle, uintptr_t address, uintptr_t nextOffset) {

    view_matrix_t result;

    //std::cout << "View Matrix:" << std::endl;
    for (int i = 0; i < 16; ++i) {
        float currentValue;
        uintptr_t currentAdress = address + i * nextOffset;

        ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(currentAdress), &currentValue, sizeof(currentValue), nullptr);

        if (currentValue == NULL) {
            std::cout << clr::red << "FAILED TO READ THE VEIW MATRIX (" << i << ")" << std::endl;
            view_matrix_t failedResult = {};
            return failedResult;
        }

        result.matrix[i] = currentValue;

        //std::cout << clr::yellow << i << " | Read " << currentAdress << " | " << result.matrix[i] << std::endl;
    }

    return result;
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

BOOL CALLBACK enum_windows(HWND hwnd, LPARAM param) {
    DWORD process_id;
    GetWindowThreadProcessId(hwnd, &process_id);
    if (process_id == param)
    {
        sdk.hwnd = hwnd;
        return false;
    }
    return true;
}

DWORD GetPIDByModuleName(const char* moduleName) {
    DWORD processIds[1024], cbNeeded, count;
    if (EnumProcesses(processIds, sizeof(processIds), &cbNeeded)) {
        count = cbNeeded / sizeof(DWORD);
        for (DWORD i = 0; i < count; ++i) {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIds[i]);
            if (hProcess != NULL) {
                HMODULE hMod;
                DWORD cbNeeded;

                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
                    char szModName[MAX_PATH];
                    if (GetModuleBaseNameA(hProcess, hMod, szModName, sizeof(szModName) / sizeof(char))) {
                        if (strstr(szModName, moduleName) != nullptr) {
                            CloseHandle(hProcess);
                            return processIds[i];
                        }
                    }
                }
                CloseHandle(hProcess);
            }
        }
    }
    return 0; // Not found
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