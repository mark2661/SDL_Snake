#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <list>
#include <SDL2/SDL.h>
#include "utils.hpp"

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

        unsigned char frame_count = 0;
        int spacing = 40;
        SDL_Rect rect = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, spacing, spacing};
        int speed = 5;
        struct body {
            SDL_Rect rect;
            bool is_head = false;
        };
        struct {
            int x = 1;
            int y = 0;
        } direction;
        std::list<body> snake;
        std::list<body>::iterator it;

        // test snake
        body head;
        body torso;
        body torso2;
        head.rect = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, spacing, spacing};
        head.is_head = true;
        torso.rect = {head.rect.x-spacing, head.rect.y, spacing, spacing};
        torso2.rect = {head.rect.x-(2*spacing), head.rect.y, spacing, spacing};
        snake.push_front(head);
        snake.push_front(torso);
        snake.push_front(torso2);
        // end test snake

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
                direction = {0, -1};
            }

            if (currentKeyStates[SDL_SCANCODE_S])
            {
                direction = {0, 1};
            }

            if (currentKeyStates[SDL_SCANCODE_A])
            {
                direction = {-1, 0};
            }

            if (currentKeyStates[SDL_SCANCODE_D])
            {
                direction = {1, 0};
            }

            // iterate through snake and update positions
            if (frame_count % 15 == 0)
            {
                body old_head = snake.back();
                body new_head;
                new_head.is_head = true;
                new_head.rect.x = old_head.rect.x + (direction.x * spacing);
                new_head.rect.y = old_head.rect.y + (direction.y * spacing);
                new_head.rect.w = old_head.rect.w;
                new_head.rect.h = old_head.rect.w;

                for (it = snake.begin(); it != std::next(snake.end(), -1); ++it)
                {
                    it->rect.x = std::next(it,1)->rect.x; 
                    it->rect.y = std::next(it,1)->rect.y;
                }
                // remove old head
                snake.pop_back();
                // add new head
                snake.push_back(new_head);

            }
            frame_count++;

            // bounds check
            // clamp rect position for each part of snake
            for (it = snake.begin(); it != std::next(snake.end(), -1); ++it)
            {
                if (rect.x > 0){ rect.x = rect.x % SCREEN_WIDTH; }
                else if (rect.x < 0)
                {
                    rect.x = SCREEN_WIDTH - rect.w;
                }
                if (rect.y > 0)
                {
                    rect.y = rect.y % SCREEN_HEIGHT;
                }
                else if (rect.y < 0)
                {
                    rect.y = SCREEN_HEIGHT - rect.h;
                }
            }

            // clear screen
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF); // black
            SDL_RenderClear(gRenderer);

            // draw grid lines
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF); // white
            for(int i = spacing; i < SCREEN_WIDTH; i+= spacing){
                SDL_RenderDrawLine(gRenderer, i, 0, i, SCREEN_HEIGHT);
            }
            for(int j = spacing; j < SCREEN_HEIGHT; j += spacing){
                SDL_RenderDrawLine(gRenderer, 0, j, SCREEN_WIDTH, j);
            }

            // render snake
            for(it = snake.begin(); it != snake.end(); ++it){
               // set draw color 
               if (it->is_head){
                //printf("head\n");
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF); // red
               }
               else{
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF); // green
               }
               // render rect
               SDL_RenderFillRect(gRenderer, &(it->rect));
            }

            //SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF); // green
            //SDL_RenderFillRect(gRenderer, &rect);

            // Render frame
            SDL_RenderPresent(gRenderer);
            //print_list(snake);
        }
    }
    close();
    return 0;
}