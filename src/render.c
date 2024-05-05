#include <raylib/raylib.h>
#include "render.h"

// -- commands --
void Render(Cam *camera) {
    BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera->camera3d);
            DrawCube((Vector3){-4.0f, 0.0f, 2.0f}, 2.0f, 5.0f, 2.0f, RED);
            DrawCubeWires((Vector3){-4.0f, 0.0f, 2.0f}, 2.0f, 5.0f, 2.0f, GOLD);

            DrawSphere((Vector3){1.0f, 0.0f, -2.0f}, 1.0f, GREEN);
            DrawSphereWires((Vector3){1.0f, 0.0f, -2.0f}, 1.0f, 16, 16, LIME);

            DrawCapsule((Vector3){-3.0f, 1.5f, -4.0f}, (Vector3){-4.0f, -1.0f, -4.0f}, 1.2f, 8, 8, VIOLET);
            DrawCapsuleWires((Vector3){-3.0f, 1.5f, -4.0f}, (Vector3){-4.0f, -1.0f, -4.0f}, 1.2f, 8, 8, PURPLE);
        EndMode3D();

        DrawFPS(10, 10);
    EndDrawing();
}
