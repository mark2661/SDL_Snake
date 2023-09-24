#pragma once
#include "ScreenInterface.hpp"
#include "StartMenu.hpp"


class MenuScreen : public ScreenInterface
{
    public:
        StartMenu menu;
        MenuScreen();
        MenuScreen(const uint16_t windowWidth, const uint16_t windowHeight);
        void processInputs();
        CustomReturnCode run(SDL_Renderer* renderer);
        CustomReturnCode run(SDL_Renderer* renderer, const int mouseX, const int mouseY);
        ~MenuScreen();

};