#pragma once
#include <Windows.h>
#include <vector>

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

bool setUpESP();
void espThread();

std::vector<DWORD> addOnToVector(const std::vector<DWORD>& list, DWORD newValue);

Vector3 worldToScreen(const Vector3 pos, view_matrix_t matrix);
void DrawFilledRect(int x, int y, int w, int h);
void DrawBorderBox(int x, int y, int w, int h, int thickness);
void DrawLine(float StartX, float StartY, float EndX, float EndY);
void drawDot(int x, int y, int size, COLORREF color);

Vector3 worldToScreen(const Vector3 pos, view_matrix_t matrix);