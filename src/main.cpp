// include public libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cmath>

// include my libraries
#include "RenderWindow.hpp"
#include "Math.hpp"
#include "Galaxy.hpp"
#include "Utils.hpp"


// define global variables
#define TIMESTEPS_MULTIPLIER 1
#define WINDOW_TITLE "LIFAMI-Project"
#define NUM_PAGES 2

/*
 *toute la description est dans le README.md (si vous n'avez pas de lecteur de markdown il est aussi disponible sur le github qui ena un par defaut)
 *
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
struct Button {
    Vector2f position;
    Vector2f size;
    SDL_Color color;
    bool pressed;
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

// init a button
void initButton(Button &b, Vector2f position, Vector2f size, SDL_Color color)
{
    b.position = position;
    b.size = size;
    b.color = color;
    b.pressed = false;
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
        if (isOnRect(all.pause.position, all.pause.size, mousePos) && !all.pause.pressed)
        {
            all.paused = !all.paused;
            all.pause.pressed = true;
        }
        //if the left click is on the button reset, reset the galaxy
        else if (isOnRect(all.reset.position, all.reset.size , mousePos) && !all.reset.pressed)
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