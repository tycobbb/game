#import <raylib/raylib.h>

int main(void) {
    InitWindow(800, 450, "hello world");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("congrats", 190, 200, 10, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
