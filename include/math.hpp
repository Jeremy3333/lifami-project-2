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

Vector2f initVector(float x, float y)
{
    Vector2f vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

Vector2f operator-(const Vector2f &lhs, const Vector2f &rhs)
{
    return initVector(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vector2f operator+(const Vector2f &lhs, const Vector2f &rhs)
{
    return initVector(lhs.x + rhs.x, lhs.y + rhs.y);
}

Vector2f operator/(const Vector2f &lhs, float rhs)
{
    return initVector(lhs.x / rhs, lhs.y / rhs);
}

Vector2f operator*(const Vector2f &lhs, float rhs)
{
    return initVector(lhs.x * rhs, lhs.y * rhs);
}
Vector2f operator*(float rhs, const Vector2f &lhs)
{
    return initVector(lhs.x * rhs, lhs.y * rhs);
}