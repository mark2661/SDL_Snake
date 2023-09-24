#pragma once
//#include "ScreenManager.hpp"
#include <cinttypes>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "utils.hpp"

//class ScreenManager;

class ScreenInterface
{
    private:
        std::string screenType;

    public:
        uint16_t windowWidth;
        uint16_t windowHeight;

        ScreenInterface();
        ScreenInterface(const uint16_t windowWidth, const uint16_t windowHeight, std::string screenType);
        std::string getType();
        virtual CustomReturnCode run(SDL_Renderer* renderer) = 0;
        virtual CustomReturnCode run(SDL_Renderer* renderer, const int mouseX, const int mouseY) = 0;

};
