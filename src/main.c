#include <raylib.h>
#include <box2d/box2d.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    b2BodyId id;
} Boundary;

typedef struct {
    b2BodyId id;
    b2Vec2 pos;
    float radius;
    b2Vec2 velocity;
    Color color;
} Ball;

typedef struct {
    Vector2 screen;
    b2WorldId worldId;
    Boundary boundary;
    Ball ball;
} Game;

Game game;

void InitGame(void);
void CreateBoundary(void);
void CreateBall(void);
void UpdateDrawFrame(void);
void UpdateBall(void);
void DrawBall(void);

int main(void) {
    // Initialize the game
    InitGame();

    // Create the window
    InitWindow(game.screen.x, game.screen.y, "Pong");

    // Set target frame rate, 
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }

    // Clean-up
    b2DestroyWorld(game.worldId);
    CloseWindow();
    return EXIT_SUCCESS;
}

void InitGame(void) {
    // 
    game.screen = (Vector2) {800.0f, 600.0f};

    // Create Box2D world
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity.y = 0.0f; // No gravity
    game.worldId = b2CreateWorld(&worldDef);

    //
    SetRandomSeed(time(NULL));

    // Create the boundary
    CreateBoundary();
    // Create the ball
    CreateBall();
}

void CreateBoundary()
{
    // Create the body
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = (b2Vec2) {0.0f, 0.0f};
    game.boundary.id = b2CreateBody(game.worldId, &bodyDef);

    // Create the shapes
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    // Top
    b2Segment segment = {(b2Vec2) {0, 0}, (b2Vec2) {game.screen.x, 0}};
    b2CreateSegmentShape(game.boundary.id, &shapeDef, &segment);
    // Bottom
    segment = (b2Segment) {(b2Vec2) {0, game.screen.y}, (b2Vec2) {game.screen.x, game.screen.y}};
    b2CreateSegmentShape(game.boundary.id, &shapeDef, &segment);
    // Left
    segment = (b2Segment) {(b2Vec2) {0, 0}, (b2Vec2) {0, game.screen.y}};
    b2CreateSegmentShape(game.boundary.id, &shapeDef, &segment);
    // Right
    segment = (b2Segment) {(b2Vec2) {game.screen.x, 0}, (b2Vec2) {game.screen.x, game.screen.y}};
    b2CreateSegmentShape(game.boundary.id, &shapeDef, &segment);
}

void CreateBall()
{
    // ball's intial position, radius, velocity and color
    game.ball.pos = (b2Vec2) {game.screen.x / 2.0f, GetRandomValue(10, game.screen.y - 10)};
    game.ball.radius = 10.0f;
    game.ball.velocity = (b2Vec2) {50.0f * (!(GetRandomValue(0, 1024) % 2)? 1.0f: -1.0f), 50.0f * (!(GetRandomValue(0, 1024) % 2)? 1.0f: -1.0f)};
    game.ball.color = GREEN;

    // Create the body
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = game.ball.pos;
    bodyDef.type = b2_dynamicBody;
    game.ball.id = b2CreateBody(game.worldId, &bodyDef);
    b2Body_SetLinearVelocity(game.ball.id, game.ball.velocity);

    // Create the shape
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.restitution = 1.0f;
    shapeDef.friction = 0.0f;
    b2Circle circle = {(b2Vec2) {0.0f, 0.0f}, game.ball.radius};
    b2CreateCircleShape(game.ball.id, &shapeDef, &circle);
}

void UpdateDrawFrame(void) {
    // Update
    b2World_Step(game.worldId, GetFrameTime(), 8);
    UpdateBall();

    // Begin the drawing process
    BeginDrawing();

    // Clear the background with white
    ClearBackground(WHITE);

    DrawLine(0, game.screen.y / 2.0f, game.screen.x, game.screen.y / 2.0f, RED);
    DrawLine(game.screen.x / 2.0f, 0, game.screen.x / 2.0f, game.screen.y, RED);

    // Draw the ball
    DrawBall();

    // End the drawing process
    EndDrawing();
}

void UpdateBall(void) {
    game.ball.pos = b2Body_GetPosition(game.ball.id);
    game.ball.velocity = b2Body_GetLinearVelocity(game.ball.id);
}

void DrawBall(void) {
    DrawCircle(game.ball.pos.x, game.ball.pos.y, game.ball.radius, game.ball.color);
}