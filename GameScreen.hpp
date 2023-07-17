#pragma once
#include "ScreenInterface.hpp"
#include "Game.hpp"

class GameScreen : public ScreenInterface
{
    public:
        uint8_t gridSpacing;
        Game game;
        unsigned char frameCount = 0;
        unsigned char gameSpeed = 15;

        GameScreen();
        GameScreen(const uint16_t windowWidth, const uint16_t windowHeight, const uint8_t gridSpacing=40);
        CustomReturnCode run(SDL_Renderer* renderer);

};