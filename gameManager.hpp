#pragma once

#include "raylib.h"
#include "math.h"
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
    
    int getMouseColumn(){
        return (GetMouseX() - FIRST_SQUARE_X) / GRID_SIZE;
    }
    
    int getMouseRow(){
        return (GetMouseY() - FIRST_SQUARE_Y)/ GRID_SIZE;
    }
    
    gridPos getMousePos(){ // NEVER call this function alone without calling insideGrid()
        return{getMouseColumn(), getMouseRow()}; 
    }

    template <typename T>
    int getCircleClosestToMouseIndex(std::vector<T> objects){ // return -1 if nothing inside mouse
        for(size_t i = 0; i < objects.size(); i++){
            if(objects[i].clock.getState() == 1){
                if(pow((GetMouseX() - objects[i].getCoords().x), 2) + 
                pow((GetMouseY() - objects[i].getCoords().y), 2) < pow(objects[i].RADIUS, 2)){
                    return i;
                }
            }
        }
        return -1;
    }

    bool gridPlants[ROW][COLUMN] = {false}; // will need to change the type when upgrading

    int columnToX(int column){
        return FIRST_SQUARE_X + column * GRID_SIZE + GRID_SIZE / 2;
    }

    int rowToY(int row){
        return FIRST_SQUARE_Y + row * GRID_SIZE + GRID_SIZE / 2;
    }
};

namespace gameConstants{
    const int EXPECTED_FPS = 60;
    const int STANDARD_VECTOR_SIZE = 1;
};

class InternalClock{
    private:
    bool state = false;
    unsigned int referenceTime = 0; // should be 0 to indicate not in use
    public:

    unsigned int getReferenceTime(){
        return referenceTime;
    }

    void setReferenceTime(unsigned int referenceTime){ // automatically sets ref time to 1 if somehow 0 is passed, use resetReferenceTime instead
        if(referenceTime == 0){
            this->referenceTime = 1;
        }
        else{
            this->referenceTime = referenceTime;
        }
    }

    void resetReferenceTime(){
        referenceTime = 0;
    }

    bool getState(){
        return state;
    }

    void setState(bool state){
        this->state = state;
    }
};

class Object{ 
    protected:
    Vector2 coords;

    public:
    void setx(float x){
        this->coords.x = x;
    }

    void sety(float y){
        this->coords.y = y;
    }

    void setCoords(Vector2 coords){
        this->coords = coords;
    }
    
    Vector2 getCoords(){
        return coords;
    }
}; 

class SunBank{
    private:
    int total = 200;
    
    public:
    void draw(){
        DrawText(std::to_string(total).c_str(), 100, 100, 63, YELLOW);
        DrawCircle(250, 125, 40, YELLOW);
    }
    
    int getTotal(){
        return total;
    }
    
    bool add(int amount){ // use negative numbers for subtraction, 0 means result would be smaller than 0, 
        //if returns 0, function does NOT change the value of sunBank
        if(total + amount >= 0){
            total += amount;
            return 1;       
        }
        return 0; 
    }
};

class Sun : public Object{
    private:
    Vector2 finalCoords = {0, 0};
    bool fromSky = true; // from sky or from sun makers
    
    static const int sideSpeed = 10; // only used to calculate position from throwing sun
    static const int UP_SPEED = 10; 
    int currentUpSpeed = UP_SPEED;
    public:
    static const int VALUE = 50;
    
    InternalClock clock; 
    static const int DROP_COOLDOWN = 10  * gameConstants::EXPECTED_FPS;// FPS
    static const int DROP_SPEED = 10;
    static const int RADIUS = 16; // Dropped sun's radius, not the radius displayed in sunBank
    
    Sun(){
        finalCoords.x = GetRandomValue(grid::FIRST_SQUARE_X, grid::FIRST_SQUARE_X + grid::COLUMN * grid::GRID_SIZE);
        finalCoords.y = GetRandomValue(grid::FIRST_SQUARE_Y, grid::FIRST_SQUARE_Y + grid::ROW * grid::GRID_SIZE);
        setx(finalCoords.x);
        sety(-RADIUS);
    }
    
    void startClock(){
        clock.setState(true);
    }
    
    void resetClock(){
        clock.setState(false);
        finalCoords.x = GetRandomValue(grid::FIRST_SQUARE_X, grid::FIRST_SQUARE_X + grid::COLUMN * grid::GRID_SIZE);
        finalCoords.y = GetRandomValue(grid::FIRST_SQUARE_Y, grid::FIRST_SQUARE_Y + grid::ROW * grid::GRID_SIZE);
        setx(finalCoords.x);
        sety(-RADIUS);
        currentUpSpeed = 10;
    }
    
    void initSunFromSunMaker(grid::gridPos sunMakerPosition){
        fromSky = false;
        clock.setState(true);
        coords.x = grid::columnToX(sunMakerPosition.column);
        coords.y = grid::rowToY(sunMakerPosition.row);
        finalCoords.x = coords.x + sideSpeed * UP_SPEED * 2;
        finalCoords.y = coords.y;
    }
    
    void mainLoop(){
        if(clock.getState() == true){
            drawSun();
            if(fromSky) dropSunFromSky();
            else throwSunFromSunMaker();
        }
    }

    static void startThrowingSunFromSunMaker(const unsigned int TIME, std::vector<Sun>& suns, grid::gridPos sunMakerPos){
        bool oneAvailable = false;
        for(size_t i = 0; i < suns.size(); i++){
            if(suns[i].clock.getState() == false){
                suns[i].initSunFromSunMaker(sunMakerPos);
                oneAvailable = true;
            }
        }
        if(oneAvailable == false){
            suns.emplace_back();
            suns.back().initSunFromSunMaker(sunMakerPos);
        }
    }
    
    static void sunsMainLoop(const unsigned int TIME, SunBank& sunbank, std::vector<Sun>& suns){
        int index = grid::getCircleClosestToMouseIndex(suns);
        for(size_t i = 0; i < suns.size(); i++){
            suns[i].mainLoop();
        }
        if(TIME != 0 && TIME % Sun::DROP_COOLDOWN == 0){
            bool oneAvailable = false;
            for(size_t j = 0; j < suns.size(); j++){
                if(suns[j].clock.getState() == false){
                    oneAvailable = true;
                    suns[j].startClock();
                    break;
                }
            }
            if(oneAvailable == false){
                suns.emplace_back();
                suns.back().startClock();
            }
        }
        if(index != -1){
            suns[index].resetClock();
            sunbank.add(Sun::VALUE);
            if(suns.size() > gameConstants::STANDARD_VECTOR_SIZE){
                suns.erase(suns.begin() + index); 
            }
        }
    }

    private:
        void drawSun(){
        DrawCircle(getCoords().x, getCoords().y, RADIUS, YELLOW);
    }
    
    void dropSunFromSky(){
        if(getCoords().y <= finalCoords.y){
            sety(getCoords().y + DROP_SPEED);
        }
    }

    void throwSunFromSunMaker(){
        if(coords.x <= finalCoords.x){
            if(clock.getReferenceTime() % 2 == 0){
                coords.x += sideSpeed;        
            }
            else{
                coords.x -= sideSpeed;
            }
            coords.y -= currentUpSpeed;
            currentUpSpeed--;
        }
    }
};


void mainLoop(const unsigned int TIME, SunBank& sunBank, std::vector<Sun>& suns){ // ONLY CALL THIS FUNCTION
    grid::drawGrid();
    sunBank.draw();
    Sun::sunsMainLoop(TIME, sunBank, suns);
    // if(IsKeyDown(KEY_S)){
    //     std::string out; 

    //     DrawText(out.c_str(), 500, 500, 100, RED);
    // }
}