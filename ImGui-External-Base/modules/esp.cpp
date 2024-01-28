#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>


#include "../header Files/modules/esp.h"
#include "../header Files/functions/memory.h"
#include "../header Files/globals.hh"
#include "../header Files/offsets.h"
#include "../header Files/functions/colored_cout.h"
#include "../header Files/functions/utils.h"
#include "../header Files/functions/render/calculations.h"
#include "../header Files/functions/render/draw.h"


HANDLE processHandle;
DWORD baseAddress;
HDC hdc;

int screenX = GetSystemMetrics(SM_CXSCREEN);
int screenY = GetSystemMetrics(SM_CYSCREEN);

void espThread() {

	processHandle = GetProcessHandleByName(globals.processName);
	baseAddress = getBaseAddress(processHandle, globals.processName);
	hdc = GetDC(FindWindowA(NULL, globals.processName.c_str()));

	std::cout << clr::yellow << "-------------------- \n";

	std::cout << clr::yellow << "screenX: " << screenX << "\n";
	std::cout << clr::yellow << "screenY: " << screenY << "\n\n";

	std::cout << clr::yellow << "processHandle: " << processHandle << "\n";
	std::cout << clr::yellow << "baseAddress: " << baseAddress << "\n";
	std::cout << clr::yellow << "hdc: " << hdc << "\n";

	std::cout << clr::yellow << "-------------------- \n";


	std::cout << clr::green << "Thread Loaded :) \n";
	while (true) {
		if (!modules.isEspOn) {
			Sleep(100);
			continue;
		}

		std::cout << clr::yellow << "-------------------- \n";

		view_matrix_t vm = readViewMatrix(processHandle, baseAddress);
		std::cout << "View Matrix:" << std::endl;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				std::cout << vm[i][j] << " ";
			}
			std::cout << std::endl;
		}

		int localteam;
		std::vector<DWORD> localteamOffsets{ offsets.localPlayer, offsets.team };
		readMemoryFromPointer(processHandle, baseAddress, localteamOffsets, &localteam, sizeof(localteam));

		int toalplayers;
		std::vector<DWORD> toalplayersOffsets{ offsets.playersLeft, 0x8 };
		readMemoryFromPointer(processHandle, baseAddress, toalplayersOffsets, &toalplayers, sizeof(toalplayers));
		std::cout << clr::yellow << "toalplayers: " << toalplayers << "\n";

		for (int i = 1; i < toalplayers; i++) {

			std::vector<DWORD> pEnt{ offsets.entityList, (DWORD)i * 0x4 };
			std::cout << clr::yellow << "\n";
			
			int phealth;
			int pteam;
			float pX, pY, pZ;

			std::vector<DWORD> pEntHealth = addOnToVector(pEnt, offsets.health);
			readMemoryFromPointer(processHandle, baseAddress, pEntHealth, & phealth, sizeof(phealth));

			std::vector<DWORD> pEntTeam = addOnToVector(pEnt, offsets.team);
			readMemoryFromPointer(processHandle, baseAddress, pEntTeam, &pteam, sizeof(pteam));

			std::vector<DWORD> pXoffset = addOnToVector(pEnt, offsets.Xoffset);
			readMemoryFromPointer(processHandle, baseAddress, pXoffset, &pX, sizeof(pX));

			std::vector<DWORD> pYoffset = addOnToVector(pEnt, offsets.Yoffset);
			readMemoryFromPointer(processHandle, baseAddress, pYoffset, &pY, sizeof(pY));

			std::vector<DWORD> pZoffset = addOnToVector(pEnt, offsets.Zoffset);
			readMemoryFromPointer(processHandle, baseAddress, pZoffset, &pZ, sizeof(pZ));

			Vector3 pPosition = { pX, pY, pZ };

			Vector3 screen2d = worldToScreen(pPosition, vm, screenX, screenY);

			float height = 200;
			float width = height / 2.4f;

			if (pteam != localteam && phealth > 0 && phealth < 101) {
				DrawBox(hdc, static_cast<int>(screen2d.x), static_cast<int>(screen2d.y), 20,20);
			}

			std::cout << clr::yellow << "-------------------- \n";
			std::cout << clr::yellow << "Player Info" << "\n";
			std::cout << clr::yellow << " [" << i << "] Player" << "\n";
			std::cout << clr::yellow << " [" << (float)phealth << "] health \n";
			std::cout << clr::yellow << " [" << (float)pteam << "]  team\n\n";

			std::cout << clr::yellow << "Player Cords\n";

			std::cout << clr::yellow << " [" << pX << "] X \n";
			std::cout << clr::yellow << " [" << pY << "] Y \n";
			std::cout << clr::yellow << " [" << pZ << "] Z\n\n";

			std::cout << clr::yellow << "Display cords\n";
			std::cout << clr::yellow << " [" << screen2d.x << "] X \n";
			std::cout << clr::yellow << " [" << screen2d.y << "] Y \n";

			std::cout << clr::yellow << "-------------------- \n";
		}
	}
}