#include "ScreenInterface.hpp"



ScreenInterface::ScreenInterface() {};

ScreenInterface::ScreenInterface(const uint16_t windowWidth, const uint16_t windowHeight, std::string screenType)
{
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    this->screenType = screenType;
}

std::string ScreenInterface::getType()
{
    return this->screenType;
}