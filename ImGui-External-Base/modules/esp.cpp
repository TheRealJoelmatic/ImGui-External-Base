#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>


#include "../header Files/modules/esp.h"
#include "../header Files/globals.hh"
#include "../header Files/functions/render/calculations.h"
#include "../header Files/functions/memory.h"
#include "../header Files/offsets.h"
#include "../header Files/functions/colored_cout.h"
#include "../header Files/functions/utils.h"
#include "../header Files/functions/render/draw.h"
#include "../overlay/renderer/renderer.h"

void espThread() {

	globals.screenX = GetSystemMetrics(SM_CXSCREEN);
	globals.screenY = GetSystemMetrics(SM_CYSCREEN);

	std::cout << clr::green << "[+] ESP Thread Loaded\n";

	while (true) {
		if (!modules.isEspOn) {
			Sleep(100);
			continue;
		}
		renderer::scene::start();
		//std::cout << clr::yellow << "-------------------- \n";

		view_matrix_t vm = readViewMatrix(sdk.processHandle, offsets.veiwMetrix, offsets.nextOffset);
		if (isViewMatrixEmpty(vm)) {
			std::cout << clr::red << "VEIW MATRIX IS EMPTY (Check Your Offsets)" << std::endl;
			continue;
		}

		int localteam;
		std::vector<DWORD> localteamOffsets{ offsets.localPlayer, offsets.team };
		readMemoryFromPointer(sdk.processHandle, sdk.baseAddress, localteamOffsets, &localteam, sizeof(localteam));

		int toalplayers;
		std::vector<DWORD> toalplayersOffsets{ offsets.playersLeft, 0x8 };
		readMemoryFromPointer(sdk.processHandle, sdk.baseAddress, toalplayersOffsets, &toalplayers, sizeof(toalplayers));
		//std::cout << clr::yellow << "toalplayers: " << toalplayers << "\n";

		for (int i = 1; i < toalplayers; i++) {

			std::vector<DWORD> pEnt{ offsets.entityList, (DWORD)i * 0x4 };
			
			char* pname;
			int phealth;
			int pteam;
			float pX, pY, pZ;

			std::vector<DWORD> pEntHealth = addOnToVector(pEnt, offsets.health);
			readMemoryFromPointer(sdk.processHandle, sdk.baseAddress, pEntHealth, & phealth, sizeof(phealth));

			std::vector<DWORD> pEntTeam = addOnToVector(pEnt, offsets.team);
			readMemoryFromPointer(sdk.processHandle, sdk.baseAddress, pEntTeam, &pteam, sizeof(pteam));

			std::vector<DWORD> pEntpname = addOnToVector(pEnt, offsets.name);
			readMemoryFromPointer(sdk.processHandle, sdk.baseAddress, pEntpname, &pname, sizeof(pname));

			std::vector<DWORD> pXoffset = addOnToVector(pEnt, offsets.Xoffset);
			readMemoryFromPointer(sdk.processHandle, sdk.baseAddress, pXoffset, &pX, sizeof(pX));

			std::vector<DWORD> pYoffset = addOnToVector(pEnt, offsets.Yoffset);
			readMemoryFromPointer(sdk.processHandle, sdk.baseAddress, pYoffset, &pY, sizeof(pY));

			std::vector<DWORD> pZoffset = addOnToVector(pEnt, offsets.Zoffset);
			readMemoryFromPointer(sdk.processHandle, sdk.baseAddress, pZoffset, &pZ, sizeof(pZ));

			Vector3 pPosition = { pZ, pX, pY};
			Vector3 topPosition = { pZ, pX + 4, pY + 50 };

			Vector3 screen2d = worldToScreen(pPosition, vm, globals.screenX, globals.screenY);
			Vector3 topScreen2d = worldToScreen(topPosition, vm, globals.screenX, globals.screenY);

			float height = 200;
			float width = height / 2.4f;
			if (!(phealth > 0 && phealth < 101)) {
				continue;
			}


			if (pteam != localteam) {
				if (!modules.showEnemys) {
					continue;
				}
				
				if (modules.showHealth) renderer::scene::text(Vector2(screen2d.x, screen2d.y), std::to_wstring(phealth).c_str(), renderer::colors::red_color, renderer::fonts::tahoma_font);
				if (modules.showBoxs) renderer::scene::box(screen2d.x, screen2d.y, (screen2d.z - topScreen2d.z), (screen2d.y - topScreen2d.y), 2, renderer::colors::red_color);
			}
			else
			{
				if (!modules.showFriendlys) {
					continue;
				}

				if (modules.showHealth) renderer::scene::text(Vector2(screen2d.x, screen2d.y), std::to_wstring(phealth).c_str(), renderer::colors::green_color, renderer::fonts::tahoma_font);
			}

			/*
				std::cout << clr::yellow << "-------------------- \n";
				std::cout << clr::yellow << "Player Info" << "\n";
				std::cout << clr::yellow << " [" << i << "] Player " << pname << "\n";
				std::cout << clr::yellow << " [" << (float)phealth << "] health \n";
				std::cout << clr::yellow << " [" << (float)pteam << "]  team\n\n";

				std::cout << clr::yellow << "Player Cords\n";

				std::cout << clr::yellow << " [" << pX << "] X \n";
				std::cout << clr::yellow << " [" << pY << "] Y \n";
				std::cout << clr::yellow << " [" << pZ << "] Z\n\n";

				std::cout << clr::yellow << "Display cords\n";
				std::cout << clr::yellow << " [" << screen2d.x << "] X \n";
				std::cout << clr::yellow << " [" << screen2d.y << "] Y \n";
				std::cout << clr::yellow << " [" << screen2d.z << "] Z \n";

				std::cout << clr::yellow << "-------------------- \n";
			*/
		}
		renderer::scene::end();
	}
}