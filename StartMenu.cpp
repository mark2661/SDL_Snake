#include "StartMenu.hpp"

///######################################################## CustomTextWrapper Button Binding Functions ##############################################

CustomReturnCode startGame()
{
    printf("Start Game\n");
    return GAME_SCREEN;
}


///######################################################## END CustomTextWrapper Button Binding Functions ##############################################

StartMenu::StartMenu() 
{
    TTF_Font* titleFont = TTF_OpenFont("data/fonts/menu_font.ttf", 40); 
    TTF_Font* optionsFont = TTF_OpenFont("data/fonts/menu_font.ttf", 20); 
    if (titleFont == nullptr or optionsFont == nullptr)
    {
        printf("Error could not load font: TTF Error!: %s\n", TTF_GetError());
    }

    // Add title text to menu
    //this->options.push_back(CustomTextWrapper("Place holder title text", titleFont, {0xFF, 0x00, 0x00}));
    // Add options text to menu
    this->options.push_back(CustomTextWrapper("temp menu option text", optionsFont, {0xFF, 0x00, 0x00}));
    CustomReturnCode (*startGamePtr)() = startGame;
    this->options.push_back(CustomTextWrapper("temp menu option text", optionsFont, {0xFF, 0x00, 0x00}, startGamePtr));
    this->options.push_back(CustomTextWrapper("Press space to start", optionsFont, {0xFF, 0x00, 0x00}));

    //test
    //this->options[0] = CustomTextWrapper("Place holder title text", titleFont, {0xFF, 0x00, 0x00});
    //this->options[1] = CustomTextWrapper("temp menu option text", optionsFont, {0xFF, 0x00, 0x00}, startGamePtr);
    //this->options[2] = CustomTextWrapper("Press space to start", optionsFont, {0xFF, 0x00, 0x00});
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

    int y = windowHeight;

    // render option list text
    for (CustomTextWrapper option: this->options)
    {
        //option.renderText(windowWidth/2, y/4, renderer);
        option.renderTextHorizontallyCentered(windowWidth, y/4, renderer);
        y += 500;
    }
}

CustomReturnCode StartMenu::run(SDL_Renderer* renderer, const uint16_t windowWidth, const uint16_t windowHeight)
{
    CustomReturnCode code = processInputs();
    render(renderer, windowWidth, windowHeight);
    return code;
}

StartMenu::~StartMenu()
{

}