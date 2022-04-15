// include public libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cmath>

// include private libraries (in include folder)
#include "RenderWindow.hpp"
#include "math.hpp"


// define global variables
#define MAX_PLANETS 10
#define TIMESTEPS_MULTIPLIER 1
#define MAX_TRACE_LENGTH 2000
#define WINDOW_TITLE "LIFAMI-Project"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define NUM_PAGES 2
#define G 6.67408e-11
#define LEFT_MOUSE_BUTTON 1
#define RIGHT_MOUSE_BUTTON 4

/*
 * Mon objectif est de faire un programme qui permet de faire une simulation d'un systeme solaire en 2D avec des planetes et des lunes.
 * Il y aura posibilité de faire des mettre en pause la simulation et de deplacer les planetes et lunes(bouton rouge en bas a gauche) le bouton bleu reset la galaxie mais pas la planete selectionnée.
 * de selectionner les planetes avec la souris et de voir la simulation centrée sur cette planete(clique droit de la souris) ou de changer leur position (clique gauche de la souris).
 * il y aura aussi un mode de trace qui permet de voir la trajectoire d'une planete.
 * et enfin si j'en ai la possibilité recréer une simulation complette du systeme solaire.
 * je m'excuse pour l'anglais en commentaire.
 */

/*
 * Debug:
 * g++ -c src/*.cpp -std=c++14 -g -Wall -m64 -I include -I C:/SDL2-w64/include  && g++ *.o -o bin/debug/main -L C:/SDL2-w64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image && start bin/debug/main
 *
 *Release
 * g++ -c src/*.cpp -std=c++14 -O3 -Wall -m64 -I include -I C:/SDL2-w64/include  && g++ *.o -o bin/release/main -s -L C:/SDL2-w64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image && start bin/release/main
 */

// define structs
struct planet {
    Vector2f position;
    Vector2f velocity;
    double mass;
    double radius;
    SDL_Color color;
    bool moveable;
    Vector2f Traces[MAX_TRACE_LENGTH];
    int TraceIndex;
    SDL_Color TraceColor;
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
    int selectedPlanet;
    button nextPage;
    button reset;
    Vector2f centerDraw;
};

//init a planet
planet initPlanet(Vector2f position, Vector2f velocity, double mass, double radius, SDL_Color color, bool moveable, SDL_Color TraceColor)
{
    planet p;
    p.position = position;
    p.velocity = velocity;
    p.mass = mass;
    p.radius = radius;
    p.color = color;
    p.moveable = moveable;
    p.TraceIndex = 0;
    Vector2f tracePos = initVector2f(-1000, -1000);

    //init traces
    for(int i = 0; i < MAX_TRACE_LENGTH; i++)
    {
        p.Traces[i] = tracePos;
    }
    p.TraceColor = TraceColor;
    return p;
}

// init all the trace of all the planet at (-100,-100)
void initTrace (galaxy &g)
{
    for(int i = 0; i < g.nbPlanets; i++)
    {
        for(int j = 0; j < MAX_TRACE_LENGTH; j++)
        {
            g.planets[i].Traces[j] = initVector2f(-100, -100);
        }
    }
}

//init galaxy with 4 planets and no selected planet
void init(galaxy &g) {
    g.nbPlanets = 4;
    g.planets[0] = initPlanet(initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), initVector2f(0, 0), 20000000000000, 10, {255, 255, 100, 255}, true, {255, 0, 0, 255});
    g.planets[1] = initPlanet(initVector2f(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2), initVector2f(0, 20), 20000000000, 5, {255, 255, 100, 255}, true, {255, 0, 0, 255});
    g.planets[2] = initPlanet(initVector2f(WINDOW_WIDTH / 2 + 150, WINDOW_HEIGHT / 2), initVector2f(0, -20), 200000000000, 7, {255, 255, 100, 255}, true, {255, 0, 0, 255});
    g.planets[3] = initPlanet(initVector2f(WINDOW_WIDTH / 2 + 160, WINDOW_HEIGHT / 2), initVector2f(0, -11), 200000, 3,{255, 255, 100, 255}, true , {255, 0, 0, 255});

    //init button next page
    g.nextPage.position = initVector2f(20, WINDOW_HEIGHT - 50);
    g.nextPage.size = initVector2f(60, 30);
    g.nextPage.color = {255, 0, 0, 255};
    g.nextPage.pressed = false;

    //init button reset
    g.reset.position = initVector2f(WINDOW_WIDTH - 80, WINDOW_HEIGHT - 50);
    g.reset.size = initVector2f(60, 30);
    g.reset.color = {0, 0, 255, 255};
    g.reset.pressed = false;

    g.selectedPlanet = -1;
    g.centerDraw = initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
}

//init galaxy with 4 planets but save the selected planet
void resetGalaxy(galaxy &g)
{
    g.nbPlanets = 4;
    g.planets[0] = initPlanet(initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), initVector2f(0, 0), 20000000000000, 10, {255, 255, 100, 255}, true, {255, 0, 0, 255});
    g.planets[1] = initPlanet(initVector2f(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2), initVector2f(0, 20), 20000000000, 5, {255, 255, 100, 255}, true, {255, 0, 0, 255});
    g.planets[2] = initPlanet(initVector2f(WINDOW_WIDTH / 2 + 150, WINDOW_HEIGHT / 2), initVector2f(0, -20), 200000000000, 7, {255, 255, 100, 255}, true, {255, 0, 0, 255});
    g.planets[3] = initPlanet(initVector2f(WINDOW_WIDTH / 2 + 160, WINDOW_HEIGHT / 2), initVector2f(0, -11), 200000, 3,{255, 255, 100, 255}, true , {255, 0, 0, 255});

    //init button next page
    g.nextPage.position = initVector2f(20, WINDOW_HEIGHT - 50);
    g.nextPage.size = initVector2f(60, 30);
    g.nextPage.color = {255, 0, 0, 255};
    g.nextPage.pressed = false;

    //init button reset
    g.reset.position = initVector2f(WINDOW_WIDTH - 80, WINDOW_HEIGHT - 50);
    g.reset.size = initVector2f(60, 30);
    g.reset.color = {0, 0, 255, 255};
    g.reset.pressed = false;

}

// return the position where to draw the planet on the screen if centerDraw is the center of the screen
Vector2f getDrawPosition(galaxy &g, int planetIndex) {
    return initVector2f(g.planets[planetIndex].position.x - g.centerDraw.x + (WINDOW_WIDTH/2), g.planets[planetIndex].position.y - g.centerDraw.y + (WINDOW_HEIGHT/2));
}

// return the position of the planet from the screen position
Vector2f getPositionFromScreenPosition(galaxy &g, Vector2f screenPosition) {
    return initVector2f(screenPosition.x + g.centerDraw.x - (WINDOW_WIDTH/2), screenPosition.y + g.centerDraw.y - (WINDOW_HEIGHT/2));
}

// draw the planet on the screen
void draw(galaxy g, RenderWindow &window) {
    window.color(0, 0, 0, 255);
    window.drawBackground();

    //draw all the planets of a galaxy with window.fillCircle
    for (int i = 0; i < g.nbPlanets; i++) {
        // draw the trace
        window.color(g.planets[i].TraceColor.r, g.planets[i].TraceColor.g, g.planets[i].TraceColor.b, g.planets[i].TraceColor.a);
        if(g.planets[i].moveable)
        {
            for(int j = 0; j < MAX_TRACE_LENGTH; j++)
            {
                window.fillCircle(g.planets[i].Traces[j].x, g.planets[i].Traces[j].y, 1);
            }
        }
        // draw the planet
        window.color(g.planets[i].color.r, g.planets[i].color.g, g.planets[i].color.b, g.planets[i].color.a);
        Vector2f drawPosition = getDrawPosition(g, i);
        window.fillCircle(drawPosition.x, drawPosition.y, g.planets[i].radius);
    }

    //draw the button next page
    window.color(g.nextPage.color.r, g.nextPage.color.g, g.nextPage.color.b, g.nextPage.color.a);
    window.fillRect(g.nextPage.position.x, g.nextPage.position.y, g.nextPage.size.x, g.nextPage.size.y);

    //draw the button reset
    window.color(g.reset.color.r, g.reset.color.g, g.reset.color.b, g.reset.color.a);
    window.fillRect(g.reset.position.x, g.reset.position.y, g.reset.size.x, g.reset.size.y);
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
bool isOnPlanet(planet p, Vector2f mousePos) {
    return distance(p.position, mousePos) < p.radius;
}

//verifies if the mouse click the button
bool isOnButton(button b, Vector2f mousePos) {
    return mousePos.x > b.position.x && mousePos.x < b.position.x + b.size.x && mousePos.y > b.position.y && mousePos.y < b.position.y + b.size.y;
}

// update the position of the planets (only if moveable)
void updateIndex0(float timeStepSeconds, galaxy &g)
{
    calculateForces(g);

    if(g.selectedPlanet == -1)
        g.centerDraw = initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    else
        g.centerDraw = g.planets[g.selectedPlanet].position;

    // update all the planets of a galaxy
    for (int i = 0; i < g.nbPlanets; i++)
    {
        if (g.planets[i].moveable)
        {
            g.planets[i].position.x += g.planets[i].velocity.x * timeStepSeconds;
            g.planets[i].position.y += g.planets[i].velocity.y * timeStepSeconds;
            g.planets[i].Traces[g.planets[i].TraceIndex] = getDrawPosition(g, i);
            g.planets[i].TraceIndex++;
            if (g.planets[i].TraceIndex >= MAX_TRACE_LENGTH)
            {
                g.planets[i].TraceIndex = 0;
            }
        }
    }

    // move a planet if the left click is on it
    int x, y;
    const Uint32 buttons = SDL_GetMouseState(&x, &y);
    if(buttons == LEFT_MOUSE_BUTTON)
    {
        Vector2f mousePos = initVector2f(x, y);
        mousePos = getPositionFromScreenPosition(g, mousePos);
        // if the left click is on a planet, change the position of the planet
        for (int i = 0; i < g.nbPlanets; i++)
        {
            if (isOnPlanet(g.planets[i], mousePos) && i != g.selectedPlanet)
            {
                g.planets[i].position = mousePos;
            }
        }
    }

    // change the selected planet if the right click is on a planet
    else if (buttons == RIGHT_MOUSE_BUTTON)
    {
        Vector2f mousePos = initVector2f(x, y);
        mousePos = getPositionFromScreenPosition(g, mousePos);
        // if the right click is on a planet, change the color of the planet
        for (int i = 0; i < g.nbPlanets; i++)
        {
            if (isOnPlanet(g.planets[i], mousePos))
            {
                g.selectedPlanet = i;
                initTrace(g);
            }
            else
            {
                g.planets[i].color = {255, 255, 100, 255};
            }
        }
    }

    // change the color of the selected planet
    g.planets[g.selectedPlanet].color = {0, 255, 0, 255};
}


void update(float timeStepSeconds, galaxy &g, int &index)
{
    // increase the timestep
    timeStepSeconds *= TIMESTEPS_MULTIPLIER;

    // update the position of the planets if it's the first index
    if(index == 0)
    {
        updateIndex0(timeStepSeconds, g);
    }

    // check if the button are clicked
    int x, y;
    const Uint32 buttons = SDL_GetMouseState(&x, &y);
    if (buttons == LEFT_MOUSE_BUTTON)
    {
        Vector2f mousePos = initVector2f(x, y);
        //if the left click is on the button next page, change the page
        if (isOnButton(g.nextPage, mousePos))
        {
            index++;
        }
        //if the left click is on the button reset, reset the galaxy
        else if (isOnButton(g.reset, mousePos))
        {
            resetGalaxy(g);
        }
    }
    else
    {
        g.nextPage.pressed = false;
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

    // main loop
    while (!quit)
    {
        // avoid the program to go too fast
        float startTicks = SDL_GetTicks();
        float timeStepS = 1.0f / window.getRefreshRate();
        float timeStepMs = 1000.0f / window.getRefreshRate();

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

        // update screen
        window.display();

        // wait for the next frame
        float frameTicks = SDL_GetTicks() - startTicks;
        if (frameTicks < timeStepMs)
            SDL_Delay(timeStepMs - frameTicks);
    }

    // clean up memory
    window.cleanUp();
    return 0;
}