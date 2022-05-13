// include public library
#include <SDL2/SDL.h>

// include my libraries
#include "Galaxy.hpp"
#include "Math.hpp"
#include "Utils.hpp"
#include "RenderWindow.hpp"
#include "Physics.hpp"

//init a planet
Planet initPlanet(Vector2f position, Vector2f velocity, double mass, double radius, SDL_Color color, bool moveable, SDL_Color TraceColor)
{
    Planet p;
    p.position = position;
    p.velocity = velocity;
    p.acceleration = initVector2f(0, 0);
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

//init galaxy with 4 planets and no selected planet
void initGalaxy(Galaxy &g) {
    g.nbPlanets = 4;
    g.planets[0] = initPlanet(initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), initVector2f(0, 0), 20000000000000, 10, {255, 255, 255, 255}, false, {0, 255, 0, 255});
    g.planets[1] = initPlanet(initVector2f(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2), initVector2f(0, 20), 20000000000, 5, {255, 255, 100, 255}, true, {0, 255, 0, 255});
    g.planets[2] = initPlanet(initVector2f(WINDOW_WIDTH / 2 + 150, WINDOW_HEIGHT / 2), initVector2f(0, -20), 200000000000, 7, {255, 255, 100, 255}, true, {0, 255, 0, 255});
    g.planets[3] = initPlanet(initVector2f(WINDOW_WIDTH / 2 + 160, WINDOW_HEIGHT / 2), initVector2f(0, -11), 200000, 3, {255, 255, 100, 255}, true, {0, 255, 0, 255});

    g.selectedPlanet = -1;
    g.loaded = true;
    g.centerDraw = initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
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

//init galaxy with 4 planets but save the selected planet
void resetGalaxy(Galaxy &g)
{
    g.nbPlanets = 4;
    g.planets[0] = initPlanet(initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), initVector2f(0, 0), 20000000000000, 10, {255, 255, 100, 255}, true, {0, 255, 0, 255});
    g.planets[1] = initPlanet(initVector2f(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2), initVector2f(0, 20), 20000000000, 5, {255, 255, 100, 255}, true, {0, 255, 0, 255});
    g.planets[2] = initPlanet(initVector2f(WINDOW_WIDTH / 2 + 150, WINDOW_HEIGHT / 2), initVector2f(0, -20), 200000000000, 7, {255, 255, 100, 255}, true, {0, 255, 0, 255});
    g.planets[3] = initPlanet(initVector2f(WINDOW_WIDTH / 2 + 160, WINDOW_HEIGHT / 2), initVector2f(0, -11), 200000, 3, {255, 255, 100, 255}, true, {0, 255, 0, 255});
}

//draw the galaxy if the index is 0
void drawGalaxy(Galaxy g, RenderWindow &window)
{
    //draw all the planets of a galaxy with window.fillCircle
    for (int i = 0; i < g.nbPlanets; i++) {
        // draw the trace
        window.color(g.planets[i].TraceColor.r, g.planets[i].TraceColor.g, g.planets[i].TraceColor.b, g.planets[i].TraceColor.a);
        if(g.planets[i].moveable)
        {
            for(int j = 0; j < MAX_TRACE_LENGTH; j++)
            {
                window.drawPoint(g.planets[i].Traces[j].x, g.planets[i].Traces[j].y);
            }
        }
        // draw the planet
        window.color(g.planets[i].color.r, g.planets[i].color.g, g.planets[i].color.b, g.planets[i].color.a);
        Vector2f drawPosition = getDrawPosition(g.planets[i].position , g.centerDraw);
        window.fillCircle(drawPosition.x, drawPosition.y, g.planets[i].radius);
        if(g.planets[i].moveable)
        {
            window.color(0, 0, 255, 255);
            window.drawLine(drawPosition.x, drawPosition.y, drawPosition.x + (g.planets[i].velocity.x / 2), drawPosition.y + (g.planets[i].velocity.y / 2));
            window.color(255, 0, 0, 255);
            window.drawLine(drawPosition.x, drawPosition.y, drawPosition.x + (g.planets[i].acceleration.x * 100), drawPosition.y + (g.planets[i].acceleration.y * 100));
        }
    }
}

// calculate the force between all planets with gForce function
void calculateForces(Galaxy &g) {
    for (int i = 0; i < g.nbPlanets; i++) {
        g.planets[i].acceleration = initVector2f(0, 0);
        for (int j = 0; j < g.nbPlanets; j++) {
            if (i != j) {
                g.planets[i].velocity = g.planets[i].velocity + gForce(g.planets[i], g.planets[j]) / g.planets[i].mass;
                g.planets[i].acceleration += gForce(g.planets[i], g.planets[j]) / g.planets[i].mass;
            }
        }
    }
}

// update the position of the planets (only if moveable)
void updateGalaxy(float timeStepSeconds, Galaxy &g, bool pause)
{
    if(g.selectedPlanet == -1)
        g.centerDraw = initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    else
        g.centerDraw = g.planets[g.selectedPlanet].position;

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
                g.planets[i].Traces[g.planets[i].TraceIndex] = getDrawPosition(g.planets[i].position ,g.centerDraw);
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
        mousePos = getPositionFromScreenPosition(g.centerDraw, mousePos);
        // if the left click is on a planet, change the position of the planet
        for (int i = 0; i < g.nbPlanets; i++)
        {
            if (isOnCircle(g.planets[i].position, g.planets[i].radius, mousePos) && i != g.selectedPlanet && !g.planets[i].grabbed)
            {
                g.planets[i].grabbed = true;
            }
            else if(g.planets[i].grabbed && !isOnCircle(g.planets[i].position, g.planets[i].radius, mousePos))
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
        mousePos = getPositionFromScreenPosition(g.centerDraw, mousePos);
        // if the right click is on a planet, change the color of the planet
        for (int i = 0; i < g.nbPlanets; i++)
        {
            if (isOnCircle(g.planets[i].position, g.planets[i].radius, mousePos) && i != g.selectedPlanet)
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