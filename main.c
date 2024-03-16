#include <raylib.h>

typedef struct {
    Vector2 position;
    Color color;
    float radius;
} SpaceObject;

int main(void) {

    // new comment in main
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Galactic Salvage Crew");

    // Initialize player's spaceship
    SpaceObject playerShip = { { screenWidth / 2, screenHeight / 2 }, BLUE, 20 };

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        // Update

        // Input handling
        if (IsKeyDown(KEY_RIGHT)) playerShip.position.x += 5;
        if (IsKeyDown(KEY_LEFT)) playerShip.position.x -= 5;
        if (IsKeyDown(KEY_UP)) playerShip.position.y -= 5;
        if (IsKeyDown(KEY_DOWN)) playerShip.position.y += 5;

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw player's spaceship
        DrawCircleV(playerShip.position, playerShip.radius, playerShip.color);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
