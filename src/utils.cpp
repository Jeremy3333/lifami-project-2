// include my libraries
#include "Utils.hpp"
#include "Math.hpp"

// return the position where to draw a vector on the screen if centerDraw is the center of the screen
Vector2f getDrawPosition(Vector2f pos, Vector2f centerDraw)
{
    return initVector2f(pos.x - centerDraw.x + (LEFT_MENU_WIDTH / 2 + (WINDOW_WIDTH / 2)), pos.y - centerDraw.y + (WINDOW_HEIGHT / 2));
}

// return the position of the planet from the screen position
Vector2f getPositionFromScreenPosition(Vector2f centerDraw, Vector2f screenPosition)
{
    return initVector2f(screenPosition.x + centerDraw.x - (LEFT_MENU_WIDTH / 2 + (WINDOW_WIDTH / 2)), screenPosition.y + centerDraw.y - (WINDOW_HEIGHT / 2));
}

// true if the Vector is on the circle
bool isOnCircle(Vector2f center, float radius, Vector2f pos)
{
    return distance(center, pos) <= radius;
}

// true if the Vector pos is on the rect
bool isOnRect(Vector2f rectPos, Vector2f size, Vector2f pos)
{
    return pos.x >= rectPos.x && pos.x <= rectPos.x + size.x && pos.y >= rectPos.y && pos.y <= rectPos.y + size.y;
}