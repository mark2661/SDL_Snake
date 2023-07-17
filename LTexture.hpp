#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class LTexture
{
    public:
        LTexture();
        ~LTexture();

        bool loadFromRenderedText(std::string textureText, SDL_Color textColour, TTF_Font* font, SDL_Renderer* renderer);
        void free();
        void render(int x, int y, SDL_Renderer* renderer, SDL_Rect* clip=NULL);
        int getWidth();
        int getHeight();

    private:
        SDL_Texture* mTexture;
        int mWidth;
        int mHeight;
};
