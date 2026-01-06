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

namespace Cay{
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
        
        void placePlant(const unsigned int time, MatTroi::SunBank& sunBank){
            if(sunBank.add(-getSunValue()) == true){
                position = grid::getMousePos();
                clock.setReferenceTime(time);  
                clock.setState(true);    
                std::cout << "placed plant at " << grid::getMouseColumn() << ' ' << grid::getMouseRow();
            } 
            else{
                std::cout << "You don't have enough sun to place it";
            }
        }
    };

    template <typename T>
    void plantsMainLoop(const unsigned int time, std::vector<T>& plants, MatTroi::SunBank& sunBank, 
        Texture2D plantTexture){
        for(size_t i = 0; i < plants.size(); i++){
            plants[i].draw(plantTexture);
        }
        
        bool oneAvailable = false;
        using namespace grid;

        if(IsKeyPressed(KEY_S) && insideGrid() && gridPlants[getMouseColumn()][getMouseRow()] == 0){
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
};

namespace TaoMatTroi{
    class SunMaker: public Cay::Plant{
        protected:
        virtual int getSunDropDelay() const = 0;
        
        public:
        void dropSun(){
        std::cout << getSunDropDelay() << '\n';
        }
    };
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
        Cay::plantsMainLoop(time, sunflowers, sunBank, sunflowerTexture);
        
    }
};