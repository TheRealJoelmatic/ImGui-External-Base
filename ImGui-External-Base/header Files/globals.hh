#pragma once

#ifndef GLOB_H
#define GLOB_H

#include <Windows.h>
#include <string>

class c_globals {
	public:
		bool active = true;

		int page = 0;
		int subPage = 0;

		std::string gameName = "assaultcube";
		std::string processName = "ac_client.exe";

		int screenX;
		int screenY;

};

class color_settings
{
	public:
		float all[4] = { 0.20f, 0.20f, 0.20f, 255 };
};

class c_modules {
public:
	bool isEspOn = false;
	bool isCrosshairOn = false;

	bool showHealth = false;
	bool showBoxs = false;

	bool showFriendlys = false;
	bool showEnemys = false;
};

//
//	structs
//

struct Vector3
{
	float x, y, z;
};
struct Vector2
{
	float x, y;
};
struct view_matrix_t {
	float matrix[16];
};


//
//	SDK
//

class c_sdk {
public:
	DWORD baseAddress;
	HANDLE processHandle;
	HWND hwnd;
	DWORD process_id;
};


inline c_globals globals;
inline color_settings color;
inline c_modules modules;
inline c_sdk sdk;

#endif