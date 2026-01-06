#pragma once
#include "gameManager.hpp"

class GridObject{
    protected:
    grid::gridPos position;
    unsigned int health;
    
    public:
    void draw(Texture2D texture){
        DrawTexture(texture, grid::FIRST_SQUARE_X + grid::GRID_SIZE * position.column, 
        grid::FIRST_SQUARE_Y + grid::GRID_SIZE * position.row, WHITE);
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

    int getx(){
        return grid::FIRST_SQUARE_X + position.column * grid::GRID_SIZE + grid::GRID_SIZE / 2;
    }

    int gety(){
        return grid::FIRST_SQUARE_Y + position.row * grid::GRID_SIZE + grid::GRID_SIZE / 2;
    }
};

class Plant: public GridObject{
    protected:
    virtual int getSunValue() const = 0;

    public:
    const int RADIUS = 45;
    InternalClock clock;

    void init(unsigned int health){
        this->health = health;
    }

    unsigned int getHealth(){
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

    void placePlant(const unsigned int time, MatTroi::SunBank& sunBank){
        // if(sunBank.add())
        position = grid::getMousePos();
        clock.setReferenceTime(time);  
        clock.setState(true);    
        std::cout << "placed plant at " << position.column << ' ' << position.row << '\n';    
    }
};

namespace TaoMatTroi{
    class SunMaker: public Plant{
        protected:
        virtual int getSunDropDelay() const = 0;
        
        public:
        void dropSun(){
        std::cout << getSunDropDelay() << '\n';
    }
    
};
    template <typename T>
    void sunMakersMainLoop(const unsigned int time, std::vector<T>& sunMakers, MatTroi::SunBank& sunBank, 
        Texture2D sunMakerTexture){
        for(size_t i = 0; i < sunMakers.size(); i++){
            sunMakers[i].draw(sunMakerTexture);
        }
        
        bool oneAvailable = false;
        if(IsKeyPressed(KEY_S) && grid::insideGrid()){
            for(size_t j = 0; j < sunMakers.size(); j++){
                if(sunMakers[j].clock.getState() == false){
                    sunMakers[j].placePlant(time, sunBank);
                    oneAvailable = true;
                    break;
                }
            }
            if(oneAvailable == false){
                sunMakers.emplace_back();
                sunMakers.back().placePlant(time, sunBank);
            }   
        }
    }
}; 

namespace HuongDuong{
    class Sunflower: public TaoMatTroi::SunMaker{
        private:
        const static int SUN_DROP_DELAY = 15 * gameConstants::EXPECTED_FPS; 
        const static int SUN_VALUE = 50;
        const static int BASE_HEALTH = 100;
        
        int getSunDropDelay() const override{
            return SUN_DROP_DELAY;
        }
        
        int getSunValue() const override{
            return SUN_VALUE;
        };
        
        public:
        Sunflower(){
            init(BASE_HEALTH);
        }
        
        void SunflowersMainLoop(){
            
        }
    };

    void sunflowersMainLoop(const unsigned int time, std::vector<Sunflower>& sunflowers, MatTroi::SunBank& sunBank,
        Texture2D sunflowerTexture){
        TaoMatTroi::sunMakersMainLoop(time, sunflowers, sunBank, sunflowerTexture);
        
    }
};