#pragma once

// include public libraries
#include <SDL2/SDL.h>

// include my libraries
#include "Math.hpp"
#include "RenderWindow.hpp"

#define MAX_ELECTROMAGNETS 10
#define CASE_SIZE 10

// define struct of electromagnetic particle
struct Electromagnetism {
    Vector2f position;
    Vector2f velocity;
    double radius;
    SDL_Color color;
};

// define struct of many electromagnetic particles
struct ElectromagnetismArray {
    Electromagnetism particles[MAX_ELECTROMAGNETS];
    int nbParticles;
    bool loaded;
};

void initElectromagnetism(Electromagnetism &e, Vector2f position, Vector2f velocity, double radius, SDL_Color color);
void initElectromagnetismArray(ElectromagnetismArray &e);
void drawElectromagnetismArray(ElectromagnetismArray e, RenderWindow &window);
