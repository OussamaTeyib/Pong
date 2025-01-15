#include <raylib.h>
#include <box2d/box2d.h>
#include <stdlib.h>
#include <time.h>

// Structure to represent the boundary walls
typedef struct {
    b2BodyId bodyId;
    b2ShapeId topShapeId, bottomShapeId, leftShapeId, rightShapeId;
} Boundary;

// Structure to represent the ball
typedef struct {
    b2BodyId bodyId;
    b2ShapeId shapeId;
    b2Vec2 pos;
    float radius;
    b2Vec2 velocity;
    Color color;
} Ball;

// Structure to represent a paddle
typedef struct {
    b2BodyId bodyId;
    b2ShapeId shapeId;
    b2Vec2 pos;
    b2Vec2 size;
    float velocity;
    Color color;
} Paddle;

// Structure to hold the game state
typedef struct {
    Vector2 screen;
    b2WorldId worldId;
    Boundary boundary;
    Ball ball;
    Paddle paddle_1, paddle_2;
} Game;

// Global game object
Game game;

// Function prototypes
void InitGame(void);
void CreateBoundary(void);
void CreateBall(void);
void CreatePaddles(void);
void UpdateDrawFrame(void);
void UpdateGame(void);
void DrawBall(void);
void DrawPaddles(void);

// Main entry point of the program
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

// Initialize the game components
void InitGame(void) {
    // Set screen dimensions
    game.screen = (Vector2) {800.0f, 600.0f};

    // Create the Box2D world
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity.y = 0.0f; // No gravity needed
    game.worldId = b2CreateWorld(&worldDef);

    // Seed the random number generator
    SetRandomSeed(time(NULL));

    // Create boundary walls, the ball, and the paddles
    CreateBoundary();
    CreateBall();
    CreatePaddles();
}

// Create the boundary walls
void CreateBoundary() {
    // Create a static body for the boundary walls
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = (b2Vec2) {0.0f, 0.0f};
    game.boundary.bodyId = b2CreateBody(game.worldId, &bodyDef);

    // Create shape definitions for the four boundary walls
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    // Top boundary wall
    b2Segment segment = {(b2Vec2) {0.0f, 0.0f}, (b2Vec2) {game.screen.x, 0.0f}};
    game.boundary.topShapeId = b2CreateSegmentShape(game.boundary.bodyId, &shapeDef, &segment);
    // Bottom boundary wall
    segment = (b2Segment) {(b2Vec2) {0.0f, game.screen.y}, (b2Vec2) {game.screen.x, game.screen.y}};
    game.boundary.bottomShapeId = b2CreateSegmentShape(game.boundary.bodyId, &shapeDef, &segment);
    // Left boundary wall
    segment = (b2Segment) {(b2Vec2) {0.0f, 0.0f}, (b2Vec2) {0.0f, game.screen.y}};
    game.boundary.leftShapeId = b2CreateSegmentShape(game.boundary.bodyId, &shapeDef, &segment);
    // Right boundary wall
    segment = (b2Segment) {(b2Vec2) {game.screen.x, 0.0f}, (b2Vec2) {game.screen.x, game.screen.y}};
    game.boundary.rightShapeId = b2CreateSegmentShape(game.boundary.bodyId, &shapeDef, &segment);
}

// Create the ball
void CreateBall() {
    // Set the ball's initial position, radius, velocity and color
    game.ball.radius = 15.0f;
    game.ball.pos = (b2Vec2) {game.screen.x / 2.0f, GetRandomValue(game.ball.radius, game.screen.y - game.ball.radius)};
    game.ball.velocity = (b2Vec2) {80.0f * (!(GetRandomValue(0, 1024) % 2)? 1.0f: -1.0f),
                                   80.0f * (!(GetRandomValue(0, 1024) % 2)? 1.0f: -1.0f)};
    game.ball.color = GREEN;

    // Create a dynamic body for the ball
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = game.ball.pos;
    bodyDef.type = b2_dynamicBody;
    game.ball.bodyId = b2CreateBody(game.worldId, &bodyDef);

    // Set the ball's velocity
    b2Body_SetLinearVelocity(game.ball.bodyId, game.ball.velocity);

    // Create a circular shape for the ball
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.restitution = 1.0f; // Perfectly elastic collition
    shapeDef.friction = 0.0f; // No friction needed
    b2Circle circle = {(b2Vec2) {0.0f, 0.0f}, game.ball.radius};
    game.ball.shapeId = b2CreateCircleShape(game.ball.bodyId, &shapeDef, &circle);
}

// Create the paddles
void CreatePaddles(void) {
    // Set the left paddle's initial position, size, velocity and color
    game.paddle_1.size = (b2Vec2) {40.0f, game.screen.y / 5.0f};
    game.paddle_1.pos = (b2Vec2) {game.paddle_1.size.x / 2.0f, game.screen.y / 2.0f};
    game.paddle_1.velocity = 20.0f;
    game.paddle_1.color = GREEN;

    // Set the right paddle's initial position, size, velocity and color
    game.paddle_2.size = (b2Vec2) {40.0f, game.screen.y / 5.0f};
    game.paddle_2.pos = (b2Vec2) {game.screen.x - game.paddle_2.size.x / 2.0f, game.screen.y / 2.0f};
    game.paddle_2.velocity = 20.0f;
    game.paddle_2.color = GREEN;

    // Create a static body for the left paddle
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = game.paddle_1.pos;
    game.paddle_1.bodyId = b2CreateBody(game.worldId, &bodyDef);

    // Create a static body for the right paddle
    bodyDef.position = game.paddle_2.pos;
    game.paddle_2.bodyId = b2CreateBody(game.worldId, &bodyDef);

    // Create shape definitions for the paddles
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    
    // Create a box shape for the left paddle
    b2Polygon left = b2MakeBox(game.paddle_1.size.x / 2.0f, game.paddle_1.size.y / 2.0f);
    game.paddle_1.shapeId = b2CreatePolygonShape(game.paddle_1.bodyId, &shapeDef, &left);

    // Create a box shape for the right paddle
    b2Polygon right = b2MakeBox(game.paddle_2.size.x / 2.0f, game.paddle_2.size.y / 2.0f);
    game.paddle_2.shapeId = b2CreatePolygonShape(game.paddle_2.bodyId, &shapeDef, &right);
}

// Main update and rendering function
void UpdateDrawFrame(void) {
    // Update the physics and game state
    UpdateGame();

    // Begin drawing
    BeginDrawing();

    // Clear the background
    ClearBackground(WHITE);

    // Draw the paddles
    DrawPaddles();

    // Draw the ball
    DrawBall();

    // End drawing
    EndDrawing();
}

// Update the physics and game state
void UpdateGame(void) {
    // Step the physics simulation
    b2World_Step(game.worldId, GetFrameTime(), 4);

    // Get movement events from the physics world
    b2BodyEvents events = b2World_GetBodyEvents(game.worldId);

    // Iterate through all bodies that moved during the step 
    for (int i = 0; i < events.moveCount; i++) {
        b2BodyMoveEvent *event = events.moveEvents + i;

        // If the ball moved, update its state
        if (B2_ID_EQUALS(event->bodyId, game.ball.bodyId)) {
            game.ball.pos = event->transform.p;
            game.ball.velocity = b2Body_GetLinearVelocity(event->bodyId);
        }
    }

    // Retrieve the maximum number of contacts the ball can have
    int ballContactCapacity = b2Shape_GetContactCapacity(game.ball.shapeId);
    // Retrieve the current contact data for the ball
    b2ContactData ballContactData[ballContactCapacity];
    int ballContactCount = b2Shape_GetContactData(game.ball.shapeId, ballContactData, ballContactCapacity);

    // Iterate through all contacts involving the ball
    for (int i = 0; i < ballContactCount; i++) {
        b2ContactData *data = ballContactData + i;

        // If the ball collides with the left boundary
        if (B2_ID_EQUALS(data->shapeIdA, game.boundary.leftShapeId)) {
            // Reset the ball's position to the screen center with a random vertical offset
            game.ball.pos = (b2Vec2) {game.screen.x / 2.0f,
                                     GetRandomValue(game.ball.radius, game.screen.y - game.ball.radius)};

            // Ensure the ball keep moving towards the left
            game.ball.velocity.x *= game.ball.velocity.x > 0.0f? -1.0f: 1.0f;
            // Randomize the ball's vertical velocity direction
            game.ball.velocity.y *= !(GetRandomValue(0, 1024) % 2)? 1.0f: -1.0f;

            // Apply the new position and velocity to the physics world
            b2Body_SetTransform(game.ball.bodyId, game.ball.pos, (b2Rot) {0.0f});
            b2Body_SetLinearVelocity(game.ball.bodyId, game.ball.velocity);
        }
        // If the ball collides with the right boundary
        if (B2_ID_EQUALS(data->shapeIdA, game.boundary.rightShapeId)) {
            // Reset the ball's position to the screen center with a random vertical offset
            game.ball.pos = (b2Vec2) {game.screen.x / 2.0f,
                                     GetRandomValue(game.ball.radius, game.screen.y - game.ball.radius)};

            // Ensure the ball keep moving towards the right
            game.ball.velocity.x *= game.ball.velocity.x > 0.0f? 1.0f: -1.0f;
            // Randomize the ball's vertical velocity direction
            game.ball.velocity.y *= !(GetRandomValue(0, 1024) % 2)? 1.0f: -1.0f;

            // Apply the new position and velocity to the physics world
            b2Body_SetTransform(game.ball.bodyId, game.ball.pos, (b2Rot) {0.0f});
            b2Body_SetLinearVelocity(game.ball.bodyId, game.ball.velocity);
        }
    }
}

// Draw the ball on the screen
void DrawBall(void) {
    DrawCircle(game.ball.pos.x, game.ball.pos.y, game.ball.radius, game.ball.color);
}

// Draw the paddles on the screen
void DrawPaddles(void) {
    // Get the top left corner of the left paddle
    b2Vec2 p = b2Body_GetWorldPoint(game.paddle_1.bodyId, (b2Vec2) {-game.paddle_1.size.x / 2.0f, -game.paddle_1.size.y / 2.0f});

    // Draw the left paddle
    DrawRectangle(p.x, p.y, game.paddle_1.size.x, game.paddle_1.size.y, game.paddle_1.color);

    // Get the top left corner of the right paddle
    p = b2Body_GetWorldPoint(game.paddle_2.bodyId, (b2Vec2) {-game.paddle_2.size.x / 2.0f, -game.paddle_2.size.y / 2.0f});

    // Draw the left paddle
    DrawRectangle(p.x, p.y, game.paddle_2.size.x, game.paddle_2.size.y, game.paddle_2.color);
}