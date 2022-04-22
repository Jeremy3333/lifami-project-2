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
 * lien du repo github : https://github.com/Jeremy3333/lifami-project-2
 */

/*
 *lien pour compiler (attention a bien mettre le chemin de votre sdl2 et sdl2_image et de le lancer dans le dossier du projet) :
 * Debug:
 * g++ -c src/*.cpp -std=c++14 -g -Wall -m64 -I include -I C:/SDL2-w64/include  && g++ *.o -o bin/debug/main -L C:/SDL2-w64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image && start bin/debug/main
 *
 *Release
 * g++ -c src/*.cpp -std=c++14 -O3 -Wall -m64 -I include -I C:/SDL2-w64/include  && g++ *.o -o bin/release/main -s -L C:/SDL2-w64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image && start bin/release/main
 */

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

struct Button {
    Vector2f position;
    Vector2f size;
    SDL_Color color;
    bool pressed;
};

struct Galaxy {
    Planet planets[MAX_PLANETS];
    int nbPlanets;
    int selectedPlanet;
};

struct All
{
    Galaxy g;
    Button pause;
    Button nextPage;
    Button reset;
    bool paused;
    Vector2f centerDraw;
};

//init a planet
Planet initPlanet(Vector2f position, Vector2f velocity, double mass, double radius, SDL_Color color, bool moveable, SDL_Color TraceColor)
{
    Planet p;
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
    p.grabbed = false;
    return p;
}

// init all the trace of all the planet at (-100,-100)
void initTrace (Galaxy &g)
{
    for(int i = 0; i < g.nbPlanets; i++)
    {
        for(int j = 0; j < MAX_TRACE_LENGTH; j++)
        {
            g.planets[i].Traces[j] = initVector2f(-100, -100);
        }
    }
}

// init a button
void initButton(Button &b, Vector2f position, Vector2f size, SDL_Color color)
{
    b.position = position;
    b.size = size;
    b.color = color;
    b.pressed = false;
}

//init galaxy with 4 planets and no selected planet
void initGalaxy(Galaxy &g) {
    g.nbPlanets = 4;
    g.planets[0] = initPlanet(initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), initVector2f(0, 0), 20000000000000, 10, {255, 255, 255, 255}, false, {255, 0, 0, 255});
    g.planets[1] = initPlanet(initVector2f(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2), initVector2f(0, 20), 20000000000, 5, {255, 255, 100, 255}, true, {255, 0, 0, 255});
    g.planets[2] = initPlanet(initVector2f(WINDOW_WIDTH / 2 + 150, WINDOW_HEIGHT / 2), initVector2f(0, -20), 200000000000, 7, {255, 255, 100, 255}, true, {255, 0, 0, 255});
    g.planets[3] = initPlanet(initVector2f(WINDOW_WIDTH / 2 + 160, WINDOW_HEIGHT / 2), initVector2f(0, -11), 200000, 3,{255, 255, 100, 255}, true , {255, 0, 0, 255});

    g.selectedPlanet = -1;
}

//init all
void initAll(All &all) {

    // init all the buttons
    initButton(all.pause, initVector2f(20, WINDOW_HEIGHT - 50), initVector2f(60, 30), {255, 0, 0, 255});
    initButton(all.nextPage, initVector2f((WINDOW_WIDTH / 2) - 30, WINDOW_HEIGHT - 50), initVector2f(60, 30), {0, 255, 0, 255});
    initButton(all.reset, initVector2f(WINDOW_WIDTH - 80, WINDOW_HEIGHT - 50), initVector2f(60, 30), {0, 0, 255, 255});

    all.centerDraw = initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    all.paused = false;

    // init galaxy
    initGalaxy(all.g);
}

//init galaxy with 4 planets but save the selected planet
void resetGalaxy(Galaxy &g)
{
    g.nbPlanets = 4;
    g.planets[0] = initPlanet(initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), initVector2f(0, 0), 20000000000000, 10, {255, 255, 100, 255}, true, {255, 0, 0, 255});
    g.planets[1] = initPlanet(initVector2f(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2), initVector2f(0, 20), 20000000000, 5, {255, 255, 100, 255}, true, {255, 0, 0, 255});
    g.planets[2] = initPlanet(initVector2f(WINDOW_WIDTH / 2 + 150, WINDOW_HEIGHT / 2), initVector2f(0, -20), 200000000000, 7, {255, 255, 100, 255}, true, {255, 0, 0, 255});
    g.planets[3] = initPlanet(initVector2f(WINDOW_WIDTH / 2 + 160, WINDOW_HEIGHT / 2), initVector2f(0, -11), 200000, 3,{255, 255, 100, 255}, true , {255, 0, 0, 255});
}

// return the position where to draw the planet on the screen if centerDraw is the center of the screen
Vector2f getDrawPosition(Galaxy g, int planetIndex, Vector2f centerDraw) {
    return initVector2f(g.planets[planetIndex].position.x - centerDraw.x + (WINDOW_WIDTH/2), g.planets[planetIndex].position.y - centerDraw.y + (WINDOW_HEIGHT/2));
}

// return the position of the planet from the screen position
Vector2f getPositionFromScreenPosition(Vector2f centerDraw, Vector2f screenPosition) {
    return initVector2f(screenPosition.x + centerDraw.x - (WINDOW_WIDTH/2), screenPosition.y + centerDraw.y - (WINDOW_HEIGHT/2));
}

//draw the galaxy if the index is 0
void drawGalaxy(Galaxy g, RenderWindow &window, Vector2f CenterDraw)
{
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
        Vector2f drawPosition = getDrawPosition(g, i, CenterDraw);
        window.fillCircle(drawPosition.x, drawPosition.y, g.planets[i].radius);
    }
}

// draw the planet on the screen
void draw(All all, RenderWindow &window, int Index) {

    // draw the galaxy if index is 0
    if (Index == 0) {
        drawGalaxy(all.g, window, all.centerDraw);
    }

    //draw the button pause
    window.color(all.pause.color.r, all.pause.color.g, all.pause.color.b, all.pause.color.a);
    window.fillRect(all.pause.position.x, all.pause.position.y, all.pause.size.x, all.pause.size.y);

    //draw the button next page
    window.color(all.nextPage.color.r, all.nextPage.color.g, all.nextPage.color.b, all.nextPage.color.a);
    window.fillRect(all.nextPage.position.x, all.nextPage.position.y, all.nextPage.size.x, all.nextPage.size.y);

    //draw the button reset
    window.color(all.reset.color.r, all.reset.color.g, all.reset.color.b, all.reset.color.a);
    window.fillRect(all.reset.position.x, all.reset.position.y, all.reset.size.x, all.reset.size.y);
}

//calculate the force between two planets
Vector2f gForce(Planet p1, Planet p2) {
    Vector2f force;
    double dist = distance(p1.position, p2.position);
    double forceMagnitude = (G * p1.mass * p2.mass) / pow(dist, 2);
    force.x = forceMagnitude * (p2.position.x - p1.position.x) / dist;
    force.y = forceMagnitude * (p2.position.y - p1.position.y) / dist;
    return force;
}


// calculate the force between all planets with gForce function
void calculateForces(Galaxy &g) {
    for (int i = 0; i < g.nbPlanets; i++) {
        for (int j = 0; j < g.nbPlanets; j++) {
            if (i != j) {
                g.planets[i].velocity = g.planets[i].velocity + gForce(g.planets[i], g.planets[j]) / g.planets[i].mass;
            }
        }
    }
}

// verifie if the left click is on a planet
bool isOnPlanet(Planet p, Vector2f mousePos) {
    return distance(p.position, mousePos) < p.radius;
}

//verifies if the mouse click the button
bool isOnButton(Button b, Vector2f mousePos) {
    return mousePos.x > b.position.x && mousePos.x < b.position.x + b.size.x && mousePos.y > b.position.y && mousePos.y < b.position.y + b.size.y;
}

// update the position of the planets (only if moveable)
void updateGalaxy(float timeStepSeconds, Galaxy &g, Vector2f &centerDraw, bool pause)
{
    if(g.selectedPlanet == -1)
        centerDraw = initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    else
        centerDraw = g.planets[g.selectedPlanet].position;

    if(!pause)
    {
        calculateForces(g);

        // update all the planets of a galaxy
        for (int i = 0; i < g.nbPlanets; i++)
        {
            if (g.planets[i].moveable)
            {
                g.planets[i].position.x += g.planets[i].velocity.x * timeStepSeconds;
                g.planets[i].position.y += g.planets[i].velocity.y * timeStepSeconds;
                g.planets[i].Traces[g.planets[i].TraceIndex] = getDrawPosition(g, i, centerDraw);
                g.planets[i].TraceIndex++;
                if (g.planets[i].TraceIndex >= MAX_TRACE_LENGTH)
                {
                    g.planets[i].TraceIndex = 0;
                }
            }
        }
    }
    // move a planet if the left click is on it
    int x, y;
    const Uint32 buttons = SDL_GetMouseState(&x, &y);
    if(buttons == LEFT_MOUSE_BUTTON)
    {
        Vector2f mousePos = initVector2f(x, y);
        mousePos = getPositionFromScreenPosition(centerDraw, mousePos);
        // if the left click is on a planet, change the position of the planet
        for (int i = 0; i < g.nbPlanets; i++)
        {
            if (isOnPlanet(g.planets[i], mousePos) && i != g.selectedPlanet && !g.planets[i].grabbed)
            {
                g.planets[i].grabbed = true;
            }
            else if(g.planets[i].grabbed && !isOnPlanet(g.planets[i], mousePos))
            {
                g.planets[i].grabbed = false;
            }
            if(g.planets[i].grabbed)
                g.planets[i].position = mousePos;
        }
    }

    // change the selected planet if the right click is on a planet
    else if (buttons == RIGHT_MOUSE_BUTTON)
    {
        Vector2f mousePos = initVector2f(x, y);
        mousePos = getPositionFromScreenPosition(centerDraw, mousePos);
        // if the right click is on a planet, change the color of the planet
        for (int i = 0; i < g.nbPlanets; i++)
        {
            if (isOnPlanet(g.planets[i], mousePos) && i != g.selectedPlanet)
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


void update(float timeStepSeconds, All &all, int &index)
{
    // increase the timestep
    timeStepSeconds *= TIMESTEPS_MULTIPLIER;

    // update the position of the planets if it's the first index
    if(index == 0)
    {
        updateGalaxy(timeStepSeconds, all.g, all.centerDraw, all.paused);
    }

    // check if the button are clicked
    int x, y;
    const Uint32 buttons = SDL_GetMouseState(&x, &y);
    if (buttons == LEFT_MOUSE_BUTTON)
    {
        Vector2f mousePos = initVector2f(x, y);
        //if the left click is on the button next page, change the page
        if (isOnButton(all.pause, mousePos) && !all.pause.pressed)
        {
            all.paused = !all.paused;
            all.pause.pressed = true;
        }
        //if the left click is on the button reset, reset the galaxy
        else if (isOnButton(all.reset, mousePos) && !all.reset.pressed)
        {
            resetGalaxy(all.g);
            all.reset.pressed = true;
        }
    }
    else
    {
        all.pause.pressed = false;
        all.reset.pressed = false;
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
    int index = 0;
    All all;

    // initialize world
    initAll(all);

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
        update(timeStepS, all, index);

        // draw
        draw(all, window, index);

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