#pragma once
#include "MenuInterface.hpp"


class StartMenu : MenuInterface
{
    public:
        StartMenu();
        CustomReturnCode processInputs();
        CustomReturnCode run(SDL_Renderer* renderer);
        void render(SDL_Renderer* renderer);

};