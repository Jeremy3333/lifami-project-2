#pragma once

// include my libraries
#include "Math.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define LEFT_MOUSE_BUTTON 1
#define RIGHT_MOUSE_BUTTON 4

Vector2f getDrawPosition(Vector2f pos, Vector2f centerDraw);
Vector2f getPositionFromScreenPosition(Vector2f centerDraw, Vector2f screenPosition);
bool isOnCircle(Vector2f center, float radius, Vector2f pos);
bool isOnRect(Vector2f rectPos, Vector2f size, Vector2f pos);