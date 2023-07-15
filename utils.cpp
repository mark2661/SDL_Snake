#include "utils.hpp"
#include <stdio.h>
#include <list>


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

