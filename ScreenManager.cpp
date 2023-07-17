#include "ScreenManager.hpp"


ScreenManager::ScreenManager() {}

ScreenManager::ScreenManager(const uint16_t windowWidth, const uint16_t windowHeight)
{
    // screens
    this->screens.insert({"GAME_SCREEN", new GameScreen(windowWidth, windowHeight)});
    this->screens.insert({"MENU_SCREEN", new MenuScreen(windowWidth, windowHeight)});
    //this->currentScreen = this->screens.at("GAME_SCREEN");
    this->currentScreen = this->screens.at("MENU_SCREEN");
}

void ScreenManager::switchScreen(const std::string screenCode)
{
    if (screenCode == "GAME_SCREEN")
    {
        this->currentScreen = this->screens.at(screenCode);
    }
    // TODO: add rest of screen transition logic

    return;
}

//bool ScreenManager::run(SDL_Renderer* renderer)
bool ScreenManager::run(SDL_Renderer* renderer)
{
    bool quit = true;
    if (this->currentScreen != nullptr)
    {
        CustomReturnCode returnCode = this->currentScreen->run(renderer);
        switch(returnCode)
        {
            case RUN:
                quit = false;
                break;
            case QUIT:
                quit = true;
                break;
            case START_MENU:
                quit = false;
                switchScreen("START_MENU");
                break;
            case GAME_OVER_MENU:
                quit = false;
                switchScreen("GAME_OVER_MENU");
                break;
            case GAME_SCREEN:
                quit = false;
                switchScreen("GAME_SCREEN");
                break;
        }
    }
    else{
        printf("Null ptr\n");
    }
    return quit;
}

ScreenManager::~ScreenManager()
{
    // manually free up memeory for each value (ScreenInterface*) in the hashmap 
    std::unordered_map<std::string, ScreenInterface *>::iterator it;
    for (it=this->screens.begin(); it!=this->screens.end(); ++it)
    {
        delete it->second;
        it->second = nullptr;
    }

    // clear does not free up the memeory pointed to by pointers in the hashmap therefore that has been done manually above.
    screens.clear();

    // since object currentScreen is pointing to is freed in the for loop above there is no need to free it here.
    // doing so will cause an exception. Instead point currentScreen to NULL to avoid dangling pointer.
    //delete currentScreen;
    currentScreen = nullptr;
}