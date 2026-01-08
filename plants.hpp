#pragma once
#include "gameManager.hpp"

class GridObject{
    protected:
    grid::gridPos position;
    unsigned int health;
    
    public:
    void draw(Texture2D texture){
        DrawTexture(texture, grid::columnToX(position.column) - grid::GRID_SIZE / 2, 
        grid::rowToY(position.row) - grid::GRID_SIZE / 2, WHITE);
    } 

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
};


class Plant: public GridObject{
    protected:
    virtual int getSunValue() const = 0;
    
    public:
    const int RADIUS = 45;
    InternalClock clock;
    
    template <typename T>
    static void plantsMainLoop(const unsigned int time, std::vector<T>& plants, SunBank& sunBank, 
        Texture2D plantTexture){
        for(size_t i = 0; i < plants.size(); i++){
            plants[i].draw(plantTexture);
        }
        
        bool oneAvailable = false;
        using namespace grid;
        if(IsKeyPressed(KEY_S) && insideGrid() && gridPlants[getMouseColumn()][getMouseRow()] == 0
        && sunBank.add(-plants[0].getSunValue()) == 1){ 
            gridPlants[getMouseColumn()][getMouseRow()] = 1;
            for(size_t j = 0; j < plants.size(); j++){
                if(plants[j].clock.getState() == false){
                    plants[j].placePlant(time, sunBank);
                    oneAvailable = true;
                    break;
                }
            }
            if(oneAvailable == false){
                plants.emplace_back();
                plants.back().placePlant(time, sunBank);
            }   
        }
    }
    
    void init(unsigned int health){
        this->health = health;
    }

    unsigned int getHealth(){
        return health;
    }

    void killPlant(){
        health = 0;
        clock.resetReferenceTime();
        clock.setState(false);
    }

    bool addHealth(int amount){ // returns 1 if health would be <= 0 and sets health to exactly 0
        if(health + amount <= 0){
            health = 0;
            killPlant();
            return 1;
        }
        else{
            health += amount;
            return 0;
        }
    }
    
    void placePlant(const unsigned int time, SunBank& sunBank){ 
        position = grid::getMousePos();
        clock.setReferenceTime(time);  
        clock.setState(true);    
    }
};


class SunMaker: public Plant{
    protected:
    virtual int getSunDropDelay() const = 0;
    
    public:
    template <typename T>
    static void sunMakersMainLoop(const unsigned int time, std::vector<T>& sunMakers, SunBank& sunBank, 
        std::vector<Sun>& suns, Texture2D sunMakerTexture){
            Plant::plantsMainLoop(time, sunMakers, sunBank, sunMakerTexture);
            for(size_t i = 0; i < sunMakers.size(); i++){
                if(sunMakers[i].clock.getState() && time != sunMakers[i].clock.getReferenceTime() &&
                    (time - sunMakers[i].clock.getReferenceTime()) % sunMakers[0].getSunDropDelay() == 0){
                    Sun::startThrowingSunFromSunMaker(time, suns, sunMakers[i].getPosition());
                }              
            }
    
    }

};


class Sunflower: public SunMaker{
    public:
    const static int SUN_DROP_DELAY = 15 * gameConstants::EXPECTED_FPS; 
    const static int SUN_VALUE = 50;
    const static int BASE_HEALTH = 100;
    
    int getSunDropDelay() const override{
        return SUN_DROP_DELAY;
    }
    
    int getSunValue() const override{
        return SUN_VALUE;
    };
    
    Sunflower(){
        init(BASE_HEALTH);
    }
    
    void SunflowersMainLoop(){
        
    }

    static void sunflowersMainLoop(const unsigned int time, std::vector<Sunflower>& sunflowers, SunBank& sunBank,
        std::vector<Sun>& suns, Texture2D sunflowerTexture){
        SunMaker::sunMakersMainLoop(time, sunflowers, sunBank, suns, sunflowerTexture);
        
    }
};
