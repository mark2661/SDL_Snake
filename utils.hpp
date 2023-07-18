#pragma once
#include <cstddef>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <list>
#include <tuple>
#include <string>
#include "LTexture.hpp"

template <typename L>
void print_list(std::list<L>);
std::tuple<int, int> getGridId(int, int, int);

// std::unordered_map cannot does not define a hash for tuples, therefore must define a custom hash function.
struct hash_tuple 
{
    template <class T1, class T2>

    size_t operator()(const std::tuple<T1, T2>& x) const
    {
        return std::get<0>(x) ^ std::get<1>(x);
    }

};

enum CustomReturnCode
{
    RUN,
    QUIT,
    START_MENU,
    GAME_OVER_MENU,
    GAME_SCREEN
};



struct CustomTextWrapper{
    std::string text = "";
    TTF_Font* font;
    SDL_Color colour = {0x00, 0x00, 0x00}; // black
    LTexture texture;
    SDL_Rect boundingRect;

    CustomTextWrapper();
    CustomTextWrapper(std::string text, TTF_Font *font, SDL_Color colour);
    void setFont(unsigned char fontSize);
    void renderText(const int x, const int y, SDL_Renderer* renderer);
    void renderTextCentered(const uint16_t windowWidth, const uint16_t y, SDL_Renderer* renderer);
    CustomReturnCode onClick();
    ~CustomTextWrapper();
};


