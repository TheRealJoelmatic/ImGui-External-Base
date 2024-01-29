#pragma once

#ifndef UTILS_H
#define UTILS_H

#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../globals.hh"

#include <sstream>
#include <Windows.h>
#include <vector>

struct Notification;

//
// Debug
//

void ShowDebugInfo(const std::string& info);

//
// Console
//

void showConsole();
void hideConsole();
void toggleConsole();

//
// Casting
//

std::string ImVec4ToStr(const ImVec4& vec4);
ImVec4 StrToImVec4(const std::string& str);

//
// Vectors
//

std::vector<DWORD> addOnToVector(const std::vector<DWORD>& list, DWORD newValue);

//
// View	Matrixs
//

bool isViewMatrixEmpty(const view_matrix_t& vm);

#endif