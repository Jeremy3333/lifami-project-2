#pragma once

// include public libraries
#include <SDL2/SDL.h> // SDL_Rect, SDL_Color, SDL_Point

// include my libraries
#include "Math.hpp"         // Vector2f,  initVector2f
#include "RenderWindow.hpp" // for the window

// define global variables
#define MAX_PLANETS 10        // max number of planets
#define G 6.67408e-11         // gravitational constant
#define MAX_TRACE_LENGTH 2000 // max length of the trace

// define structs
struct Planet
{
    Vector2f position;                 // position de la planète
    Vector2f velocity;                 // vélocité de la planète
    Vector2f acceleration;             // accélération de la planète
    double mass;                       // masse de la planète
    double radius;                     // rayon de la planète
    SDL_Color color;                   // couleur de la planète
    bool moveable;                     // si la planète est déplaçable ou non
    Vector2f Traces[MAX_TRACE_LENGTH]; // traces de la planète
    int TraceIndex;                    // index de la trace
    SDL_Color TraceColor;              // couleur de la trace
    bool grabbed;                      // si la planète est en train d'etre déplacée ou non
};

struct Galaxy
{
    Planet planets[MAX_PLANETS]; // tableau de planètes
    int nbPlanets;               // nombre de planètes
    int selectedPlanet;          // planète sélectionnée
    bool loaded;                 // si la galaxie est chargée ou non
    Vector2f centerDraw;         // centre de dessin
};

Planet initPlanet(Vector2f position, Vector2f velocity, double mass, double radius, SDL_Color color, bool moveable, SDL_Color TraceColor);
void initGalaxy(Galaxy &g);
void initTrace(Galaxy &g);
void resetGalaxy(Galaxy &g);
void drawGalaxy(Galaxy g, RenderWindow &window);
void calculateForces(Galaxy &g);
void updateGalaxy(float timeStepSeconds, Galaxy &g, bool pause);