#pragma once
#include <iostream>

struct Vector2f
{
    float x, y;
    void print()
    {
        std::cout << "x: " << x << " y: " << y << std::endl;
    }
};