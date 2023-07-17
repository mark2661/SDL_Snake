#include "MenuScreen.hpp"


MenuScreen::MenuScreen() {}

MenuScreen::MenuScreen(const uint16_t windowWidth, const uint16_t windowHeight) : ScreenInterface(windowWidth, windowHeight)
{

}

CustomReturnCode MenuScreen::run(SDL_Renderer* renderer)
{
    return menu.run(renderer, this->windowWidth, this->windowHeight);
}