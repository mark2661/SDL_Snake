#pragma once
#include "ScreenInterface.hpp"
#include "StartMenu.hpp"


class MenuScreen : public ScreenInterface
{
    public:
        StartMenu menu;
        MenuScreen();
        MenuScreen(const uint16_t windowWidth, const uint16_t windowHeight);
        CustomReturnCode run(SDL_Renderer* renderer);

};