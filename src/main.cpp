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
    g.numPlanets = 3;
    g.planets[0].position = initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    g.planets[0].velocity = initVector2f(0, 0);
    g.planets[0].mass = 100000000000000000000000000000;
    g.planets[0].radius = 5;
    g.planets[0].color = {255, 255, 255, 255};
    g.planets[0].moveable = false;

    g.planets[1].position = initVector2f(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2);
    g.planets[1].velocity = initVector2f(0, 0);
    g.planets[1].mass = 100000000000000000000000000000;
    g.planets[1].radius = 5;
    g.planets[1].color = {255, 255, 100, 255};
    g.planets[1].moveable = true;

    g.planets[2].position = initVector2f(WINDOW_WIDTH / 2 + 150, WINDOW_HEIGHT / 2);
    g.planets[2].velocity = initVector2f(0, 0);
    g.planets[2].mass = 100000000000000000000000000000;
    g.planets[2].radius = 5;
    g.planets[2].color = {255, 255, 100, 255};
    g.planets[2].moveable = true;
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

//calculate the force between two planets
Vector2f gForce(planet p1, planet p2) {
    Vector2f g_force;
    double G = 6.67408e-11;
    double distance = sqrt(pow(p1.position.x - p2.position.x, 2) + pow(p1.position.y - p2.position.y, 2));
    double forceMagnitude = G * p1.mass * p2.mass / pow(distance, 2);
    g_force.x = forceMagnitude * (p1.position.x - p2.position.x) / distance;
    g_force.y = forceMagnitude * (p1.position.y - p2.position.y) / distance;
    return g_force;
}

void update(float timeStepSeconds, galaxy &g) {
    timeStepSeconds *= TIMESTEPS_MULTIPLIER;

    //calculate the force between all planets with gForce function
    for (int i = 0; i < g.numPlanets; i++)
    {
        for (int j = i + 1; j < g.numPlanets; j++)
        {
            Vector2f force = gForce(g.planets[i], g.planets[j]);
            g.planets[i].velocity.x += (force.x / g.planets[i].mass) * timeStepSeconds;
            g.planets[i].velocity.y += (force.y / g.planets[i].mass) * timeStepSeconds;
            g.planets[j].velocity.x -= (force.x / g.planets[j].mass) * timeStepSeconds;
            g.planets[j].velocity.y -= (force.y / g.planets[j].mass) * timeStepSeconds;
        }
    }

    //update all the planets of a galaxy
    for (int i = 0; i < g.numPlanets; i++)
    {
        if (g.planets[i].moveable)
        {
            g.planets[i].position.x += g.planets[i].velocity.x * timeStepSeconds;
            g.planets[i].position.y += g.planets[i].velocity.y * timeStepSeconds;
            std::cout << "velocity: " << g.planets[i].velocity.x << " " << g.planets[i].velocity.y << std::endl;
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