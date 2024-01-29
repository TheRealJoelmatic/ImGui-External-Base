#include "../header Files/menu.hh"
#include "../header Files/functions/utils.h"
#include "../header Files/globals.hh"

//
// Debug
//

void ShowDebugInfo(const std::string& info) {
    MessageBox(NULL, info.c_str(), "Information", MB_OK | MB_ICONINFORMATION);
}

//
// Console
//

void hideConsole() {
    HWND console = GetConsoleWindow();
    ShowWindow(console, SW_HIDE);
}

void showConsole() {
    HWND console = GetConsoleWindow();
    ShowWindow(console, SW_SHOW);
	SetWindowPos(console, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void toggleConsole() {
	HWND console = GetConsoleWindow();
	if (IsWindowVisible(console)) {
		ShowWindow(console, SW_HIDE);
	}
	else {
		ShowWindow(console, SW_SHOW);
	}
}

//
// Casting
//

std::string ImVec4ToStr(const ImVec4& vec4) {
	std::stringstream ss;
	ss << "{" << vec4.x << ", " << vec4.y << ", " << vec4.z << ", " << vec4.w << "}";
	return ss.str();
}
ImVec4 StrToImVec4(const std::string& str) {
	ImVec4 vec4;
	std::stringstream ss(str);
	char ch; // to store the comma between values

	// Assuming the format is "{x, y, z, w}"
	ss >> ch >> vec4.x >> ch >> vec4.y >> ch >> vec4.z >> ch >> vec4.w >> ch;

	return vec4;
}

//
// Vectors
//

std::vector<DWORD> addOnToVector(const std::vector<DWORD>& list, DWORD newValue) {
	std::vector<DWORD> result = list;
	result.push_back(newValue);
	return result;
}

//
// ViewMatrix
//

bool isViewMatrixEmpty(const view_matrix_t& vm) {
	// Check if all elements are close to zero (within a tolerance)
	const float tolerance = 0.0001f;

	for (int i = 0; i < 16; ++i) {
		if (std::abs(vm.matrix[i]) > tolerance) {
			return false;
		}
	}

	return true;
}