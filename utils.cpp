#include "utils.hpp"


template <typename L>
void print_list(std::list<L> li)
{
    for (auto const item: li)
    {
        printf("Rectangle:\tX: %d, Y: %d\n", item.rect.x, item.rect.y);
    }
}

std::tuple<int, int> getGridId(int x, int y, int spacing)
{
    int gridX = x / spacing;
    int gridY = y / spacing;
    
    //return std::tuple<int, int> 
    return std::make_tuple(gridX, gridY);
}



///######################################################## CustomTextWrapper Implementation #######################################################
CustomTextWrapper::CustomTextWrapper() {}
CustomTextWrapper::CustomTextWrapper(std::string text, TTF_Font *font, SDL_Color colour)
{
    this->text = text;
    this->font = font;
    this->colour = colour;
}

void CustomTextWrapper::setFont(unsigned char fontSize)
{
    if (font != nullptr)
    {
        printf("Error font already set");
        return;
    }

    this->font = TTF_OpenFont("data/fonts/menu_font.ttf", fontSize);
}

void CustomTextWrapper::renderText(const int x, const int y, SDL_Renderer *renderer)
{
    this->texture.loadFromRenderedText(this->text, this->colour, this->font, renderer);
    this->texture.render(x, y, renderer, NULL);
}

CustomTextWrapper::~CustomTextWrapper()
{
    font = nullptr;
}
///######################################################## End CustomTextWrapper Implementation #######################################################