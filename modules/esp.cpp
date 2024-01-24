#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

#include "../header Files/modules/esp.h"
#include "../header Files/functions/memory.h"
#include "../header Files/globals.hh"
#include "../header Files/offsets.h"
#include "../header Files/functions/colored_cout.h"

c_globals Globals_ESP;

HANDLE processHandle;
DWORD baseAddress;
HDC hdc;

int screenX = GetSystemMetrics(SM_CXSCREEN);
int screenY = GetSystemMetrics(SM_CYSCREEN);

#define EnemyPen 0x000000FF
HBRUSH EnemyBrush = CreateSolidBrush(0x000000FF);


bool setUpESP() {
	processHandle = GetProcessHandleByName(Globals_ESP.processName);
	baseAddress = getBaseAddress(processHandle, Globals_ESP.processName);
	hdc = GetDC(FindWindowA(NULL, Globals_ESP.processName.c_str()));
	return true;
}

void espThread() {

	setUpESP();
	std::cout << clr::yellow << "screenX: " << screenX << "\n";
	std::cout << clr::yellow << "screenY: " << screenY << "\n";

	std::cout << clr::yellow << "-------------------- \n";

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

		view_matrix_t vm;
		readMemory(processHandle, baseAddress + offsets.veiwMetrix, &vm, sizeof(view_matrix_t));

		std::cout << "View Matrix:" << std::endl;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				std::cout << vm[i * 4 + j] << " ";
			}
			std::cout << std::endl;
		}

		int localteam;
		std::vector<DWORD> localteamOffsets{ offsets.localPlayer, offsets.team };
		readMemoryFromPointer(processHandle, baseAddress, localteamOffsets, &localteam, sizeof(localteam));
		std::cout << clr::yellow << "localteam: " << localteam << "\n";

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

			Vector3 screenpos = worldToScreen(pPosition, vm);

			float height = 200;
			float width = height / 2.4f;

			if (screenpos.z >= 0.01f && pteam != localteam && phealth > 0 && phealth < 101) {
				//DrawBorderBox(screenpos.x - (width / 2), screenpos.y, width, height, 1);
				//DrawLine(screenX / 2, screenY, screenpos.x, screenpos.y);
				drawDot(static_cast<int>(screenpos.x), static_cast<int>(screenpos.y), 10, RGB(255, 0, 0));
			}

			std::cout << clr::yellow << "-------------------- \n";
			std::cout << clr::yellow << " [" << i << "] Player " << "\n";
			std::cout << clr::yellow << " [" << (float)phealth << "] health \n";
			std::cout << clr::yellow << " [" << (float)pteam << "]  team\n";

			std::cout << clr::yellow << " [" << pX << "] X \n";
			std::cout << clr::yellow << " [" << pY << "] Y \n";
			std::cout << clr::yellow << " [" << pZ << "] Z\n";
			std::cout << clr::yellow << "-------------------- \n";
		}
		Sleep(100);
	}
}

//
//		Cacluations
//

std::vector<DWORD> addOnToVector(const std::vector<DWORD>& list, DWORD newValue) {
	std::vector<DWORD> result = list;
	result.push_back(newValue);
	return result;
}

//
//		Draw Stuff
//

Vector3 worldToScreen(const Vector3 pos, view_matrix_t matrix) {
	float _x = matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3];
	float _y = matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3];

	float w = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3];

	float inv_w = 1.f / w;
	_x *= inv_w;
	_y *= inv_w;

	float x = screenX * .5f;
	float y = screenY * .5f;

	x += 0.5f * _x * screenX + 0.5f;
	y -= 0.5f * _y * screenY + 0.5f;

	return { x,y,w };
}

void DrawFilledRect(int x, int y, int w, int h)
{
	RECT rect = { x, y, x + w, y + h };
	FillRect(hdc, &rect, EnemyBrush);
}

void DrawBorderBox(int x, int y, int w, int h, int thickness)
{
	DrawFilledRect(x, y, w, thickness); //Top horiz line
	DrawFilledRect(x, y, thickness, h); //Left vertical line
	DrawFilledRect((x + w), y, thickness, h); //right vertical line
	DrawFilledRect(x, y + h, w + thickness, thickness); //bottom horiz line
}

void DrawLine(float StartX, float StartY, float EndX, float EndY)
{
	int a, b = 0;
	HPEN hOPen;
	HPEN hNPen = CreatePen(PS_SOLID, 2, EnemyPen);// penstyle, width, color
	hOPen = (HPEN)SelectObject(hdc, hNPen);
	MoveToEx(hdc, StartX, StartY, NULL); //start
	a = LineTo(hdc, EndX, EndY); //end
	DeleteObject(SelectObject(hdc, hOPen));
}

void drawDot(int x, int y, int size, COLORREF color) {
	// Calculate the coordinates of the bounding rectangle for the ellipse
	int left = x - size / 2;
	int top = y - size / 2;
	int right = x + size / 2;
	int bottom = y + size / 2;

	// Draw a filled ellipse (circle) at the specified coordinates
	HBRUSH hBrush = CreateSolidBrush(color);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Ellipse(hdc, left, top, right, bottom);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);
}
