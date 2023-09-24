#pragma once
#include "MenuInterface.hpp"


CustomReturnCode startGame();

class StartMenu : MenuInterface
{
    public:
        std::vector<CustomTextWrapper> options;
        //CustomTextWrapper options[3];
        StartMenu();
        CustomReturnCode processInputs();
        void render(SDL_Renderer* renderer, const uint16_t windowWidth, const uint16_t windowHeight);
        CustomReturnCode run(SDL_Renderer* renderer, const uint16_t windowWidth, const uint16_t windowHeight);
        ~StartMenu();

};