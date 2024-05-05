#include <raylib/raylib.h>
#include "cam.h"
#include "render.h"

// -- main --
int main(void) {
    const int kScreenWidth = 800;
    const int kScreenHeight = 450;

    InitWindow(
        kScreenWidth,
        kScreenHeight,
        "hello world"
    );

    Cam cam = Cam_New();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Cam_Update(&cam);

        // -- render --
        Render(&cam);
    }

    CloseWindow();

    return 0;
}
