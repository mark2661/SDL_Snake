#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <cmath>
#include <list>
#include <tuple>
#include <unordered_map>
#include <SDL2/SDL.h>
#include "utils.hpp"

bool init();
void close();
std::tuple<int, int> getGridId(int, int, int);


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

std::tuple<int, int> getGridId(int x, int y, int spacing)
{
    int gridX = x / spacing;
    int gridY = y / spacing;
    
    //return std::tuple<int, int> 
    return std::make_tuple(gridX, gridY);
}

// std::unordered_map cannot does not define a hash for tuples, therefore must define a custom hash function.
struct hash_tuple 
{
    template <class T1, class T2>

    size_t operator()(const std::tuple<T1, T2>& x) const
    {
        return std::get<0>(x) ^ std::get<1>(x);
    }

};

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

        // settings
        unsigned char frame_count = 0;
        int spacing = 40;
        int speed = 15;

        // Initialise grid hash map
        std::unordered_map<std::tuple<int,int>, int, hash_tuple> grid_map;
        int num_grid_rows = SCREEN_WIDTH / spacing;
        int num_grid_cols = SCREEN_HEIGHT / spacing;
        // populate every cell with 0 (0 = unoccupied cell, 1 = cell occupied by snake body segment)
        for (int r = 0; r < num_grid_rows; ++r){
            for (int c = 0; c < num_grid_cols; ++c){
                grid_map[std::make_tuple(r, c)] = 0;
            }
        }
        // end initialise grid hash map

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
        head.rect = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, spacing, spacing};
        head.is_head = true;
        torso.rect = {head.rect.x-spacing, head.rect.y, spacing, spacing};
        snake.push_front(head);
        snake.push_front(torso);
        // update grid map
        grid_map[getGridId(head.rect.x, head.rect.y, spacing)] = 1;
        grid_map[getGridId(torso.rect.x, torso.rect.y, spacing)] = 1;
        // end test snake


        // test food
        food snake_food;
        int x = static_cast<int>(0 + spacing*pow(snake_food.scale,2)); // snake_food.scale ^ 2 centers coordinate
        int y = static_cast<int>(0 + spacing*pow(snake_food.scale,2));
        int w = static_cast<int>(spacing*snake_food.scale);
        int h = static_cast<int>(spacing*snake_food.scale);
        snake_food.rect = {x, y, w, h};
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
            if (frame_count % speed == 0) // artificially slow down the movement update allow the snake to move cell by cell in the grid at a playable speed
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

                // update grid map
                // update grid map with posiiton of new_head
                grid_map[getGridId(snake.back().rect.x, snake.back().rect.y, spacing)] = 1;
                // set cell previously ocupied by the snake tail as unocupied (i.e = 0)
                grid_map[getGridId(snake.front().rect.x, snake.front().rect.y, spacing)] = 0;

                // check for head/body collsiion 
                // iterate from tail to (head - 2) and check for collison with head (Game over condition)
                for (it = snake.begin(); it != std::next(snake.end(), -2); ++it){
                    if (SDL_HasIntersection(&(snake.back().rect), &(it->rect))){
                        // TODO: Transition to game over screen and display score
                        printf("Game Over!\nScore: %d\n", snake.size()-1);
                        quit = true;
                    }
                }

                // check for collision between head and food
                if (SDL_HasIntersection(&(snake.back().rect), &(snake_food.rect)))
                {
                    // add new body segment
                    // TODO: Fix bug mentioned below
                    // kind of buggy since direction refers to head direction not tail direction
                    int newX = (direction.x != 0) ? snake.front().rect.x + (spacing * -1 * direction.x) : snake.front().rect.x;
                    int newY = (direction.y != 0) ? snake.front().rect.y + (spacing * -1 * direction.y) : snake.front().rect.y;
                    snake.push_front((body){newX, newY, spacing, spacing});
                    grid_map[getGridId(newX, newY, spacing)] = 1;

                    // find new location for food rect (any cell unocupied by snake)
                    int cellX = -1;
                    int cellY = -1;
                    bool cell_found = false;
                    // Warning: currently NO end conditon!! always assumes empty cell is available for food
                    // solution could be to iteratre through the grid_map before hand and check an unoccupied cell exist (i.e 0 in grid_map)
                    while (!cell_found)
                    {
                        // keep guessing random cells on the grid until a cell is found which is not occupied by a snake body element (i.e grid_map = 0
                        // at that cell)
                        int hRange = SCREEN_WIDTH / spacing;
                        int vRange = SCREEN_HEIGHT / spacing;
                        int hGuess = rand() % hRange;
                        int vGuess = rand() % vRange;

                        if (grid_map.find(std::make_tuple(hGuess, vGuess)) != grid_map.end())
                        {
                            if (grid_map.at(std::make_tuple(hGuess, vGuess)) == 0)
                            {
                                cellX = hGuess * spacing;
                                cellY = vGuess * spacing;
                                cell_found = true; // break loop
                            }
                        }
                    }
                    snake_food.rect.x = static_cast<int>(cellX + (spacing * pow(snake_food.scale, 2)));
                    snake_food.rect.y = static_cast<int>(cellY + (spacing * pow(snake_food.scale, 2)));
                }
            }

            // clear screen
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF); // black
            SDL_RenderClear(gRenderer);

            // draw grid lines
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF); // white
            for (int i = spacing; i < SCREEN_WIDTH; i += spacing)
            {
                SDL_RenderDrawLine(gRenderer, i, 0, i, SCREEN_HEIGHT);
            }
            for (int j = spacing; j < SCREEN_HEIGHT; j += spacing)
            {
                SDL_RenderDrawLine(gRenderer, 0, j, SCREEN_WIDTH, j);
            }

            // render food
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF); // blue
            SDL_RenderFillRect(gRenderer, &(snake_food.rect));

            // render snake
            for (it = snake.begin(); it != snake.end(); ++it)
            {
                // set draw color
                if (it->is_head)
                {
                    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF); // red
                }
                else
                {
                    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF); // green
                }
                // render rect
                SDL_RenderFillRect(gRenderer, &(it->rect));
            }

            // Render frame
            SDL_RenderPresent(gRenderer);
            frame_count++;
        }
    }
    close();
    return 0;
}