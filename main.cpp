#include "gameManager.hpp"

int main () {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My first RAYLIB program!");
    SetTargetFPS(60);

    int time = 0;
    sun::SunBank sunBank;
    std::vector<sun::Sun> sun(1);
    while (WindowShouldClose() == false){
        BeginDrawing();
        ClearBackground(WHITE);
        mainLoop(time, sunBank, sun);
        time++;
        DrawText(std::to_string(time / 60).c_str(), 500, 500, 100, RED);
        EndDrawing();
    }

    CloseWindow();
}