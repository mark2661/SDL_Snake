#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include "utils.hpp"



class MenuInterface 
{
    public:
        virtual CustomReturnCode processInputs() = 0;
        virtual void render(SDL_Renderer* renderer, const uint16_t windowWidth, const uint16_t windowHeight) = 0;
        virtual CustomReturnCode run(SDL_Renderer* renderer, const uint16_t windowWidth, const uint16_t windowHeight) = 0;

};