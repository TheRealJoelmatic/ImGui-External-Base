#include <unordered_map>
#include <iostream>

#include "../header Files/menu.hh"

#include "../header Files/modules/teleport.h"
#include "../header Files/functions/utils.h"

#include "../header Files/functions/memory.h"
#include "../header Files/globals.hh"

#include "../header Files/functions/colored_cout.h"
#include "../header Files/offsets.h"



void teleportToLocation(float Z, float X, float Y) {

    HANDLE processHandle = GetProcessHandleByName(globals.processName);
    DWORD baseAddress = getBaseAddress(processHandle, globals.processName);

    if (baseAddress == NULL) {
        ShowDebugInfo("Failed to get module base address");
        CloseHandle(processHandle);
        return;
    }

    writeMemoryToPointer(processHandle, baseAddress, offsets.xPointsOffsets, X);
    writeMemoryToPointer(processHandle, baseAddress, offsets.yPointsOffsets, Y);
    writeMemoryToPointer(processHandle, baseAddress, offsets.zPointsOffsets, Z);

	std::cout << clr::green << "Teleported to: " + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z) << std::endl;
}

float getCurrentLocation() {

    HANDLE processHandle = GetProcessHandleByName(globals.processName);
    DWORD baseAddress = getBaseAddress(processHandle, globals.processName);

    if (baseAddress == NULL) {
		ShowDebugInfo("Failed to get module base address");
        CloseHandle(processHandle);
        return 1;
    }

    float x;
    readMemoryFromPointer(processHandle, baseAddress, offsets.xPointsOffsets, &x, sizeof(float));
    if (std::isnan(x)) std::cout << clr::red << "FAILD TO FIND X \n";
    else std::cout << clr::green << "Found X \n";

    float y;
    readMemoryFromPointer(processHandle, baseAddress, offsets.yPointsOffsets, &y, sizeof(float));
    if (std::isnan(y)) std::cout << clr::red << "FAILD TO FIND Y \n";
    else std::cout << clr::green << "Found Y \n";

    float z;
    readMemoryFromPointer(processHandle, baseAddress, offsets.zPointsOffsets, &z, sizeof(float));
    if (std::isnan(z)) std::cout << clr::red << "FAILD TO FIND Z \n";
    else std::cout << clr::green << "Found Z \n";


    updateCords(x, y, z);
}