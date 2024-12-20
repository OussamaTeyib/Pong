#include <raylib.h>

void UpdateDrawFrame(void);

int main(void) {
    InitWindow(800, 600, "Pong");

    // Set target frame rate
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }

    // Clean-up
    CloseWindow();
    return 0;
}

void UpdateDrawFrame(void) {
        // Begin the drawing process
        BeginDrawing();

        // Clear the background with white
        ClearBackground(WHITE);

        // End the drawing process
        EndDrawing();
}