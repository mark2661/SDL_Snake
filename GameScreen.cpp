#include "GameScreen.hpp"



GameScreen::GameScreen() {};

GameScreen:: GameScreen(const uint16_t windowWidth, const uint16_t windowHeight, const uint8_t gridSpacing) 
             : ScreenInterface(windowWidth, windowHeight)
{
    this->gridSpacing = gridSpacing;
    this->game = Game(windowWidth, windowHeight, gridSpacing);
}

CustomReturnCode GameScreen::run(SDL_Renderer* renderer)
{
    bool quitGame = false;
    quitGame = this->game.run(renderer, this->frameCount, this->gameSpeed);
    this->frameCount += 1;
    CustomReturnCode code = (quitGame) ? QUIT : RUN;
    return code;
}