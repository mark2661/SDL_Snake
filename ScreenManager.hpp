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
    private:
        uint16_t windowWidth;
        uint16_t windowHeight;
        SDL_Renderer* renderer = nullptr;
        SDL_Event event;

    public:
        std::unordered_map<std::string, ScreenInterface *> screens;

        ScreenInterface* currentScreen = nullptr;
    
        ScreenManager();
        ScreenManager(const uint16_t windowWidth, const uint16_t windowHeight, SDL_Renderer* renderer);
        uint16_t getWindowWidth();
        uint16_t getWindowHeight();
        SDL_Renderer* getRenderer();
        void switchScreen(std::string screenCode);
        //bool run(SDL_Renderer* renderer);
        bool run();
        bool runGame();
        bool runMenu(const int mouseX, const int mouseY);
        ~ScreenManager();

};