#pragma once
#include "gameManager.hpp"

class Zombie: public Object{
    protected:
    virtual int getHealth() const = 0;
    virtual int getSpeed() const = 0;
    static const int START_X = gameConstants::MAX_SCREEN_WIDTH + 30;
    
    void draw(){
        DrawCircle(coords.x, coords.y, 30, BLACK);
        coords.x -= getSpeed();
    }

    public:
    Zombie(int row){
        coords.x = START_X;
        coords.y = grid::FIRST_SQUARE_Y + grid::GRID_SIZE * row + grid::GRID_SIZE / 2;
    }

    template <typename T>
    static void zombiesMainLoop(std::vector<T>& zombies){
        for(size_t i = 0; i < zombies.size(); i++){
            zombies[i].draw();
        }
    }
};

class BasicZombie: public Zombie{
    private:
    int getHealth() const override{
        return HEALTH;
    }

    int getSpeed() const override{
        return SPEED;
    }

    public:
    static const int HEALTH = 100;
    static const int SPEED = 1;

    using Zombie::Zombie;
};