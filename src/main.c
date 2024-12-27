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
void MovePaddle(int id, int direction);

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
    // Update
    if (IsKeyDown(KEY_S))
        MovePaddle(0, -1);
    if (IsKeyDown(KEY_Z))
        MovePaddle(0, 1);
    if (IsKeyDown(KEY_UP))
        MovePaddle(1, -1);
    if (IsKeyDown(KEY_DOWN))
        MovePaddle(1, 1);

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

void MovePaddle(int id, int direction) {
    paddles[id].y += 10.0f * direction;

    if (paddles[id].y < 0) paddles[id].y = 0;
    if (paddles[id].y > (screenHeight - paddles[id].height)) paddles[id].y = screenHeight - paddles[id].height;
}