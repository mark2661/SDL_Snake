#include "StartMenu.hpp"


StartMenu::StartMenu() 
{
    TTF_Font* titleFont = TTF_OpenFont("data/fonts/menu_font.ttf", 20); 
    if (titleFont == nullptr)
    {
        printf("Error could not load font: TTF Error!: %s\n", TTF_GetError());
    }

    // Add options text to menu
    this->options.push_back(CustomTextWrapper("Place holder title text", titleFont, {0xFF, 0x00, 0x00}));
}

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

void StartMenu::render(SDL_Renderer* renderer, const uint16_t windowWidth, const uint16_t windowHeight)
{
    // clear screen
    SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF); // grey
    SDL_RenderClear(renderer);

    // render option list text
    for (CustomTextWrapper option: this->options)
    {
        //option.renderText(windowWidth/4, windowHeight/4, renderer);
        option.renderTextCentered(windowWidth, windowHeight/4, renderer);
    }
}

CustomReturnCode StartMenu::run(SDL_Renderer* renderer, const uint16_t windowWidth, const uint16_t windowHeight)
{
    CustomReturnCode code = processInputs();
    render(renderer, windowWidth, windowHeight);
    return code;
}