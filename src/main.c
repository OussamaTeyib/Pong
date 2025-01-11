#include <raylib.h>
#include <stdlib.h>

typedef struct {
    Vector2 pos;
    float radius;
    Vector2 velocity;
    Color color;
} Ball;

typedef struct {
    Rectangle rect;
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
    game.screen = (Vector2) {800.0f, 600.0f};

    InitWindow(game.screen.x, game.screen.y, "Pong");

    game.bgColor = WHITE;
    game.halflineColor = GREEN;

    game.ball = (Ball) {.pos.x = game.screen.x / 2.0f, .pos.y = GetRandomValue(10, game.screen.y - 10), .radius = 10.0f, .velocity.x = 5.0f * (!(GetRandomValue(0, 1024) % 2)? 1.0f: -1.0f), .velocity.y = 5.0f * (!(GetRandomValue(0, 1024) % 2)? 1.0f: -1.0f), .color = GREEN};

    game.paddles[0] = (Paddle) {.rect.x = 0.0f, .rect.y = (game.screen.y - game.screen.y / 6.0f) / 2.0f, .rect.width = 10.0f, .rect.height = game.screen.y / 6.0f, .velocity = 10.0f, .color = GREEN};
    game.paddles[1] = (Paddle) {.rect.x = game.screen.x - 10.0f, .rect.y = (game.screen.y - game.screen.y / 6.0f) / 2.0f, .rect.width = 10.0f, .rect.height = game.screen.y / 6.0f, .velocity = 10.0f, .color = GREEN};

    game.score = (Score) {.p1 = 0, .p2 = 0, .size = 30, .color = GREEN};

    // Set target frame rate, 
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }

    // Clean-up
    CloseWindow();
    return EXIT_SUCCESS;
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
    for (float i = 0.0f; i <= game.screen.y; i += game.screen.y / 20.0f) {
        DrawLine(game.screen.x / 2.0f, i, game.screen.x / 2.0f, i + game.screen.y / 40.0f, game.halflineColor);
    }
}

void MovePaddles(void) {
    if (IsKeyDown(KEY_W))
        game.paddles[0].rect.y -= game.paddles[0].velocity;
    if (IsKeyDown(KEY_S ))
        game.paddles[0].rect.y += game.paddles[0].velocity;

    if (IsKeyDown(KEY_UP))
        game.paddles[1].rect.y -= game.paddles[1].velocity;
    if (IsKeyDown(KEY_DOWN))
        game.paddles[1].rect.y += game.paddles[1].velocity;

    for (int i = 0; i < 2; i++) {
        if (game.paddles[i].rect.y < 0.0f) game.paddles[i].rect.y = 0.0f;
        if (game.paddles[i].rect.y > (game.screen.y - game.paddles[i].rect.height)) game.paddles[i].rect.y = game.screen.y - game.paddles[i].rect.height;
    }
}

void DrawPaddles(void) {
    for (int i = 0; i < 2; i++)
        DrawRectangleRec(game.paddles[i].rect, game.paddles[i].color);
}

void MoveBall(void) {
    game.ball.pos.x += game.ball.velocity.x;
    game.ball.pos.y += game.ball.velocity.y;

    if (game.ball.pos.x < game.ball.radius) game.ball.pos.x = game.ball.radius;
    if (game.ball.pos.x > game.screen.x - game.ball.radius) game.ball.pos.x = game.screen.x - game.ball.radius;

    if (game.ball.pos.y < game.ball.radius) game.ball.pos.y = game.ball.radius;
    if (game.ball.pos.y > game.screen.y - game.ball.radius) game.ball.pos.y = game.screen.y - game.ball.radius;

    if ((game.ball.pos.y + game.ball.radius) >= game.screen.y || (game.ball.pos.y - game.ball.radius) <= 0.0f)
        game.ball.velocity.y *= -1.0f;

    if (CheckCollisionCircleRec(game.ball.pos, game.ball.radius, game.paddles[0].rect) || CheckCollisionCircleRec(game.ball.pos, game.ball.radius, game.paddles[1].rect)) {
        game.ball.velocity.x *= -1.0f;
    }

    if ((game.ball.pos.x - game.ball.radius) <= 0.0f) {
        game.score.p2++;
        game.ball.pos.x = game.screen.x / 2.0f;
        game.ball.pos.y = GetRandomValue(game.ball.radius, game.screen.y - game.ball.radius);
        game.ball.velocity.y *= !(GetRandomValue(0, 1024) % 2)? 1.0f: -1.0f ;
    }

    if ((game.ball.pos.x + game.ball.radius) >= game.screen.x) {
        game.score.p1++;
        game.ball.pos.x = game.screen.x / 2.0f;
        game.ball.pos.y = GetRandomValue(game.ball.radius, game.screen.y - game.ball.radius);
        game.ball.velocity.y *= !(GetRandomValue(0, 1024) % 2)? 1.0f: -1.0f;
    }
}

void DrawBall(void) {
    DrawCircleV(game.ball.pos, game.ball.radius, game.ball.color);
}

void DrawScore(void) {
    const char *text = TextFormat("%d %d", game.score.p1, game.score.p2);
    int width = MeasureText(text, game.score.size);
    DrawText(text, (game.screen.x - width) / 2.0f, 0.0f, game.score.size, game.score.color);
}