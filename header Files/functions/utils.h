#pragma once

#ifndef UTILS_H
#define UTILS_H

#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

#include <sstream>

struct Notification;

void ShowDebugInfo(const std::string& info);

void showConsole();
void hideConsole();
void toggleConsole();

std::string ImVec4ToStr(const ImVec4& vec4);
ImVec4 StrToImVec4(const std::string& str);

#endif