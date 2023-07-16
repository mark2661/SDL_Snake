#pragma once
#include "Menu.hpp"


class StartMenu : Menu
{
    public:
        StartMenu();
        CustomReturnCode processInputs();
        CustomReturnCode run(SDL_Renderer* renderer);
        void render(SDL_Renderer* renderer);

};