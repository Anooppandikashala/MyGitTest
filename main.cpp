// #include <raylib.h>

// typedef struct {
//     Vector2 position;
//     Color color;
//     float radius;
// } SpaceObject;

// int main() {
//     // Initialization
//     const int screenWidth = 1280;
//     const int screenHeight = 720;
//     const int increment = 3;

//     InitWindow(screenWidth, screenHeight, "Galactic Salvage Crew");

//     // Initialize player's spaceship
//     SpaceObject playerShip = { { screenWidth / 2, screenHeight / 2 }, BLUE, 20 };

//     SetTargetFPS(60);

//     // Main game loop
//     while (!WindowShouldClose()) {
//         // Update

//         // Input handling
//         if (IsKeyDown(KEY_RIGHT)) 
//         {
//             if(playerShip.position.x + (playerShip.radius/2) < screenWidth - (playerShip.radius/2) )
//             {
//                 playerShip.position.x += increment;
//             }
//         }
//         if (IsKeyDown(KEY_LEFT)) 
//         {
//             if(playerShip.position.x - (playerShip.radius/2) > increment )
//             {
//                 playerShip.position.x -= increment;
//             }
//         }
//         if (IsKeyDown(KEY_UP)) 
//         {
//             if(playerShip.position.y - (playerShip.radius/2) > increment )
//             {
//                 playerShip.position.y -= increment;
//             }
//         }
//         if (IsKeyDown(KEY_DOWN)) 
//         {
//             if(playerShip.position.y + (playerShip.radius/2) < screenHeight - (playerShip.radius/2) )
//             {
//                 playerShip.position.y += increment;
//             }
//         }

//         // Draw
//         BeginDrawing();
//         ClearBackground(RAYWHITE);

//         // Draw player's spaceship
//         DrawCircleV(playerShip.position, playerShip.radius, playerShip.color);

//         EndDrawing();
//     }

//     // De-Initialization
//     CloseWindow();

//     return 0;
// }


#include <raylib.h>
#include <vector>
#include <algorithm>
#include <thread>

const int screenWidth = 1500;
const int screenHeight = 800;


#define STONE_SIZE 30
#define PLAYER_SIZE 50
#define ROCK_SIZE_LIMIT 30
#define NO_OF_ROCKS 100
#define NO_OF_STONES 100


struct Player {
    Rectangle rect;
    Texture2D tex;
};

struct Stone {
    Rectangle rect;
    Texture2D tex;
    bool active;
};

struct Rock {
    Rectangle rect;
    Texture2D tex;
    bool active;
};

Player player;
Stone stones[NO_OF_STONES];
Rock rocks[NO_OF_ROCKS];
int score;

std::vector<std::pair<float,float>> stonePositions;
std::vector<std::pair<float,float>> rockPositions;


bool isPosOverlapWithCurrentPositions(const std::vector<std::pair<float,float>>& positions,const std::pair<float,float>& currentPos,float size)
{
    for(auto pos : positions)
    {
        Rectangle r1({pos.first,pos.second,size,size});
        Rectangle r2({currentPos.first,currentPos.second,size,size});
        if(CheckCollisionRecs(r2, r1))
        {
            return true;
        }
    }
    return false;
}

void initStones()
{
    for (int i = 0; i < NO_OF_STONES; ++i) {

        float x = (float)GetRandomValue(0, screenWidth - STONE_SIZE);
        float y = (float)GetRandomValue(0, screenHeight - STONE_SIZE);

        while(isPosOverlapWithCurrentPositions(stonePositions,std::make_pair(x,y),STONE_SIZE))
        {
            x = (float)GetRandomValue(0, screenWidth - STONE_SIZE);
            y = (float)GetRandomValue(0, screenHeight - STONE_SIZE);
        }

        // stones[i].rect = {x,y,STONE_SIZE,STONE_SIZE};

        Image stoneImg = LoadImage("stone.png");
        ImageResize(&stoneImg, STONE_SIZE, STONE_SIZE);
        stones[i].tex = LoadTextureFromImage(stoneImg);
        stones[i].rect = {x,y,(float)stones[i].tex.width,(float)stones[i].tex.height};

        stones[i].active = true;
        stonePositions.push_back(std::make_pair(x,y));
        UnloadImage(stoneImg);
    }
}

void initRocks()
{
    for (int i = 0; i < NO_OF_ROCKS; ++i) {
        float rockSize = ROCK_SIZE_LIMIT;//(float)GetRandomValue(5, ROCK_SIZE_LIMIT);

        float x = (float)GetRandomValue(0, screenWidth - rockSize);
        float y = (float)GetRandomValue(0, screenHeight - rockSize);

        while(isPosOverlapWithCurrentPositions(stonePositions,std::make_pair(x,y),STONE_SIZE) ||
            isPosOverlapWithCurrentPositions(rockPositions,std::make_pair(x,y),rockSize) )
        {
            x = (float)GetRandomValue(0, screenWidth - rockSize);
            y = (float)GetRandomValue(0, screenHeight - rockSize);
        }

        Image rockImg = LoadImage("rock.png");
        ImageResize(&rockImg, rockSize, rockSize);
        rocks[i].tex = LoadTextureFromImage(rockImg);
        rocks[i].rect = {x,y,(float)rocks[i].tex.width,(float)rocks[i].tex.height};
        rocks[i].active = true;
        rockPositions.push_back(std::make_pair(x,y));
        UnloadImage(rockImg);
    }
}

void InitGame() {
    // Initialize player
    Image playerImg = LoadImage("player.png");
    ImageResize(&playerImg, PLAYER_SIZE, PLAYER_SIZE);
    player.tex = LoadTextureFromImage(playerImg);
    player.rect = {screenWidth / 2 - PLAYER_SIZE, screenHeight / 2 - PLAYER_SIZE,(float)player.tex.width,(float)player.tex.height};
    UnloadImage(playerImg);

    // Initialize stones and rocks
    initStones();
    initRocks();
    
    rockPositions.clear();
    stonePositions.clear();

    score = 0;
}

void UpdateGame() {
    // Update game logic here

    // Move player with arrow keys
    if (IsKeyDown(KEY_RIGHT)) player.rect.x += 5;
    if (IsKeyDown(KEY_LEFT)) player.rect.x -= 5;
    if (IsKeyDown(KEY_DOWN)) player.rect.y += 5;
    if (IsKeyDown(KEY_UP)) player.rect.y -= 5;

    // Check collisions with stones
    for (int i = 0; i < NO_OF_STONES; ++i) {
        if (stones[i].active && CheckCollisionRecs(player.rect, stones[i].rect)) {
            stones[i].active = false;
            score += 10;
        }
    }

    // Check collisions with rocks
    for (int i = 0; i < NO_OF_ROCKS; ++i) {
        if (rocks[i].active && CheckCollisionRecs(player.rect, rocks[i].rect)) {
            rocks[i].active = false;
            score -= 5;
        }
    }
}

void drawStones()
{
    for (int i = 0; i < NO_OF_STONES; ++i) {
        if (stones[i].active)
        {
            DrawTexture(stones[i].tex,stones[i].rect.x,stones[i].rect.y,WHITE);
            // DrawRectangleRec(stones[i].rect, stones[i].color);
        }
    }
}

void drawRocks()
{
    for (int i = 0; i < NO_OF_ROCKS; ++i) {
        // if (rocks[i].active) DrawRectangleRec(rocks[i].rect, rocks[i].color);
        if (rocks[i].active) 
        {
            DrawTexture(rocks[i].tex,rocks[i].rect.x,rocks[i].rect.y,WHITE);
            //DrawRectangleRec(rocks[i].rect, rocks[i].color);
        }
    }
}

void DrawGame() {
    // Draw game elements here

    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw player
    DrawTexture(player.tex,player.rect.x,player.rect.y,WHITE);

    // DrawPoly({100,100},8,50,10,RED);

    // Draw stones and rocks
    drawStones();    
    drawRocks();

    // Draw score
    DrawText(TextFormat("Score: %d", score), 10, 10, 30, BLACK);

    EndDrawing();
}

int main() {
    InitWindow(screenWidth, screenHeight, "Moose Miners Game");
    SetTargetFPS(60);

    InitGame();

    while (!WindowShouldClose()) {
        UpdateGame();
        DrawGame();
    }

    CloseWindow();

    return 0;
}
