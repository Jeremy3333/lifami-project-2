#pragma once

// include public library
#include <SDL2/SDL.h>

// include private library (in include folder)
#include "Math.hpp"

// define global variables
#define MAX_PLANETS 10
#define G 6.67408e-11
#define MAX_TRACE_LENGTH 2000

// define structs
struct Planet {
    Vector2f position;
    Vector2f velocity;
    double mass;
    double radius;
    SDL_Color color;
    bool moveable;
    Vector2f Traces[MAX_TRACE_LENGTH];
    int TraceIndex;
    SDL_Color TraceColor;
    bool grabbed;
};

struct Galaxy {
    Planet planets[MAX_PLANETS];
    int nbPlanets;
    int selectedPlanet;
};

//init a planet
Planet initPlanet(Vector2f position, Vector2f velocity, double mass, double radius, SDL_Color color, bool moveable, SDL_Color TraceColor);