#include <raylib/raylib.h>
#include <raylib/raymath.h>
#include <raylib/rcamera.h>
#include "cam.h"

// -- lifetime --
void Cam_Init(Cam *cam) {
    Camera3D camera3d = {
        .position = (Vector3){ 0.0f, 10.0f, 10.0f },
        .target = (Vector3){ 0.0f, 0.0f, 0.0f },
        .up = (Vector3){ 0.0f, 1.0f, 0.0f },
        .fovy = 45.0f,
        .projection = CAMERA_PERSPECTIVE,
    };

    *cam = (Cam){
        .camera3d = camera3d
    };
}

void Cam_Update(Cam *cam) {
    Camera3D *inner = &(cam->camera3d);

    // -- update --
    Vector3 dir = Vector3Zero();
    if (IsKeyDown('W')) {
        dir.z += 1.0f;
    }

    if (IsKeyDown('S')) {
        dir.z -= 1.0f;
    }

    if (IsKeyDown('A')) {
        dir.x -= 1.0f;
    }

    if (IsKeyDown('D')) {
        dir.x += 1.0f;
    }

    if (IsKeyDown('Q')) {
        dir.y -= 1.0f;
    }

    if (IsKeyDown('E')) {
        dir.y += 1.0f;
    }

    if (dir.x != 0.0f) {
        CameraMoveRight(inner, dir.x * 0.01f, false);
    }

    if (dir.y != 0.0f) {
        CameraMoveUp(inner, dir.y * 0.01f);
    }

    if (dir.z != 0.0f) {
        CameraMoveForward(inner, dir.z * 0.01f, false);
    }

    dir = Vector3Zero();

    if (IsKeyDown('I')) {
        dir.y += 1.0f;
    }

    if (IsKeyDown('K')) {
        dir.y -= 1.0f;
    }

    if (IsKeyDown('J')) {
        dir.x += 1.0f;
    }

    if (IsKeyDown('L')) {
        dir.x -= 1.0f;
    }

    if (dir.x != 0.0f) {
        CameraYaw(inner, dir.x * 0.01f, false);
    }

    if (dir.y != 0.0f) {
        CameraPitch(inner, dir.y * 0.01f, true, false, false);
    }
}