#include "ScreenInterface.hpp"



ScreenInterface::ScreenInterface() {};

ScreenInterface::ScreenInterface(const uint16_t windowWidth, const uint16_t windowHeight)
{
    //this->manager = manager;
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
}