#include "StartMenu.hpp"


StartMenu::StartMenu() {}

CustomReturnCode StartMenu::processInputs()
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_SPACE])
    {
        //return QUIT;
        return GAME_SCREEN;
    }

    return RUN;
}

void StartMenu::render(SDL_Renderer* renderer)
{
    // clear screen
    SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF); // grey
    SDL_RenderClear(renderer);
}

CustomReturnCode StartMenu::run(SDL_Renderer* renderer)
{
    CustomReturnCode code = processInputs();
    render(renderer);
    return code;
}