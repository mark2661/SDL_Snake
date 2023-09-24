#include "ScreenManager.hpp"


ScreenManager::ScreenManager() {}

ScreenManager::ScreenManager(const uint16_t windowWidth, const uint16_t windowHeight, SDL_Renderer* renderer)
{
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    this->renderer = renderer;
    

    // screens
    this->screens.insert({"GAME_SCREEN", new GameScreen(windowWidth, windowHeight)});
    this->screens.insert({"MENU_SCREEN", new MenuScreen(windowWidth, windowHeight)});
    //this->currentScreen = this->screens.at("GAME_SCREEN");
    this->currentScreen = this->screens.at("MENU_SCREEN");
}

uint16_t ScreenManager::getWindowWidth()
{
    return this->windowWidth;
}

uint16_t ScreenManager::getWindowHeight()
{
    return this->windowHeight;
}

SDL_Renderer* ScreenManager::getRenderer()
{
    return this->renderer;
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

bool ScreenManager::runGame()
{
        bool quit = false;
        CustomReturnCode returnCode = this->currentScreen->run(this->renderer);
        switch (returnCode)
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

        return quit;
}

bool ScreenManager::runMenu(const int mouseX, const int mouseY)
{
        bool quit = false;
        CustomReturnCode returnCode = this->currentScreen->run(this->renderer, mouseX, mouseY);
        switch (returnCode)
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

        return quit;

}

//bool ScreenManager::run(SDL_Renderer* renderer)
bool ScreenManager::run()
{
    bool quit = false;
    int mouseX = -1;
    int mouseY = -1;
    // process event queue
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            return true;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (SDL_BUTTON(SDL_BUTTON_LEFT))
            {
                SDL_GetMouseState(&mouseX, &mouseY);
                //printf("Click: MouseX: %d, MouseY: %d\n", mouseX, mouseY);
            }
        }
    }
    if (this->renderer != nullptr and this->currentScreen != nullptr)
    {
        if (this->currentScreen->getType() == "GAME")
        {
            bool quit = this->runGame();
        }
        else if (this->currentScreen->getType() == "MENU")
        {

            //printf("In screen manager: X: %d\n" ,this->currentScreen->menu.options.at(1).boundingRect.x)
            bool quit = this->runMenu(mouseX, mouseY);
        }
        //CustomReturnCode returnCode = this->currentScreen->run(this->renderer, mouseX, mouseY);
        //switch (returnCode)
        //{
        //case RUN:
        //    quit = false;
        //    break;
        //case QUIT:
        //    quit = true;
        //    break;
        //case START_MENU:
        //    quit = false;
        //    switchScreen("START_MENU");
        //    break;
        //case GAME_OVER_MENU:
        //    quit = false;
        //    switchScreen("GAME_OVER_MENU");
        //    break;
        //case GAME_SCREEN:
        //    quit = false;
        //    switchScreen("GAME_SCREEN");
        //    break;
        //}
    }
    else
    {
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
    renderer = nullptr;
    currentScreen = nullptr;
}