// include public libraries
#include <cmath>

// include my libraries
#include "Math.hpp"

// init a Vector2f
Vector2f initVector2f(double x, double y)
{
    Vector2f v;
    v.x = x;
    v.y = y;
    return v;
}

Vector2f operator-(const Vector2f &lhs, const Vector2f &rhs)
{
    return initVector2f(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vector2f operator+(const Vector2f &lhs, const Vector2f &rhs)
{
    return initVector2f(lhs.x + rhs.x, lhs.y + rhs.y);
}

Vector2f operator+=(Vector2f &lhs, const Vector2f &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

Vector2f operator/(const Vector2f &lhs, float rhs)
{
    return initVector2f(lhs.x / rhs, lhs.y / rhs);
}

Vector2f operator*(const Vector2f &lhs, float rhs)
{
    return initVector2f(lhs.x * rhs, lhs.y * rhs);
}

Vector2f operator*(float rhs, const Vector2f &lhs)
{
    return initVector2f(lhs.x * rhs, lhs.y * rhs);
}

// calculate the distance between two Vector2f
double distance(Vector2f v1, Vector2f v2)
{
    return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
}