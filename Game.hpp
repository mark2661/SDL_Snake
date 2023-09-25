# pragma once
#include <SDL2/SDL.h>
#include <unordered_map>
#include <tuple>
#include <cmath>
#include <iostream>
#include "utils.hpp"
#include "Snake.hpp"


struct Food{
    SDL_Rect rect;
    float scale = 0.5f;

    Food(int x, int y, int w, int h, const int spacing)
    {
        // initialises food rect in the top left grid cell
        int init_x = static_cast<int>(0 + spacing*pow(this->scale,2)); // snake_food.scale ^ 2 centers coordinate
        int init_y = static_cast<int>(0 + spacing*pow(this->scale,2));
        int init_w = static_cast<int>(spacing*this->scale);
        int init_h = static_cast<int>(spacing*this->scale);
        this->rect = {init_x, init_y, init_w, init_h};

    }
    Food(float customScale)
    {
        scale = customScale;
    }

    void render(SDL_Renderer* renderer)
    {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
        SDL_RenderFillRect(renderer, &(this->rect));
    }
};

class Game
{
    private:
        int screenWidth;
        int screenHeight;
        int gridSpacing = 40;
        bool pause = false;

    public:
        std::unordered_map<std::tuple<int, int>, int, hash_tuple> grid_map;
        Snake snake;
        Food food = Food(0, 0, gridSpacing, gridSpacing, gridSpacing);
    
        Game();
        Game(const int screenWidth, const int screenHeight, const int gridSpacing);
        void populateGrid();
        void moveSnake();
        void outOfBoundsCheck();
        bool collisionCheck();
        void checkFoodConsumption();
        void respawnFood();
        void renderGame(SDL_Renderer* renderer);
        void processInput();
        bool run(SDL_Renderer* renderer, const int frameCount, const int speed);

};



