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