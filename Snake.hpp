#pragma once
#include <list>
#include <SDL2/SDL.h>

struct BodySegment
{
    SDL_Rect rect;
    bool is_head = false;

    BodySegment() {};
    BodySegment(bool is_head)
    {
        this->is_head = is_head;
    }
    BodySegment(int x, int y, int w, int h)
    {
        this->rect.x = x;
        this->rect.y = y;
        this->rect.w = w;
        this->rect.h = h;
    }
};

struct Direction
{
    int x = 1;
    int y = 0;
};

class Snake
{
    private:
    BodySegment head;
    BodySegment torso;

    public:
    std::list<BodySegment> snakeBody;
    std::list<BodySegment>::iterator snakeBodyIterator; 
    Direction direction;


    Snake();
    Snake(const int screenWidth, const int screenHeight, const int width, const int height);
    void render(SDL_Renderer* renderer);
};