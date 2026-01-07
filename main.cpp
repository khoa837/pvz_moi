#include "gameManager.hpp"
#include "plants.hpp"

int main () {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My first RAYLIB program!");
    SetTargetFPS(gameConstants::EXPECTED_FPS);

    unsigned int time = 0; // might be a problem if the game runs for too long
    SunBank sunBank;
    std::vector<Sun> suns(gameConstants::STANDARD_VECTOR_SIZE);
    std::vector<Sunflower> sunflowers(gameConstants::STANDARD_VECTOR_SIZE);
    Texture2D sunflowerTexture = LoadTexture("texture/huongduong.png");    
    while (WindowShouldClose() == false){
        BeginDrawing();
        ClearBackground(WHITE);
        Sunflower::sunflowersMainLoop(time, sunflowers, sunBank, suns, sunflowerTexture);
        mainLoop(time, sunBank, suns);
        time++;
        DrawText(std::to_string(time / 60).c_str(), 500, 500, 100, RED);
        EndDrawing();
    }

    UnloadTexture(sunflowerTexture);
    CloseWindow();
}