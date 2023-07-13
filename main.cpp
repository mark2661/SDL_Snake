#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>

bool init();
void close();

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Rendering Window
SDL_Window* gWindow = nullptr;

// Renderer
SDL_Renderer* gRenderer = nullptr;

bool init()
{
    // Initialisation flag
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) > 0)
    {
        printf("SDL could not initialise! SDL ERROR!: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
            success = false;
        }

        // create window
        gWindow = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if (gWindow == nullptr)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }

        else
        {
            // create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1 , SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

            if (gRenderer == nullptr)
            {
                printf("Renderer could not be created! SDL ERROR: %s\n", SDL_GetError());
                success = false;
            }

            else
            {
                // initialise renderer colour
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF); // black
            }
        }
    }
    return success;
}

void close()
{
    // destroy renderer
    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;

    // destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;


    SDL_Quit();
}


int main()
{
    if (!init())
    {
        printf("Failed to initialise!\n");
    }
    else
    {
        bool quit = false;

        // SDL event handler
        SDL_Event event;


        SDL_Rect rect = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 100, 200};
        int moveVal = 10;
        const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);

        while(!quit)
        {
            // process event queue
            while (SDL_PollEvent(&event) != 0)
            {
                if (event.type == SDL_QUIT) {quit = true;}
                
            }

            currentKeyStates = SDL_GetKeyboardState(NULL);
            // process keyboard input
            if (currentKeyStates[SDL_SCANCODE_W])
            {
                rect.y -= moveVal;
            }

            if (currentKeyStates[SDL_SCANCODE_S])
            {
                rect.y += moveVal;
            }

            if (currentKeyStates[SDL_SCANCODE_A])
            {
                rect.x -= moveVal;
            }

            if (currentKeyStates[SDL_SCANCODE_D])
            {
                rect.x += moveVal;
            }

            // clear screen
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF); // black
            SDL_RenderClear(gRenderer);

            //create green rectangle
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF); // green
            SDL_RenderFillRect(gRenderer, &rect);

            SDL_RenderPresent(gRenderer);
        }
    }
    close();
    return 0;
}