#pragma once

#include <iostream>

#include "Coordinate2d.hpp"

template <typename ValType>
struct Point
{
    ValType value;
    Coordinate2d<uint> c;

    template <typename ValT>
    friend std::ostream &operator<<(std::ostream &out, const Point<ValT> &p);
};

template <typename ValT>
std::ostream &operator<<(std::ostream &out, const Point<ValT> &p)
{
    out << '(' << p.c.x << ',' << p.c.y << ") [" << p.value << ']';

    return out;
}