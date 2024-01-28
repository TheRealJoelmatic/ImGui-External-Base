#include "../../header Files/functions/render/calculations.h"

Vector3 worldToScreen(const Vector3 pos, view_matrix_t matrix, float screenXParam, float screenYParam) {
    float screenW = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3];

    if (screenW < 0.01f) {
        return Vector3(-99.0f, -99.0f, screenW);
    }

    float screenX = (matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3]) / screenW;
    float screenY = (matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3]) / screenW;

    float camx = screenXParam / 2.0f;
    float camy = screenYParam / 2.0f;

    float X = camx + (screenX * camx);
    float Y = camy - (screenY * camy);

    return Vector3(X, Y, screenW);
}