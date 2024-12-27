#include <raylib.h>

const int screenWidth = 800;
const int screenHeight = 600;

typedef struct {
    float x, y;
    float radius;
    int velocity;
} Ball;

Rectangle paddles[2];

Ball ball;

void UpdateDrawFrame(void);

int main(void) {
    InitWindow(screenWidth, screenHeight, "Pong");

    ball = (Ball) {screenWidth / 2, screenHeight / 2, 10.0f, 5.0f};

    paddles[0] = (Rectangle) {0.0f, (screenHeight - screenHeight / 8) / 2, 10.0f, screenHeight / 8};
    paddles[1] = (Rectangle) {screenWidth - 10.0f, (screenHeight - screenHeight / 8) / 2, 10.0f, screenHeight / 8};

    // Set target frame rate, 
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

        // Draw the ball
        DrawCircle(ball.x, ball.y, ball.radius, GREEN);

        // Draw the paddles
        for (int i = 0; i < 2; i++)
            DrawRectangle(paddles[i].x, paddles[i].y, paddles[i].width, paddles[i].height, GREEN);

        // End the drawing process
        EndDrawing();
}