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

#define PLAYER_DIRECTION_UP 0
#define PLAYER_DIRECTION_DOWN 1
#define PLAYER_DIRECTION_LEFT 2
#define PLAYER_DIRECTION_RIGHT 3
#define PLAYER_DIRECTION_UP_LEFT 4
#define PLAYER_DIRECTION_UP_RIGHT 5
#define PLAYER_DIRECTION_DOWN_LEFT 6
#define PLAYER_DIRECTION_DOWN_RIGHT 7

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
    int direction;
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
    w.player.direction = PLAYER_DIRECTION_DOWN;
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
}

// choose wich part of the texture to draw
void selectRect(world &w)
{
    switch (w.player.direction)
    {
        {
        case PLAYER_DIRECTION_UP:
            w.player.playerRect.x = w.player.frameWidth;
            w.player.playerRect.y = w.player.frameHeight * 0;
            break;
        case PLAYER_DIRECTION_DOWN:
            w.player.playerRect.x = w.player.frameWidth;
            w.player.playerRect.y = w.player.frameHeight * 4;
            break;
        case PLAYER_DIRECTION_LEFT:
            w.player.playerRect.x = w.player.frameWidth ;
            w.player.playerRect.y = w.player.frameHeight * 2;
            break;
        case PLAYER_DIRECTION_RIGHT:
            w.player.playerRect.x = w.player.frameWidth;
            w.player.playerRect.y = w.player.frameHeight * 2;
            break;
        case PLAYER_DIRECTION_UP_LEFT:
            w.player.playerRect.x = w.player.frameWidth;
            w.player.playerRect.y = w.player.frameHeight * 1;
            break;
        case PLAYER_DIRECTION_UP_RIGHT:
            w.player.playerRect.x = w.player.frameWidth;
            w.player.playerRect.y = w.player.frameHeight * 1;
            break;
        case PLAYER_DIRECTION_DOWN_LEFT:
            w.player.playerRect.x = w.player.frameWidth;
            w.player.playerRect.y = w.player.frameHeight * 3;
            break;
        case PLAYER_DIRECTION_DOWN_RIGHT:
            w.player.playerRect.x = w.player.frameWidth;
            w.player.playerRect.y = w.player.frameHeight * 3;
            break;
        default:
            break;
        }
    }
}

void draw(RenderWindow &window, world world)
{
    window.color(255, 200, 200, 255);
    window.drawBackground();
    selectRect(world);
    if(world.player.direction == PLAYER_DIRECTION_UP || world.player.direction == PLAYER_DIRECTION_DOWN || world.player.direction == PLAYER_DIRECTION_RIGHT || world.player.direction == PLAYER_DIRECTION_DOWN_RIGHT || world.player.direction == PLAYER_DIRECTION_UP_RIGHT)
    {
        window.drawTextureRect(world.player.moveTexture, world.player.x, world.player.y, HEIGHT_MODIFIER, world.player.playerRect);
    }
    else
    {
        window.drawTextureRectFlip(world.player.moveTexture, world.player.x, world.player.y, HEIGHT_MODIFIER, world.player.playerRect);
    }
}

void update(float timeStepSeconds, world &world, const Uint8 *state)
{
    // update player position and direction
    if (state[SDL_SCANCODE_W])
    {
        world.player.y -= timeStepSeconds * 500;
        world.player.direction = PLAYER_DIRECTION_UP;
    }
    if (state[SDL_SCANCODE_S])
    {
        world.player.y += timeStepSeconds * 500;
        world.player.direction = PLAYER_DIRECTION_DOWN;
    }
    if (state[SDL_SCANCODE_A])
    {
        world.player.x -= timeStepSeconds * 500;
        world.player.direction = PLAYER_DIRECTION_LEFT;
    }
    if (state[SDL_SCANCODE_D])
    {
        world.player.x += timeStepSeconds * 500;
        world.player.direction = PLAYER_DIRECTION_RIGHT;
    }
    if (state[SDL_SCANCODE_W] && state[SDL_SCANCODE_A])
    {
        world.player.direction = PLAYER_DIRECTION_UP_LEFT;
    }
    if (state[SDL_SCANCODE_W] && state[SDL_SCANCODE_D])
    {
        world.player.direction = PLAYER_DIRECTION_UP_RIGHT;
    }
    if (state[SDL_SCANCODE_S] && state[SDL_SCANCODE_A])
    {
        world.player.direction = PLAYER_DIRECTION_DOWN_LEFT;
    }
    if (state[SDL_SCANCODE_S] && state[SDL_SCANCODE_D])
    {
        world.player.direction = PLAYER_DIRECTION_DOWN_RIGHT;
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