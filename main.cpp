#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <cmath>
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
        struct food{
            SDL_Rect rect;
            float scale = 0.5f;
        };

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


        // test food
        food snake_food;
        int x = static_cast<int>(0 + spacing*pow(snake_food.scale,2)); // snake_food.scale ^ 2 centers coordinate
        int y = static_cast<int>(0 + spacing*pow(snake_food.scale,2));
        int w = static_cast<int>(spacing*snake_food.scale);
        int h = static_cast<int>(spacing*snake_food.scale);
        snake_food.rect = {x, y, w, h};
        bool eaten = false;
        bool stop_eat_check = false;

        // end test food

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
                // update the head position
                body old_head = snake.back();
                body new_head;
                new_head.is_head = true;
                new_head.rect.x = old_head.rect.x + (direction.x * spacing);
                new_head.rect.y = old_head.rect.y + (direction.y * spacing);
                new_head.rect.w = old_head.rect.w;
                new_head.rect.h = old_head.rect.h;

                // update body positions
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
            // clamp rect position for snake head
            if (snake.back().rect.x > 0)
            {
                snake.back().rect.x = snake.back().rect.x % SCREEN_WIDTH;
            }
            else if (snake.back().rect.x < 0)
            {
                snake.back().rect.x = SCREEN_WIDTH - snake.back().rect.w;
            }
            if (snake.back().rect.y > 0)
            {
                snake.back().rect.y = snake.back().rect.y % SCREEN_HEIGHT;
            }
            else if (snake.back().rect.y < 0)
            {
                snake.back().rect.y = SCREEN_HEIGHT - snake.back().rect.h;
            }


            // check for collision between head and food

            if (SDL_HasIntersection(&(snake.back().rect), &(snake_food.rect)) and !stop_eat_check){
                eaten = true;
                // add new body segment
                int newX = (direction.x != 0) ? snake.front().rect.x + (spacing * -1 * direction.x) : snake.front().rect.x;
                int newY = (direction.y != 0) ? snake.front().rect.y + (spacing * -1 * direction.y) : snake.front().rect.y;
                snake.push_front((body){newX, newY, spacing, spacing});


                // find new location for food rect (any cell unocupied by snake)
                bool cell_found = false;
                int cellX = -1;
                int cellY = -1;
                while (!cell_found){
                    int hRange = SCREEN_WIDTH / spacing;
                    int vRange = SCREEN_HEIGHT / spacing;
                    printf("Horizontal squares: %d\n", hRange);
                    printf("Vertical squares: %d\n", vRange);
                    int hGuess = rand() % hRange;
                    int vGuess = rand() % vRange;
                    cellX = hGuess * spacing; 
                    cellY = vGuess * spacing; 

                    for(auto body_segment: snake){
                        if (body_segment.rect.x == cellX and body_segment.rect.y == cellY) { continue; }
                    }
                    cell_found = true;
                }
                int y = static_cast<int>(0 + spacing * pow(snake_food.scale, 2));
                snake_food.rect.x = static_cast<int>(cellX + (spacing * pow(snake_food.scale, 2)));
                snake_food.rect.y = static_cast<int>(cellY + (spacing * pow(snake_food.scale, 2)));
                //stop_eat_check = true; // temporary hack code to stop conditional after first execution.
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

            // render food
            //if (!eaten){
            //    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF); // blue
            //    SDL_RenderFillRect(gRenderer, &(snake_food.rect));
            //}

            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF); // blue
            SDL_RenderFillRect(gRenderer, &(snake_food.rect));

            // render snake
            for(it = snake.begin(); it != snake.end(); ++it){
               // set draw color 
               if (it->is_head){
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF); // red
               }
               else{
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF); // green
               }
               // render rect
               SDL_RenderFillRect(gRenderer, &(it->rect));
            }

            // Render frame
            SDL_RenderPresent(gRenderer);
        }
    }
    close();
    return 0;
}