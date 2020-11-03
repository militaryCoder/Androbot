#pragma once

template <typename T>
class Range
{
public:
    Range() = default;
    Range(T from, T to)
        : from(from), to(to) { }
    Range(const Range &) = default;

    T from;
    T to;
};