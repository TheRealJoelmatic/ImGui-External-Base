#include "../header Files/menu.hh"

#include "../header Files/modules/teleport.h"
#include "../header Files/functions/utils.h"

#include "../header Files/functions/memory.h"

std::string processName = "Refunct-Win32-Shipping.exe";
HANDLE processHandle;
DWORD processId;


DWORD yPointer = 0x1E306A4;
DWORD yoffset1 = 0x1E8;
DWORD yoffset2 = 0x698;


std::vector<DWORD> xPointsOffsets = { yPointer, yoffset1, yoffset2 - 0x4 };
std::vector<DWORD> yPointsOffsets = { yPointer, yoffset1, yoffset2 };
std::vector<DWORD> zPointsOffsets = { yPointer, yoffset1, yoffset2 - 0x8 };

void teleportToLocation(float Z, float X, float Y) {

    HANDLE processHandle = GetProcessHandleByName(processName);
    DWORD baseAddress = getBaseAddress(processHandle, processName);

    if (baseAddress == NULL) {
        ShowDebugInfo("Failed to get module base address");
        CloseHandle(processHandle);
        return;
    }

    writeMemoryToPointer(processHandle, baseAddress, xPointsOffsets, X);
    writeMemoryToPointer(processHandle, baseAddress, yPointsOffsets, Y);
    writeMemoryToPointer(processHandle, baseAddress, zPointsOffsets, Z);

	std::cout << "Teleported to: " + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z) << std::endl;
}

float getCurrentLocation() {

    HANDLE processHandle = GetProcessHandleByName(processName);
    DWORD baseAddress = getBaseAddress(processHandle, processName);

    if (baseAddress == NULL) {
		ShowDebugInfo("Failed to get module base address");
        CloseHandle(processHandle);
        return 1;
    }

    float x = readMemoryFromPointer(processHandle, baseAddress, xPointsOffsets);
    float y = readMemoryFromPointer(processHandle, baseAddress, yPointsOffsets);
    float z = readMemoryFromPointer(processHandle, baseAddress, zPointsOffsets);

    updateCords(x, y, z);
    //ShowDebugInfo("Done");
}