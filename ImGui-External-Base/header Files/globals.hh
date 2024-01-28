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
};

class color_settings
{
	public:
		float all[4] = { 0.20f, 0.20f, 0.20f, 255 };
};

class c_modules {
public:
	bool isEspOn = false;
};

inline c_globals globals;
inline color_settings color;
inline c_modules modules;


//
//	structs
//

struct Vector3
{
	float x, y, z;
};

struct view_matrix_t {
	float* operator[](int index) {
		return matrix[index];
	}

	float matrix[4][4];
};

#endif