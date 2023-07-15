#include "Snake.hpp"
#include <SDL2/SDL.h>


Snake::Snake(){}

Snake::Snake(const int x, const int y, const int w, const int h)
{
    this->head = BodySegment(true);
    this->torso = BodySegment();

     // initialise head position and size
    this->head.rect.x = x;
    this->head.rect.y = y;
    this->head.rect.w = w;
    this->head.rect.h = h;

    // initialise torso position and size (to the left of head)
    this->torso.rect.x = this->head.rect.x - w;
    this->torso.rect.y = this->head.rect.y;
    this->torso.rect.w = w;
    this->torso.rect.h = h;

    this->snakeBody.push_front(head);
    this->snakeBody.push_front(torso);
}

void Snake::render(SDL_Renderer* renderer)
{
    for (auto segment: this->snakeBody)
    {
        if (segment.is_head)
        {
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF); // red
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF); // green
        }

        SDL_RenderFillRect(renderer, &(segment.rect));
    }

}

