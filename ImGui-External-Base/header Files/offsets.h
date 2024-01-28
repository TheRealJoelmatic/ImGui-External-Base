#pragma once

#ifndef OFFSETS_H
#define OFFSETS_H
#include <Windows.h>

class c_offsets {
public:
	DWORD

	veiwMetrix = 0x501AE8,

	playersLeft = 0x0005CDB4,

	localPlayer = 0x0018AC00,
	entityList = 0x0018AC04,

	//
	// offsets from entity class
	//

	headPos = 0x4,
	feetPos = 0x28,
	veiwAnglesX = 0x34,
	veiwAnglesY = 0x38,
	name = 0x205,
	team = 0x030c,
	dead = 0x0338,

	Xoffset = 0x2C,
	Yoffset = 0x30,
	Zoffset = 0x28,

	ammo = 0x140,
	health = 0xEC,

	//
	// Position Offsets
	//

	yPointer = 0x17E0A8,
	yoffset1 = 0x30;

	std::vector<DWORD> xPointsOffsets = { yPointer, yoffset1 - 0x4 };
	std::vector<DWORD> yPointsOffsets = { yPointer, yoffset1 };
	std::vector<DWORD> zPointsOffsets = { yPointer, yoffset1 - 0x8 };
};

inline c_offsets offsets;
#endif