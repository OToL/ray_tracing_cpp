#include <raylib.h>

int main() {

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raytracing");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Hello Raylib", 20, 20, 50, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
