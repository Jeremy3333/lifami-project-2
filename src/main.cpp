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
#define MAX_TRACE_LENGTH 1000

#define WINDOW_TITLE "LIFAMI"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define NUM_PAGES 2

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
    Vector2f Traces[MAX_TRACE_LENGTH];
    int TraceIndex;
};

struct button {
    Vector2f position;
    Vector2f size;
    SDL_Color color;
    bool pressed;
};

struct galaxy {
    planet planets[MAX_PLANETS];
    int nbPlanets;
    button nextPage;
};

//init a Vector2f
Vector2f initVector2f(double x, double y) {
    Vector2f v;
    v.x = x;
    v.y = y;
    return v;
}

//init a planet
planet initPlanet(Vector2f position, Vector2f velocity, double mass, double radius, SDL_Color color, bool moveable)
{
    planet p;
    p.position = position;
    p.velocity = velocity;
    p.mass = mass;
    p.radius = radius;
    p.color = color;
    p.moveable = moveable;
    p.TraceIndex = 0;
    for(int i = 0; i < 100; i++)
    {
        p.Traces[i] = position;
    }
    return p;
}

//init galaxy with 4 planets
void init(galaxy &g) {
    g.nbPlanets = 4;
    g.planets[0] = initPlanet(initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), initVector2f(0, 0), 20000000000000, 10, {255, 255, 255, 255}, false);
    g.planets[1] = initPlanet(initVector2f(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2), initVector2f(0, 20), 2000000000, 5, {255, 255, 100, 255}, true);
    g.planets[2] = initPlanet(initVector2f(WINDOW_WIDTH / 2 + 150, WINDOW_HEIGHT / 2), initVector2f(0, -20), 2000000000, 5, {255, 255, 100, 255}, true);
    g.planets[3] = initPlanet(initVector2f(WINDOW_WIDTH / 2 + 160, WINDOW_HEIGHT / 2), initVector2f(0, -17), 200000, 3,{255, 255, 100, 255}, true);

    //init button
    g.nextPage.position = initVector2f(20, WINDOW_HEIGHT - 50);
    g.nextPage.size = initVector2f(60, 30);
    g.nextPage.color = {255, 0, 0, 255};
    g.nextPage.pressed = false;
}

void draw(galaxy g, RenderWindow &window) {
    window.color(0, 0, 0, 255);
    window.drawBackground();

    //draw all the planets of a galaxy with window.fillCircle
    for (int i = 0; i < g.nbPlanets; i++) {
        window.color(g.planets[i].color.r, g.planets[i].color.g, g.planets[i].color.b, g.planets[i].color.a);
        window.fillCircle(g.planets[i].position.x, g.planets[i].position.y, g.planets[i].radius);
        // draw the trace
        for(int j = 0; j < MAX_TRACE_LENGTH; j++)
        {
            window.fillCircle(g.planets[i].Traces[j].x, g.planets[i].Traces[j].y, 1);
        }
    }

    //draw the button
    window.color(g.nextPage.color.r, g.nextPage.color.g, g.nextPage.color.b, g.nextPage.color.a);
    window.fillRect(g.nextPage.position.x, g.nextPage.position.y, g.nextPage.size.x, g.nextPage.size.y);
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
    for (int i = 0; i < g.nbPlanets; i++) {
        for (int j = 0; j < g.nbPlanets; j++) {
            if (i != j) {
                g.planets[i].velocity = g.planets[i].velocity + gForce(g.planets[i], g.planets[j]) / g.planets[i].mass;
            }
        }
    }
}

// verifie if the left click is on a planet
bool isOnPlanet(galaxy g, Vector2f mousePos) {
    for (int i = 0; i < g.nbPlanets; i++) {
        if (distance(g.planets[i].position, mousePos) < g.planets[i].radius) {
            return true;
        }
    }
    return false;
}

//verifies if the mouse click the rect button
bool isOnButton(galaxy g, Vector2f mousePos) {
    if (distance(g.nextPage.position, mousePos) < g.nextPage.size.x / 2) {
        return true;
    }
    return false;
}

void updateIndex0(float timeStepSeconds, galaxy &g)
{
    calculateForces(g);

    // update all the planets of a galaxy
    for (int i = 0; i < g.nbPlanets; i++)
    {
        if (g.planets[i].moveable)
        {
            g.planets[i].position.x += g.planets[i].velocity.x * timeStepSeconds;
            g.planets[i].position.y += g.planets[i].velocity.y * timeStepSeconds;
            g.planets[i].Traces[g.planets[i].TraceIndex] = g.planets[i].position;
            g.planets[i].TraceIndex++;
            if (g.planets[i].TraceIndex >= MAX_TRACE_LENGTH)
            {
                g.planets[i].TraceIndex = 0;
            }
        }
    }

    int x, y;
    if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        Vector2f mousePos = initVector2f(x, y);
        // if the left click is on a planet, change the position of the planet
        if (isOnPlanet(g, mousePos))
        {
            for (int i = 0; i < g.nbPlanets; i++)
            {
                if (distance(g.planets[i].position, mousePos) < g.planets[i].radius)
                {
                    g.planets[i].position = mousePos;
                }
            }
        }
    }
}

void update(float timeStepSeconds, galaxy &g, int &index)
{
    timeStepSeconds *= TIMESTEPS_MULTIPLIER;

    if(index == 0)
    {
        updateIndex0(timeStepSeconds, g);
    }

    int x, y;
    if (SDL_GetMouseState(&x, &y) == SDL_BUTTON_LEFT)
    {
        std::cout << "index = " << index << std::endl;
        Vector2f mousePos = initVector2f(x, y);
        //if the left click is on the button, change the page
        if (isOnButton(g, mousePos) && g.nextPage.pressed == false)
        {
            g.nextPage.pressed = true;
            index++;
        }
    }
    //if the index is supperieur to the number of pages - 1, change the index to 0
    if (index > NUM_PAGES - 1)
    {
        index = 0;
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
    int index = 0;

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

        // update game
        update(timeStepS, g, index);

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