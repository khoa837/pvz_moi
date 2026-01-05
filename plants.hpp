#pragma once

#include "gameManager.hpp"
class Plant: public Object{
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

    bool addHealth(int amount){ // returns 1 if health is <= 0, sets health to exactly 0
        if(health + amount <= 0){
            health = 0;
            return 1;
        }
        else{
            health += amount;
            return 0;
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