#pragma once
#include "gameManager.hpp"

class GridObject{
    protected:
    grid::gridPos position;

    public:
    void setPosition(grid::gridPos position){
        this->position.column = position.column;
        this->position.row = position.row;
    }

    void setPosition(int column, int row){
        this->position.column = column;
        this->position.row = row;
    }

    grid::gridPos getPosition(){
        return position;
    }

    int getColumn(){
        return position.column;
    }

    int getRow(){
        return position.row;
    }

    float getx(){
        return grid::FIRST_SQUARE_X + position.column * grid::GRID_SIZE + grid::GRID_SIZE / 2;
    }

    float gety(){
        return grid::FIRST_SQUARE_Y + position.row * grid::GRID_SIZE + grid::GRID_SIZE / 2;
    }
};

class Plant: public GridObject{
    protected:
    int health;

    public:
    const int RADIUS = 45;
    InternalClock clock;

    void init(int health){
        this->health = health;
    }

    int getHealth(){
        return health;
    }

    bool addHealth(int amount){ // returns 1 if health would be <= 0 and sets health to exactly 0
        if(health + amount <= 0){
            health = 0;
            return 1;
        }
        else{
            health += amount;
            return 0;
        }
    }

    void placePlant(const unsigned int time){
        if(IsKeyPressed(KEY_S) && grid::insideGrid()){
            position = grid::getMousePos();
            clock.setReferenceTime(time);          
        }
    }
};

class SunMaker: public Plant{
    protected:
    virtual int getDelay() const = 0;

    public:
    void dropSun(){
        std::cout << getDelay() << '\n';
    }

    template <typename T>
    void sunMakerMainLoop(const unsigned int time){
        
    }
};

class Sunflower: public SunMaker{
    private:
    const static int SUN_DROP_DELAY = 15 * gameConstants::EXPECTED_FPS; 

    int getDelay() const override{
        return SUN_DROP_DELAY;
    }

    public:
    Sunflower(int health){
        init(health);
    }
};