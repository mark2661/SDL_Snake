#include "MenuScreen.hpp"


MenuScreen::MenuScreen() {}

MenuScreen::MenuScreen(const uint16_t windowWidth, const uint16_t windowHeight) : ScreenInterface(windowWidth, windowHeight, "MENU")
{

}

void MenuScreen::processInputs()
{

}

CustomReturnCode MenuScreen::run(SDL_Renderer* renderer)
{
    return menu.run(renderer, this->windowWidth, this->windowHeight);
}

CustomReturnCode MenuScreen::run(SDL_Renderer* renderer, const int mouseX, const int mouseY)
{
    menu.run(renderer, this->windowWidth, this->windowHeight);
    CustomReturnCode returnCode = RUN;
    if (mouseX != -1 and mouseY != -1)
    {
        SDL_Point mousePoint = {mouseX, mouseY};
        for (CustomTextWrapper option: menu.options)
        {
            // TODO: Fix bug bounding rect has initialised values for some reason. 
            // it appears the CustomTextWrapper objects are constantly being destroyed and re initialised.

            printf("Button: %s, X: %d, Y: %d\n", option.text.c_str(), option.boundingRect.x, option.boundingRect.y);
            if (SDL_PointInRect(&(mousePoint), &(option.boundingRect)))
            {
                printf("Button clicked\n");
                returnCode = option.onClick();
            }
        }
        printf("\n\n");
    }
    return returnCode;
}

MenuScreen::~MenuScreen()
{

}