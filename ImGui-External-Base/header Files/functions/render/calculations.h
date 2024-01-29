#pragma once

#ifndef CAC_H
#define CAC_H

#include <Windows.h>
#include <vector>
#include "../../globals.hh"

Vector3 worldToScreen(const Vector3& pos, const view_matrix_t& matrix, float screenXParam, float screenYParam);

#endif