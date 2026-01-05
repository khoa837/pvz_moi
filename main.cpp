#include "gameManager.hpp"
#include "plants.hpp"

int main () {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My first RAYLIB program!");
    SetTargetFPS(gameConstants::EXPECTED_FPS);

    unsigned int time = 0; // might be a problem if the game runs for too long
    sun::SunBank sunBank;
    std::vector<sun::Sun> sun(gameConstants::STANDARD_VECTOR_SIZE);
    Sunflower sunflower(50);
    GridObject test;
    test.setPosition({2, 3});
    while (WindowShouldClose() == false){
        BeginDrawing();
        ClearBackground(WHITE);
        mainLoop(time, sunBank, sun);
        time++;
        DrawCircle(test.getx(), test.gety(), 40, RED);
        DrawText(std::to_string(time / 60).c_str(), 500, 500, 100, RED);
        EndDrawing();
    }

    CloseWindow();
}