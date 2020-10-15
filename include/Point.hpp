#pragma once

#include <iostream>

typedef unsigned int uint;

struct Coordinate2d
{
    uint x;
    uint y;
};

template <typename ValType>
struct Point
{
    ValType value;
    Coordinate2d c;

    template <typename ValT>
    friend std::ostream &operator<<(std::ostream &out, const Point<ValT> &p);
};

template <typename ValT>
std::ostream &operator<<(std::ostream &out, const Point<ValT> &p)
{
    out << '(' << p.c.x << ',' << p.c.y << ')' << " [" << p.value << ']';

    return out;
}