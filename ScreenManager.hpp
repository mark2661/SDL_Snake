#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <unordered_map>
#include "ScreenInterface.hpp"
#include "GameScreen.hpp"
#include "MenuScreen.hpp"
#include "utils.hpp"

class ScreenInterface;
class GameScreen;

class ScreenManager
{
    public:
        std::unordered_map<std::string, ScreenInterface *> screens;

        ScreenInterface* currentScreen = nullptr;
    
        ScreenManager();
        ScreenManager(const uint16_t windowWidth, const uint16_t windowHeight);
        void switchScreen(std::string screenCode);
        bool run(SDL_Renderer* renderer);
        ~ScreenManager();

};