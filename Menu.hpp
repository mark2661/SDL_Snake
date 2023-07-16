#pragma once
#include <SDL2/SDL.h>
#include "utils.hpp"

class Menu 
{
    public:
        //Menu();
        virtual CustomReturnCode processInputs() = 0;
        virtual CustomReturnCode run(SDL_Renderer* renderer) = 0;
        virtual void render(SDL_Renderer* renderer) = 0;

};