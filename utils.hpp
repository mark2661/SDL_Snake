# pragma once
#include <list>
#include <tuple>
#include <cstddef>


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

