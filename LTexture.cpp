#include "LTexture.hpp"

LTexture::LTexture()
{
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    free();
}

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColour, TTF_Font* font, SDL_Renderer* renderer)
{
    // delete pre-existing texture
    free();

    // render text to surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColour);
    if (textSurface == nullptr)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (mTexture == NULL)
        {
            printf("Unable to create texture from rendered surface! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            // get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        // free old surface
        SDL_FreeSurface(textSurface);
    }

    return mTexture != NULL;
}

void LTexture::free()
{
    // free texture if it exist
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

int LTexture::getWidth()
{
    return mWidth;
}


int LTexture::getHeight()
{
    return mHeight;
}

void LTexture::render(int x, int y, SDL_Renderer* renderer, SDL_Rect* clip)
{
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // render to screen
    SDL_RenderCopy(renderer, mTexture, clip, &renderQuad);
}
