// include public libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cmath>

// include private libraries (in include folder)
#include "RenderWindow.hpp"
#include "math.hpp"

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

Vector2f initVector(float x, float y)
{
    Vector2f vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

Vector2f operator-(const Vector2f &lhs, const Vector2f &rhs)
{
    return initVector(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vector2f operator+(const Vector2f &lhs, const Vector2f &rhs)
{
    return initVector(lhs.x + rhs.x, lhs.y + rhs.y);
}

Vector2f operator/(const Vector2f &lhs, float rhs)
{
    return initVector(lhs.x / rhs, lhs.y / rhs);
}

Vector2f operator*(const Vector2f &lhs, float rhs)
{
    return initVector(lhs.x * rhs, lhs.y * rhs);
}
Vector2f operator*(float rhs, const Vector2f &lhs)
{
    return initVector(lhs.x * rhs, lhs.y * rhs);
}

void init()
{
}

void draw(RenderWindow &window)
{
}

void update(float timeStep)
{
}

int main(int argc, char **argv)
{
    // initialize SDL
    RenderWindow window(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

    // initialize variables
    SDL_Event event;
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    bool quit = false;

    // initialize world
    init();

    // load media

    // main loop
    while (!quit)
    {
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
        // update world
        update(timeStepS);
        // draw
        draw(window);
        // render

        window.display();

        float frameTicks = SDL_GetTicks() - startTicks;

        if (frameTicks < timeStepMs)
            SDL_Delay(timeStepMs - frameTicks);
    }

    // clean up memory
    window.cleanUp();
    return 0;
}