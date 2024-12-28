#include <raylib.h>

typedef struct {
    float x, y;
    float radius;
    float x_velocity, y_velocity;
    Color color;
} Ball;

typedef struct {
    float x, y;
    float width, height;
    float velocity;
    Color color;
} Paddle;

typedef struct {
    int p1, p2;
    int size;
    Color color;
} Score;

typedef struct {
    Vector2 screen;
    Color bgColor;
    Color halflineColor;
    Ball ball;
    Paddle paddles[2];
    Score score;
} Game;

Game game;

void UpdateDrawFrame(void);
void DrawHalfline(void);
void MovePaddles(void);
void DrawPaddles(void);
void MoveBall(void);
void DrawBall(void);
void DrawScore(void);

int main(void) {
    game.screen = (Vector2) {800, 600};

    InitWindow(game.screen.x, game.screen.y, "Pong");

    game.bgColor = WHITE;
    game.halflineColor = GREEN;

    game.ball = (Ball) {.x = game.screen.x / 2, .y = GetRandomValue(10, game.screen.y - 10), .radius = 10.0f, .x_velocity = 5.0f * (!(GetRandomValue(0, 1024) % 2)? 1: -1), .y_velocity = 5.0f * (!(GetRandomValue(0, 1024) % 2)? 1: -1), .color = GREEN};

    game.paddles[0] = (Paddle) {.x = 0.0f, .y = (game.screen.y - game.screen.y / 6) / 2, .width = 10.0f, .height = game.screen.y / 6, .velocity = 10.0f, .color = GREEN};
    game.paddles[1] = (Paddle) {.x = game.screen.x - 10.0f, .y = (game.screen.y - game.screen.y / 6) / 2, .width = 10.0f, .height = game.screen.y / 6, .velocity = 10.0f, .color = GREEN};

    game.score = (Score) {.p1 = 0, .p2 = 0, .size = 30, .color = GREEN};

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
    MovePaddles();
    MoveBall();

    // Begin the drawing process
    BeginDrawing();

    // Clear the background with white
    ClearBackground(WHITE);

    // Draw the hafline
    DrawHalfline();

    // Draw the paddles
    DrawPaddles();

    // Draw the ball
    DrawBall();

    // Show the score
    DrawScore();
 
    // End the drawing process
    EndDrawing();
}

void DrawHalfline(void) {
    for (int i = 0; i <= game.screen.y; i += game.screen.y / 20) {
        DrawLine(game.screen.x / 2, i, game.screen.x / 2, i + game.screen.y / 40, game.halflineColor);
    }
}

void MovePaddles(void) {
    if (IsKeyDown(KEY_W))
        game.paddles[0].y -= game.paddles[0].velocity;
    if (IsKeyDown(KEY_S ))
        game.paddles[0].y += game.paddles[0].velocity;

    if (IsKeyDown(KEY_UP))
        game.paddles[1].y -= game.paddles[1].velocity;
    if (IsKeyDown(KEY_DOWN))
        game.paddles[1].y += game.paddles[1].velocity;

    for (int i = 0; i < 2; i++) {
        if (game.paddles[i].y < 0) game.paddles[i].y = 0.0f;
        if (game.paddles[i].y > (game.screen.y - game.paddles[i].height)) game.paddles[i].y = game.screen.y - game.paddles[i].height;
    }
}

void DrawPaddles(void) {
    for (int i = 0; i < 2; i++)
        DrawRectangle(game.paddles[i].x, game.paddles[i].y, game.paddles[i].width, game.paddles[i].height, game.paddles[i].color);
}

void MoveBall(void) {
    game.ball.x += game.ball.x_velocity;
    game.ball.y += game.ball.y_velocity;

    if (game.ball.x < game.ball.radius) game.ball.x = game.ball.radius;
    if (game.ball.x > game.screen.x - game.ball.radius) game.ball.x = game.screen.x - game.ball.radius;

    if (game.ball.y < game.ball.radius) game.ball.y = game.ball.radius;
    if (game.ball.y > game.screen.y - game.ball.radius) game.ball.y = game.screen.y - game.ball.radius;

    if ((game.ball.y + game.ball.radius) >= game.screen.y || (game.ball.y - game.ball.radius) <= 0.0f)
        game.ball.y_velocity *= -1;

    if (CheckCollisionCircleRec((Vector2) {game.ball.x, game.ball.y}, game.ball.radius, (Rectangle) {game.paddles[0].x, game.paddles[0].y, game.paddles[0].width, game.paddles[0].height}) || CheckCollisionCircleRec((Vector2) {game.ball.x, game.ball.y}, game.ball.radius, (Rectangle) {game.paddles[1].x, game.paddles[1].y, game.paddles[1].width, game.paddles[1].height})) {
        game.ball.x_velocity *= -1;
    }

    if ((game.ball.x - game.ball.radius) <= 0.0f) {
        game.score.p2++;
        game.ball.x = game.screen.x / 2;
        game.ball.y = GetRandomValue(game.ball.radius, game.screen.y - game.ball.radius);
        game.ball.y_velocity *= !(GetRandomValue(0, 1024) % 2)? 1: -1;
    }

    if ((game.ball.x + game.ball.radius) >= game.screen.x) {
        game.score.p1++;
        game.ball.x = game.screen.x / 2;
        game.ball.y = GetRandomValue(game.ball.radius, game.screen.y - game.ball.radius);
        game.ball.y_velocity *= !(GetRandomValue(0, 1024) % 2)? 1: -1;
    }
}

void DrawBall(void) {
    DrawCircle(game.ball.x, game.ball.y, game.ball.radius, game.ball.color);
}

void DrawScore(void) {
    const char *text = TextFormat("%d %d", game.score.p1, game.score.p2);
    int width = MeasureText(text, game.score.size);
    DrawText(text, (game.screen.x - width) / 2, 0.0f, game.score.size, game.score.color);
}