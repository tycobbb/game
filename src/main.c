#include <raylib/raylib.h>
#include "draw.h"

// -- main --
int main(void) {
    const int kScreenWidth = 800;
    const int kScreenHeight = 450;

    InitWindow(
        kScreenWidth,
        kScreenHeight,
        "hello world"
    );

    Camera camera = {
        .position = (Vector3){ 0.0f, 10.0f, 10.0f },
        .target = (Vector3){ 0.0f, 0.0f, 0.0f },
        .up = (Vector3){ 0.0f, 1.0f, 0.0f },
        .fovy = 45.0f,
        .projection = CAMERA_PERSPECTIVE,
    };

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Draw(camera);
    }

    CloseWindow();

    return 0;
}
