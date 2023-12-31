#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <cmath>
#include <list>
#include <tuple>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "utils.hpp"
#include "Game.hpp"

bool init();
void close();

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FPS 60

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
            //gRenderer = SDL_CreateRenderer(gWindow, -1 , SDL_RENDERER_ACCELERATED);

            if (gRenderer == nullptr)
            {
                printf("Renderer could not be created! SDL ERROR: %s\n", SDL_GetError());
                success = false;
            }

            else
            {
                // initialise renderer colour
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF); // black

                // init TTF font
                if (TTF_Init() == -1)
                {
                    printf("SDL_ttf could not initialise! SDL_ttf Error!: %s\n", TTF_GetError());
                    success = false;
                }
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


    TTF_Quit();
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
        bool running = true;

        // SDL event handler
        SDL_Event event;

        // settings
        unsigned int frame_count = 0;
        int spacing = 40;
        int speed = 15;

        Game game = Game(SCREEN_WIDTH, SCREEN_HEIGHT, spacing);

        while(running)
        {
            Uint32 start = SDL_GetTicks();

            // process event queue
            while (SDL_PollEvent(&event) != 0)
            {
                if (event.type == SDL_QUIT)
                {
                    close();
                    return 0;
                }
            }

            running = !game.run(gRenderer, frame_count, speed);

            // Render frame
            SDL_RenderPresent(gRenderer);
            frame_count += 1;

            // cap frame rate
            Uint32 end = SDL_GetTicks();
            SDL_Delay(floor((1000.0f/FPS) - (end - start)));
        }
    }
    close();
    return 0;
}
