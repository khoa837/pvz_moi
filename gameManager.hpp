#include "raylib.h"
#include <iostream>
#include <vector>

namespace grid{
    struct gridPos{
        int column;
        int row;
    };

    const int COLUMN = 13;
    const int ROW = 5;
    const int GRID_SIZE = 100;
    const int FIRST_SQUARE_X = 100;
    const int FIRST_SQUARE_Y = 500;
    
    void drawGrid(){
        for(int i = 0; i < COLUMN; i++){
            for(int j = 0; j < ROW; j++){
                DrawRectangleLines(FIRST_SQUARE_X + i * GRID_SIZE, FIRST_SQUARE_Y + j * GRID_SIZE, GRID_SIZE, GRID_SIZE, 
                BLACK);
            }
        }
    }
    
    bool insideGrid(){
        if(GetMouseX() >= FIRST_SQUARE_X && GetMouseX() <= FIRST_SQUARE_X + COLUMN * 
        GRID_SIZE
        && GetMouseY() >= FIRST_SQUARE_Y && GetMouseY() <= FIRST_SQUARE_Y + ROW *
         GRID_SIZE)
        {
            return true;
        }
        return false;
    }
    
    gridPos getMousePos(){ // NEVER call this function alone without calling insideGrid()
        return{(GetMouseX() - FIRST_SQUARE_X) / GRID_SIZE, (GetMouseY() - FIRST_SQUARE_Y)
            / GRID_SIZE};
    }

    // template <typename T>
};

namespace gameTime{
    const int EXPECTED_FPS = 60;
};

class InternalClock{
    private:
    bool state = false;
    
    public:

    bool getState(){
        return state;
    }

    void setState(bool state){
        this->state = state;
    }
};

class Coordinates{ // this class is used solely to store coords
    private:
    Vector2 coords;

    public:
    Coordinates(float x, float y){
        this->coords.x = x;
        this->coords.y = y;
    }

    void setx(float x){
        this->coords.x = x;
    }

    void sety(float y){
        this->coords.y = y;
    }

    void set(Vector2 coords){
        this->coords = coords;
    }
    
    Vector2 get(){
        return coords;
    }
}; 

namespace sun{
    class SunBank{
        private:
        unsigned int total = 200;

        public:
        void draw(){
            DrawText(std::to_string(total).c_str(), 100, 100, 63, YELLOW);
            DrawCircle(250, 125, 40, YELLOW);
        }

        int getTotal(){
            return total;
        }

        bool add(int amount){ // use negative numbers for subtraction, 0 means result would be smaller than 0
        if(total + amount >= 0){
                total += amount;
                return 1;       
            }
            return 0; 
        }
    };

    class Sun{
        private:
        Coordinates finalCoords{0, 0};
        Coordinates currentCoords{0, 0};
        
        void drawSun(){
            DrawCircle(currentCoords.get().x, currentCoords.get().y, RADIUS, YELLOW);
        }
        
        void dropSun(){
            if(currentCoords.get().y <= finalCoords.get().y){
                currentCoords.sety(currentCoords.get().y + DROP_SPEED);
            }
        }

        public:
        InternalClock clock; 
        
        static const int DROP_COOLDOWN = 5  * gameTime::EXPECTED_FPS;// FPS
        static const int DROP_SPEED = 10;
        static const int RADIUS = 16; // Dropped sun's radius, not the radius displayed in sunBank
        
        void startClock(){
            clock.setState(true);
            finalCoords.setx(GetRandomValue(grid::FIRST_SQUARE_X, grid::FIRST_SQUARE_X + grid::COLUMN * grid::GRID_SIZE));
            finalCoords.sety(GetRandomValue(grid::FIRST_SQUARE_Y, grid::FIRST_SQUARE_Y + grid::COLUMN * grid::GRID_SIZE));
            currentCoords.setx(finalCoords.get().x);
            currentCoords.sety(RADIUS);
        }

        void mainLoop(){
            if(clock.getState()){
                drawSun();
                dropSun();
            }
        }
    };

    void sunsMainLoop(const int TIME, std::vector<Sun>& suns){
        for(size_t i = 0; i < suns.size(); i++){
            suns[i].mainLoop();
        }
        if(TIME % sun::Sun::DROP_COOLDOWN == 0){
            suns[0].startClock();
        }

    }
};

void mainLoop(const int TIME, sun::SunBank& sunBank, std::vector<sun::Sun>& suns){ // ONLY CALL THIS FUNCTION
    grid::drawGrid();
    sunBank.draw();
    sun::sunsMainLoop(TIME, suns);
    if(IsKeyDown(KEY_S)){
        std::string out; 
        DrawText(out.c_str(), 500, 500, 100, RED);
    }
}