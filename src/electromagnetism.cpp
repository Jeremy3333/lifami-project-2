// include my libraries
#include "Electromagnetism.hpp"
#include "Utils.hpp"

// init an electromagnetic particle
void initElectromagnetism(Electromagnetism &e, Vector2f position, Vector2f velocity, double radius, SDL_Color color)
{
    e.position = position;
    e.velocity = velocity;
    e.radius = radius;
    e.color = color;
}

// init an array of electromagnetic particles with 1 particules
void initElectromagnetismArray(ElectromagnetismArray &e)
{
    e.nbParticles = 1;
    initElectromagnetism(e.particles[0], initVector2f(0, 0), initVector2f(0, 0), 2, {255, 255, 255, 255});
    e.loaded = true;
}

void drawElectromagnetismArray(ElectromagnetismArray e, RenderWindow &window)
{
    for(int x = 0; x < e.nbParticles; x++)
    {
        window.color(e.particles[x].color.r, e.particles[x].color.g, e.particles[x].color.b, e.particles[x].color.a);
    }
}