#pragma once
//#include "ScreenManager.hpp"
#include <cinttypes>
#include <SDL2/SDL.h>
#include "utils.hpp"

//class ScreenManager;

class ScreenInterface
{
    public:
        //ScreenManager* manager;
        uint16_t windowWidth;
        uint16_t windowHeight;

        ScreenInterface();
        ScreenInterface(const uint16_t windowWidth, const uint16_t windowHeight);
        virtual CustomReturnCode run(SDL_Renderer* renderer) = 0;

};
