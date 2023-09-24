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

CustomTextWrapper::CustomTextWrapper(std::string text, TTF_Font *font, SDL_Color colour, CustomReturnCode (*onClickFunctionPtr)())
{
    this->text = text;
    this->font = font;
    this->colour = colour;
    this->bindOnClick(onClickFunctionPtr);

    // test code
    //printf("IN CTW Constructor");
    //printf("IN CTW Constructor After: test text\n: %s\n", this->test.c_str());
    //this->texture.loadFromRenderedText(this->text, this->colour, this->font, renderer);
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

void CustomTextWrapper::setBoundingRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    if (this->boundingRect.x == -1 and this->boundingRect.y == -1)
    {
        this->boundingRect.x = x;
        this->boundingRect.y = y;
        this->boundingRect.w = w;
        this->boundingRect.h = h;
    }
}

//const SDL_Rect* CustomTextWrapper::getBoundingRect()
//{
//    //printf("Before getRect: x: %d, y: %d, w: %d, h: %d\n", boundingRect.x, boundingRect.y, boundingRect.w, boundingRect.h);
//    return &(this->boundingRect); 
//}

void CustomTextWrapper::renderText(const int x, const int y, SDL_Renderer *renderer)
{
    this->texture.loadFromRenderedText(this->text, this->colour, this->font, renderer);
    this->boundingRect = {x, y, this->texture.getWidth(), this->texture.getHeight()};
    this->texture.render(x, y, renderer, NULL);
}

void CustomTextWrapper::renderTextHorizontallyCentered(const uint16_t windowWidth, const uint16_t y, SDL_Renderer* renderer)
{
    this->texture.loadFromRenderedText(this->text, this->colour, this->font, renderer);

    // top left x coord of the text texture when centered horizontally on the screen.
    int x = static_cast<int>(0.5f * (windowWidth - this->texture.getWidth()));
    this->setBoundingRect(x, y, this->texture.getWidth(), this->texture.getHeight());
    //this->boundingRect = {x, y, this->texture.getWidth(), this->texture.getHeight()};

    this->texture.render(this->boundingRect.x, this->boundingRect.y, renderer, NULL);

    //test code
    //printf("In Render: X: %d, Y: %d\n", this->boundingRect.x, this->boundingRect.y);
}

void CustomTextWrapper::bindOnClick(CustomReturnCode (*funcPtr)())
{
    this->onClickFunctionPtr = funcPtr;
}

CustomReturnCode CustomTextWrapper::onClick()
{
    if (this->onClickFunctionPtr != nullptr)
    {
        return this->onClickFunctionPtr();
    }
    else
    // button not binded to anything do nothing and just continue as normal
    {
        return RUN;
    }

}

CustomTextWrapper::CustomTextWrapper(const CustomTextWrapper &oldObject)
{
    //printf("In copy\n");
    //std::string text = "default text";
    //TTF_Font* font = nullptr;
    //SDL_Color colour = {0x00, 0x00, 0x00}; // black
    //LTexture texture;
    //SDL_Rect boundingRect = {-1, -1, -1, -1}; // value to signal the struct is uninitialised;
    //std::string test;
    //CustomReturnCode (*onClickFunctionPtr)() = nullptr;

    text = oldObject.text;
    font = oldObject.font; // may cause an error may need to use TTF_OpenFont again instead.
    colour = oldObject.colour;
    texture = oldObject.texture;
    //printf("In copy: X: %d, Y: %d\n", oldObject.boundingRect.x, oldObject.boundingRect.y);
    boundingRect = {oldObject.boundingRect.x, oldObject.boundingRect.y, oldObject.boundingRect.w, oldObject.boundingRect.h};
    CustomReturnCode (*onClickFunctionPtr)() = oldObject.onClickFunctionPtr;
   
}

CustomTextWrapper::~CustomTextWrapper()
{
    //printf("Destructor called: %s\n", this->text.c_str());
    //printf("In Destructor: X: %d, Y: %d\n", this->boundingRect.x, this->boundingRect.y);
    this->onClickFunctionPtr = nullptr;
    this->font = nullptr;
}
///######################################################## End CustomTextWrapper Implementation #######################################################