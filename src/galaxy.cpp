// inclue les librairies publiques
#include <SDL2/SDL.h> // SDL_Rect, SDL_Color, SDL_Point

// inclue les librairies de mon programme
#include "Galaxy.hpp"       // Galaxy, Planet, initPlanet, initGalaxy, initTrace, resetGalaxy, drawGalaxy, calculateForces, updateGalaxy
#include "Math.hpp"         // Vector2f,  initVector2f
#include "Utils.hpp"        // getDrawPosition, getPositionFromScreenPosition, isOnCircle, isOnRect
#include "RenderWindow.hpp" // for the window
#include "Physics.hpp"      // calculateForces

// initialise la planète
Planet initPlanet(Vector2f position, Vector2f velocity, double mass, double radius, SDL_Color color, bool moveable, SDL_Color TraceColor)
{
    Planet p;                                       // créer une planète
    p.position = position;                          // position de la planète
    p.velocity = velocity;                          // vélocité de la planète
    p.acceleration = initVector2f(0, 0);            // accélération de la planète
    p.mass = mass;                                  // masse de la planète
    p.radius = radius;                              // rayon de la planète
    p.color = color;                                // couleur de la planète
    p.moveable = moveable;                          // si la planète est déplaçable ou non
    p.TraceIndex = 0;                               // index de la trace
    Vector2f tracePos = initVector2f(-1000, -1000); // position de la trace en dehors de l'écran
    // créer les traces
    for (int i = 0; i < MAX_TRACE_LENGTH; i++)
    {
        p.Traces[i] = tracePos; // position de la trace en dehors de l'écran
    }
    p.TraceColor = TraceColor; // couleur de la trace
    p.grabbed = false;         // si la planète est en train d'etre déplacée ou non
    return p;                  // retourner la planète
}

// initialise la galaxie
void initGalaxy(Galaxy &g)
{
    g.nbPlanets = 4;                                                                                                                                                         // nombre de planètes
    g.planets[0] = initPlanet(initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), initVector2f(0, 0), 20000000000000, 10, {255, 255, 255, 255}, false, {0, 255, 0, 255});     // créer la planète 1
    g.planets[1] = initPlanet(initVector2f(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2), initVector2f(0, 20), 20000000000, 5, {255, 255, 100, 255}, true, {0, 255, 0, 255});   // créer la planète 2
    g.planets[2] = initPlanet(initVector2f(WINDOW_WIDTH / 2 + 150, WINDOW_HEIGHT / 2), initVector2f(0, -20), 200000000000, 7, {255, 255, 100, 255}, true, {0, 255, 0, 255}); // créer la planète 3
    g.planets[3] = initPlanet(initVector2f(WINDOW_WIDTH / 2 + 160, WINDOW_HEIGHT / 2), initVector2f(0, -11), 200000, 3, {255, 255, 100, 255}, true, {0, 255, 0, 255});       // créer la planète 4

    g.selectedPlanet = -1;                                            // planète sélectionnée
    g.loaded = true;                                                  // si la galaxie est chargée ou non
    g.centerDraw = initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2); // centre de dessin
}

// initialise la trace en dehors de l'écran
void initTrace(Galaxy &g)
{
    for (int i = 0; i < g.nbPlanets; i++)
    {
        for (int j = 0; j < MAX_TRACE_LENGTH; j++)
        {
            g.planets[i].Traces[j] = initVector2f(-100, -100); // position de la trace en dehors de l'écran
        }
    }
}

// réinitialise la galaxie (différament que le init)
void resetGalaxy(Galaxy &g)
{
    g.nbPlanets = 4;                                                                                                                                                         // nombre de planètes
    g.planets[0] = initPlanet(initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), initVector2f(0, 0), 20000000000000, 10, {255, 255, 255, 255}, false, {0, 255, 0, 255});     // créer la planète 1
    g.planets[1] = initPlanet(initVector2f(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2), initVector2f(0, 20), 20000000000, 5, {255, 255, 100, 255}, true, {0, 255, 0, 255});   // créer la planète 2
    g.planets[2] = initPlanet(initVector2f(WINDOW_WIDTH / 2 + 150, WINDOW_HEIGHT / 2), initVector2f(0, -20), 200000000000, 7, {255, 255, 100, 255}, true, {0, 255, 0, 255}); // créer la planète 3
    g.planets[3] = initPlanet(initVector2f(WINDOW_WIDTH / 2 + 160, WINDOW_HEIGHT / 2), initVector2f(0, -11), 200000, 3, {255, 255, 100, 255}, true, {0, 255, 0, 255});       // créer la planète 4
}

// dessine la galaxie
void drawGalaxy(Galaxy g, RenderWindow &window)
{
    for (int i = 0; i < g.nbPlanets; i++)
    {
        window.color(g.planets[i].TraceColor.r, g.planets[i].TraceColor.g, g.planets[i].TraceColor.b, g.planets[i].TraceColor.a); // couleur de la trace
        if (g.planets[i].moveable)
        {
            for (int j = 0; j < MAX_TRACE_LENGTH; j++)
            {
                window.drawPoint(g.planets[i].Traces[j].x, g.planets[i].Traces[j].y); // dessiner la trace
            }
        }
        window.color(g.planets[i].color.r, g.planets[i].color.g, g.planets[i].color.b, g.planets[i].color.a); // couleur de la planète
        Vector2f drawPosition = getDrawPosition(g.planets[i].position, g.centerDraw);                         // position de la planète dans le dessin
        window.fillCircle(drawPosition.x, drawPosition.y, g.planets[i].radius);                               // dessiner la planète
        if (g.planets[i].moveable)
        {
            window.color(0, 0, 255, 255);                                                                                                                                // couleur de la vélocité
            window.drawLine(drawPosition.x, drawPosition.y, drawPosition.x + (g.planets[i].velocity.x / 2), drawPosition.y + (g.planets[i].velocity.y / 2));             // dessiner la vélocité
            window.color(255, 0, 0, 255);                                                                                                                                // couleur de la accélération
            window.drawLine(drawPosition.x, drawPosition.y, drawPosition.x + (g.planets[i].acceleration.x * 100), drawPosition.y + (g.planets[i].acceleration.y * 100)); // dessiner l'accélération
        }
    }
}

// Calcule la force de gravitation entre toute les planètes de la galaxie
void calculateForces(Galaxy &g)
{
    for (int i = 0; i < g.nbPlanets; i++)
    {
        g.planets[i].acceleration = initVector2f(0, 0); // accélération à 0
        for (int j = 0; j < g.nbPlanets; j++)
        {
            if (i != j)
            {
                g.planets[i].velocity = g.planets[i].velocity + gForce(g.planets[i], g.planets[j]) / g.planets[i].mass; // ajoute la force de gravitation à la vélocité
                g.planets[i].acceleration += gForce(g.planets[i], g.planets[j]) / g.planets[i].mass;                    // ajoute la force de gravitation à l'accélération
            }
        }
    }
}

// Met a jour la position de la planète (seulement si elle est déplaçable)
void updateGalaxy(float timeStepSeconds, Galaxy &g, bool pause)
{
    if (g.selectedPlanet == -1)
        g.centerDraw = initVector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2); // centre de dessin
    else
        g.centerDraw = g.planets[g.selectedPlanet].position; // centre de dessin sur la planète sélectionnée

    if (!pause)
    {
        calculateForces(g); // calcule la force de gravitation entre toute les planètes

        // met à jour la position des planètes
        for (int i = 0; i < g.nbPlanets; i++)
        {
            if (g.planets[i].moveable)
            {
                g.planets[i].position.x += g.planets[i].velocity.x * timeStepSeconds;                                // met à jour la position en x
                g.planets[i].position.y += g.planets[i].velocity.y * timeStepSeconds;                                // met à jour la position en y
                g.planets[i].Traces[g.planets[i].TraceIndex] = getDrawPosition(g.planets[i].position, g.centerDraw); // met à jour la trace
                g.planets[i].TraceIndex++;                                                                           // incrémente l'index de la trace
                if (g.planets[i].TraceIndex >= MAX_TRACE_LENGTH)
                {
                    g.planets[i].TraceIndex = 0; // remet l'index à 0
                }
            }
        }
    }
    // bouge la planète si on clique avec la souris sur la planète clique gauche
    int x, y;                                         // coordonnées de la souris
    const Uint32 buttons = SDL_GetMouseState(&x, &y); // récupère les boutons de la souris
    if (buttons == LEFT_MOUSE_BUTTON)
    {
        Vector2f mousePos = initVector2f(x, y);                           // position de la souris
        mousePos = getPositionFromScreenPosition(g.centerDraw, mousePos); // position de la souris dans la galaxie
        // if the left click is on a planet, change the position of the planet
        for (int i = 0; i < g.nbPlanets; i++)
        {
            if (isOnCircle(g.planets[i].position, g.planets[i].radius, mousePos) && i != g.selectedPlanet && !g.planets[i].grabbed)
            {
                g.planets[i].grabbed = true; // la planète est maintenant sélectionnée
            }
            else if (g.planets[i].grabbed && !isOnCircle(g.planets[i].position, g.planets[i].radius, mousePos))
            {
                g.planets[i].grabbed = false; // la planète n'est plus sélectionnée
            }
            if (g.planets[i].grabbed)
                g.planets[i].position = mousePos; // met à jour la position de la planète
        }
    }

    // change la planète sélectionnée si on clique avec la souris sur la planète clique droite
    else if (buttons == RIGHT_MOUSE_BUTTON)
    {
        Vector2f mousePos = initVector2f(x, y);                           // position de la souris
        mousePos = getPositionFromScreenPosition(g.centerDraw, mousePos); // position de la souris dans la galaxie
        // si la souris est sur une planète, la planète est sélectionnée
        for (int i = 0; i < g.nbPlanets; i++)
        {
            if (isOnCircle(g.planets[i].position, g.planets[i].radius, mousePos) && i != g.selectedPlanet)
            {
                g.selectedPlanet = i; // la planète est sélectionnée
                initTrace(g);         // réinitialise la trace
            }
            else
            {
                if (g.planets[i].moveable)
                    g.planets[i].color = {255, 255, 100, 255}; // remet la couleur de la planète à la couleur par défaut
                else
                    g.planets[i].color = {255, 255, 255, 255}; // remet la couleur de la planète à la couleur par défaut
            }
        }
    }

    g.planets[g.selectedPlanet].color = {0, 255, 0, 255}; // met la couleur de la planète sélectionnée en vert
}