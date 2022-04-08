// include public libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cmath>

// include private libraries (in include folder)
#include "RenderWindow.hpp"
#include "math.hpp"

#define MAX_PLANETS 10
#define TIMESTEPS_MULTIPLIER 1

#define WINDOW_TITLE "LIFAMI"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define G 6.67408e-11

/*
 * Debug:
 * g++ -c src/*.cpp -std=c++14 -g -Wall -m64 -I include -I C:/SDL2-w64/include  && g++ *.o -o bin/debug/main -L C:/SDL2-w64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image && start bin/debug/main
 *
 *Release
 * g++ -c src/*.cpp -std=c++14 -O3 -Wall -m64 -I include -I C:/SDL2-w64/include  && g++ *.o -o bin/release/main -s -L C:/SDL2-w64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image && start bin/release/main
 */

struct planet {
    Vector2f position;
    Vector2f velocity;
    double mass;
    double radius;
    SDL_Color color;
    bool moveable;
};

struct galaxy {
    planet planets[MAX_PLANETS];
    int numPlanets;
};

//init a Vector2f
Vector2f initVector2f(double x, double y) {
    Vector2f v;
    v.x = x;
    v.y = y;
    return v;
}

//init galaxy with 3 planets
void init(galaxy &g) {
    g.numPlanets = 4;
    g.planets[0].position = initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    g.planets[0].velocity = initVector2f(0, 0);
    g.planets[0].mass = 20000000000000;
    g.planets[0].radius = 10;
    g.planets[0].color = {255, 255, 255, 255};
    g.planets[0].moveable = false;

    g.planets[1].position = initVector2f(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2);
    g.planets[1].velocity = initVector2f(0, 20);
    g.planets[1].mass = 2000000000;
    g.planets[1].radius = 5;
    g.planets[1].color = {255, 255, 100, 255};
    g.planets[1].moveable = true;

    g.planets[2].position = initVector2f(WINDOW_WIDTH / 2 + 150, WINDOW_HEIGHT / 2);
    g.planets[2].velocity = initVector2f(0, -20);
    g.planets[2].mass = 2000000000;
    g.planets[2].radius = 5;
    g.planets[2].color = {255, 255, 100, 255};
    g.planets[2].moveable = true;

    g.planets[3].position = initVector2f(WINDOW_WIDTH / 2 + 160, WINDOW_HEIGHT / 2);
    g.planets[3].velocity = initVector2f(0, -17);
    g.planets[3].mass = 200000;
    g.planets[3].radius = 3;
    g.planets[3].color = {255, 255, 100, 255};
    g.planets[3].moveable = true;
}

void draw(galaxy g, RenderWindow &window) {
    window.color(0, 0, 0, 255);
    window.drawBackground();

    //draw all the planets of a galaxy with window.fillCircle
    for (int i = 0; i < g.numPlanets; i++) {
        window.color(g.planets[i].color.r, g.planets[i].color.g, g.planets[i].color.b, g.planets[i].color.a);
        window.fillCircle(g.planets[i].position.x, g.planets[i].position.y, g.planets[i].radius);
    }
}

//calculate the distance between two Vector2f
double distance(Vector2f v1, Vector2f v2) {
    return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
}

//calculate the force between two planets
Vector2f gForce(planet p1, planet p2) {
    Vector2f force;
    double dist = distance(p1.position, p2.position);
    double forceMagnitude = (G * p1.mass * p2.mass) / pow(dist, 2);
    force.x = forceMagnitude * (p2.position.x - p1.position.x) / dist;
    force.y = forceMagnitude * (p2.position.y - p1.position.y) / dist;
    return force;
}

// calculate the force between all planets with gForce function
void calculateForces(galaxy &g) {
    for (int i = 0; i < g.numPlanets; i++) {
        for (int j = 0; j < g.numPlanets; j++) {
            if (i != j) {
                g.planets[i].velocity = g.planets[i].velocity + gForce(g.planets[i], g.planets[j]) / g.planets[i].mass;
            }
        }
    }
}

// verifie if the left click is on a planet
bool isOnPlanet(galaxy g, Vector2f mousePos) {
    for (int i = 0; i < g.numPlanets; i++) {
        if (distance(g.planets[i].position, mousePos) < g.planets[i].radius) {
            return true;
        }
    }
    return false;
}

void update(float timeStepSeconds, galaxy &g) {
    timeStepSeconds *= TIMESTEPS_MULTIPLIER;
    calculateForces(g);

    //update all the planets of a galaxy
    for (int i = 0; i < g.numPlanets; i++)
    {
        if (g.planets[i].moveable)
        {
            g.planets[i].position.x += g.planets[i].velocity.x * timeStepSeconds;
            g.planets[i].position.y += g.planets[i].velocity.y * timeStepSeconds;
        }
    }

    //if the left click is on a planet, change the position of the planet
    int x, y;
    if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        Vector2f mousePos = initVector2f(x, y);
        if (isOnPlanet(g, mousePos))
        {
            for (int i = 0; i < g.numPlanets; i++)
            {
                if (distance(g.planets[i].position, mousePos) < g.planets[i].radius)
                {
                    g.planets[i].position = mousePos;
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    // initialize SDL
    RenderWindow window(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

    // initialize variables
    SDL_Event event;
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    bool quit = false;
    galaxy g;

    // initialize world
    init(g);

    // load media

    // main loop
    while (!quit)
    {
        float startTicks = SDL_GetTicks();
        float timeStepS = 1.0f / window.getRefreshRate();
        float timeStepMs = 1000.0f / window.getRefreshRate();
        SDL_PumpEvents();

        // get input
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || state[SDL_SCANCODE_ESCAPE])
                quit = true;
        }

        // clear screen
        window.clear();
        // update world
        update(timeStepS, g);
        // draw
        draw(g, window);

        window.display();

        float frameTicks = SDL_GetTicks() - startTicks;

        if (frameTicks < timeStepMs)
            SDL_Delay(timeStepMs - frameTicks);
    }

    // clean up memory
    window.cleanUp();
    return 0;
}