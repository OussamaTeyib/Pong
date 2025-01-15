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

// prototypes
void InitGame(void);
void CreateBoundary(void);
void CreateBall(void);
void UpdateDrawFrame(void);
void UpdateGame(void);
void DrawBall(void);

int main(void) {
    // Initialize the game
    InitGame();

    // Create the window
    InitWindow(game.screen.x, game.screen.y, "Pong");

    // Set the frame rate, 
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        // Update physics and draw the frame
        UpdateDrawFrame();
    }

    // Clean-up
    b2DestroyWorld(game.worldId);
    game.worldId = b2_nullWorldId;
    CloseWindow();
    return EXIT_SUCCESS;
}

// Initialize game components
void InitGame(void) {
    // Set screen size
    game.screen = (Vector2) {800.0f, 600.0f};

    // Create the Box2D world
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity.y = 0.0f; // No gravity needed
    game.worldId = b2CreateWorld(&worldDef);

    // Seed the random number generator
    SetRandomSeed(time(NULL));

    // Create the boundary walls and the ball
    CreateBoundary();
    CreateBall();
}

// Create the boundary around the screen
void CreateBoundary() {
    // Create a static body for the boundary
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = (b2Vec2) {0.0f, 0.0f};
    game.boundary.id = b2CreateBody(game.worldId, &bodyDef);

    // Create shapes for the boundary
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    // Top boundary
    b2Segment segment = {(b2Vec2) {0, 0}, (b2Vec2) {game.screen.x, 0}};
    b2CreateSegmentShape(game.boundary.id, &shapeDef, &segment);
    // Bottom boundary
    segment = (b2Segment) {(b2Vec2) {0, game.screen.y}, (b2Vec2) {game.screen.x, game.screen.y}};
    b2CreateSegmentShape(game.boundary.id, &shapeDef, &segment);
    // Left boundary
    segment = (b2Segment) {(b2Vec2) {0, 0}, (b2Vec2) {0, game.screen.y}};
    b2CreateSegmentShape(game.boundary.id, &shapeDef, &segment);
    // Right boundary
    segment = (b2Segment) {(b2Vec2) {game.screen.x, 0}, (b2Vec2) {game.screen.x, game.screen.y}};
    b2CreateSegmentShape(game.boundary.id, &shapeDef, &segment);
}

// Create the ball
void CreateBall() {
    // Set the ball's initial position, radius, initial velocity and color
    game.ball.pos = (b2Vec2) {game.screen.x / 2.0f, GetRandomValue(10, game.screen.y - 10)};
    game.ball.radius = 10.0f;
    game.ball.velocity = (b2Vec2) {50.0f * (!(GetRandomValue(0, 1024) % 2)? 1.0f: -1.0f), 50.0f * (!(GetRandomValue(0, 1024) % 2)? 1.0f: -1.0f)};
    game.ball.color = GREEN;

    // Create a dynamic body for the ball
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = game.ball.pos;
    bodyDef.type = b2_dynamicBody;
    game.ball.id = b2CreateBody(game.worldId, &bodyDef);
    b2Body_SetLinearVelocity(game.ball.id, game.ball.velocity);

    // Create a circular shape for the ball
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.restitution = 1.0f; // Perfectly elastic collition
    shapeDef.friction = 0.0f; // No friction needed
    b2Circle circle = {(b2Vec2) {0.0f, 0.0f}, game.ball.radius};
    b2CreateCircleShape(game.ball.id, &shapeDef, &circle);
}

// Main update and render function
void UpdateDrawFrame(void) {
    // Update the game state
    UpdateGame();

    // Begin the drawing process
    BeginDrawing();

    // Clear the background with white
    ClearBackground(WHITE);

    // Draw the ball
    DrawBall();

    // End the drawing process
    EndDrawing();
}

// Update the game state
void UpdateGame(void) {
    // Advance the physics simulation by one step
    b2World_Step(game.worldId, GetFrameTime(), 4);

    // Retrieve body movement events from the physics world
    b2BodyEvents events = b2World_GetBodyEvents(game.worldId);

    // Iterate through all bodies that moved during this step 
    for (int i = 0; i < events.moveCount; i++) {
        b2BodyMoveEvent *event = events.moveEvents + i;

        // Check if the moved body is the ball
        if (B2_ID_EQUALS(event->bodyId, game.ball.id)) {
            // Update the ball's position and velocity
            game.ball.pos = event->transform.p;
            game.ball.velocity = b2Body_GetLinearVelocity(game.ball.id);
        }
    }
}

// Draw the ball on the screen
void DrawBall(void) {
    DrawCircle(game.ball.pos.x, game.ball.pos.y, game.ball.radius, game.ball.color);
}