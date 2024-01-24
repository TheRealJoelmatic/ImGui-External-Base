#pragma once

#ifndef MENU_H
#define MENU_H

#include <d3d9.h>
#include "imgui/imgui.h"

static float xCord;
static float yCord;
static float zCord;

void updateCords(float _xCord, float _yCord, float _zCord);
void ApplyCustomColors(const ImVec4& color, ImGuiCol_ whatToChange);
void ApplyRoundedCorners(float radius);

void setupImgui();

namespace ui {
	void init(LPDIRECT3DDEVICE9);
	void renderMenu();
	void RunStyle();
}

namespace ui {
	inline LPDIRECT3DDEVICE9 dev;
}

namespace ui {
	const char window_title[255] = "Trainer Made by @Joelmatic";
	inline ImVec2 screen_res{ 000, 000 };
	inline ImVec2 window_pos{ 0, 0 };
	inline ImVec2 window_size{ 600, 400 };
	inline DWORD  window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
}

#endif