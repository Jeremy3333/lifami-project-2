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
#define MAX_RECT_WALL 20
#define MAX_ANGLE_WALL 20
#define HEIGHT_MODIFIER 2

/*
 * Debug:
 * g++ -c src/*.cpp -std=c++14 -g -Wall -m64 -I include -I C:/SDL2-w64/include  && g++ *.o -o bin/debug/main -L C:/SDL2-w64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image && start bin/debug/main
 *
 *Release
 * g++ -c src/*.cpp -std=c++14 -O3 -Wall -m64 -I include -I C:/SDL2-w64/include  && g++ *.o -o bin/release/main -s -L C:/SDL2-w64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image && start bin/release/main
 */

struct player
{
    float x, y;
    float animation_timer;
    SDL_Texture *moveTexture;
    int frameWidth, frameHeight;
    SDL_Rect playerRect;
    bool left;
};

struct rect_wall
{
    float x, y;
    float width, height;
};

struct angle_wall
{
    float x, y;
    float width;
    int direction;
};

struct world
{
    player player;
    rect_wall rect_walls[MAX_RECT_WALL];
    angle_wall angle_walls[MAX_ANGLE_WALL];
};

void init(world &w, RenderWindow window)
{
    w.player.x = WINDOW_WIDTH / 2;
    w.player.y = WINDOW_HEIGHT / 2;
    w.player.animation_timer = 0;
    {
        int textureWidth = 0;
        int textureHeight = 0;
        w.player.moveTexture = window.loadTexture("assets/media/move.png");
        SDL_QueryTexture(w.player.moveTexture, NULL, NULL, &textureWidth, &textureHeight);
        w.player.frameWidth = textureWidth / 16;
        w.player.frameHeight = textureHeight / 19;
        w.player.playerRect.x = w.player.frameWidth;
        w.player.playerRect.y = w.player.frameHeight * 4;
        w.player.playerRect.w = w.player.frameWidth;
        w.player.playerRect.h = w.player.frameHeight;
    }
    //init player texture
}

void draw(RenderWindow &window, world world)
{
    window.color(255, 200, 200, 255);
    window.drawBackground();
    if(world.player.left)
    {
        window.drawTextureRectFlip(world.player.moveTexture, world.player.x, world.player.y, HEIGHT_MODIFIER, world.player.playerRect);
    }
    else
    {
        window.drawTextureRect(world.player.moveTexture, world.player.x, world.player.y, HEIGHT_MODIFIER, world.player.playerRect);
    }
}

void update(float timeStepSeconds, world &world, const Uint8 *state)
{
    // update player position and direction
    if (state[SDL_SCANCODE_W])
    {
        world.player.y -= timeStepSeconds * 500;
        world.player.playerRect.x = world.player.frameWidth * 1;
        world.player.playerRect.y = world.player.frameHeight * 0;
        world.player.left = false;
    }
    if (state[SDL_SCANCODE_S])
    {
        world.player.y += timeStepSeconds * 500;
        world.player.playerRect.x = world.player.frameWidth * 1;
        world.player.playerRect.y = world.player.frameHeight * 4;
        world.player.left = false;
    }
    if (state[SDL_SCANCODE_A])
    {
        world.player.x -= timeStepSeconds * 500;
        world.player.playerRect.x = world.player.frameWidth * 1;
        world.player.playerRect.y = world.player.frameHeight * 2;
        world.player.left = true;
    }
    if (state[SDL_SCANCODE_D])
    {
        world.player.x += timeStepSeconds * 500;
        world.player.playerRect.x = world.player.frameWidth * 1;
        world.player.playerRect.y = world.player.frameHeight * 2;
        world.player.left = false;
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
    world w;

    // initialize world
    init(w, window);

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
        update(timeStepS, w, state);
        // draw
        draw(window, w);
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