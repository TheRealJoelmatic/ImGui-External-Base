#include "../../header Files/functions/render/calculations.h"

Vector3 worldToScreen(const Vector3& pos, const view_matrix_t& matrix, float screenXParam, float screenYParam) {
    float screenW = matrix.matrix[3] * pos.x + matrix.matrix[7] * pos.y + matrix.matrix[11] * pos.z + matrix.matrix[15];

    if (screenW < 0.01f) {
        return Vector3(-99.0f, -99.0f, screenW);
    }

    float screenX = (matrix.matrix[0] * pos.x + matrix.matrix[4] * pos.y + matrix.matrix[8] * pos.z + matrix.matrix[12]) / screenW;
    float screenY = (matrix.matrix[1] * pos.x + matrix.matrix[5] * pos.y + matrix.matrix[9] * pos.z + matrix.matrix[13]) / screenW;

    float camx = screenXParam / 2.0f;
    float camy = screenYParam / 2.0f;

    float X = camx + (screenX * camx);
    float Y = camy - (screenY * camy);

    return Vector3(X, Y, screenW);
}