#ifndef CAM_H
#define CAM_H

#include <raylib/raylib.h>

typedef struct Cam {
  Camera3D camera3d;
} Cam;

// -- lifecycle --
void Cam_Init(Cam *cam);
void Cam_Update(Cam *cam);

static inline Cam Cam_New() {
    Cam cam;
    Cam_Init(&cam);
    return cam;
}

#endif
