// include public libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

// include my libraries
#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char *p_title, int p_w, int p_h)
    : window(NULL), renderer(NULL), width(p_w), height(p_h)
{
    // initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    std::cout << "SDL Initialized" << std::endl;

    // create window
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    std::cout << "Window created" << std::endl;

    // create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    std::cout << "Renderer created" << std::endl;
}

SDL_Texture *RenderWindow::loadTexture(const char *p_filePath)
{
    // load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(p_filePath);
    if (loadedSurface == NULL)
    {
        std::cout << "IMG_Load Error: " << IMG_GetError() << std::endl;
        exit(1);
    }
    std::cout << "Image loaded" << std::endl;

    // create texture from surface pixels
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (texture == NULL)
    {
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    std::cout << "Texture created" << std::endl;

    // get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
    std::cout << "Surface freed" << std::endl;

    return texture;
}

int RenderWindow::getRefreshRate()
{
    int displayIndex = SDL_GetWindowDisplayIndex(window);
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(displayIndex, 0, &mode);
    return mode.refresh_rate;
}

void RenderWindow::cleanUp()
{
    // destroy renderer
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    std::cout << "Renderer destroyed" << std::endl;

    // destroy window
    SDL_DestroyWindow(window);
    window = NULL;
    std::cout << "Window destroyed" << std::endl;

    // quit SDL subsystems
    SDL_Quit();
    std::cout << "SDL quit" << std::endl;
}

void RenderWindow::clear()
{
    // clear screen
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
}

void RenderWindow::display()
{
    // update screen
    SDL_RenderPresent(renderer);
}

void RenderWindow::color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void RenderWindow::fillCircle(float x, float y, float radius)
{
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void RenderWindow::drawRectangle(int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
}

void RenderWindow::drawLine(float x1, float y1, float x2, float y2)
{
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void RenderWindow::drawPoint(float x, float y)
{
    SDL_RenderDrawPoint(renderer, x, y);
}

void RenderWindow::drawBackground()
{
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = width;
    rect.h = height;
    SDL_RenderFillRect(renderer, &rect);
}

void RenderWindow::drawTexture(SDL_Texture *p_texture, int p_x, int p_y, float heightModifier)
{
    SDL_Rect rect;
    rect.x = p_x;
    rect.y = p_y;
    SDL_QueryTexture(p_texture, NULL, NULL, &rect.w, &rect.h);
    rect.w *= heightModifier;
    rect.h *= heightModifier;
    rect.x -= rect.w / 2;
    rect.y -= rect.h / 2;
    SDL_RenderCopy(renderer, p_texture, NULL, &rect);
}

void RenderWindow::drawTextureFlip(SDL_Texture *p_texture, int p_x, int p_y, float heightModifier)
{
    SDL_Rect rect;
    rect.x = p_x;
    rect.y = p_y;
    SDL_QueryTexture(p_texture, NULL, NULL, &rect.w, &rect.h);
    rect.w *= heightModifier;
    rect.h *= heightModifier;
    rect.x -= rect.w / 2;
    rect.y -= rect.h / 2;
    SDL_RenderCopyEx(renderer, p_texture, NULL, &rect, 0, NULL, SDL_FLIP_HORIZONTAL);
}

void RenderWindow::drawTextureRect(SDL_Texture *p_texture, int p_x, int p_y, float heightModifier, SDL_Rect p_rect)
{
    SDL_Rect rect;
    rect.x = p_x;
    rect.y = p_y;
    rect.w = p_rect.w;
    rect.h = p_rect.h;
    rect.w *= heightModifier;
    rect.h *= heightModifier;
    rect.x -= rect.w / 2;
    rect.y -= rect.h / 2;
    SDL_RenderCopy(renderer, p_texture, &p_rect, &rect);
}

// like drawTextureRect but flip texture horizontally
void RenderWindow::drawTextureRectFlip(SDL_Texture *p_texture, int p_x, int p_y, float heightModifier, SDL_Rect p_rect)
{
    SDL_Rect rect;
    rect.x = p_x;
    rect.y = p_y;
    rect.w = p_rect.w;
    rect.h = p_rect.h;
    rect.w *= heightModifier;
    rect.h *= heightModifier;
    rect.x -= rect.w / 2;
    rect.y -= rect.h / 2;
    SDL_RenderCopyEx(renderer, p_texture, &p_rect, &rect, 0, NULL, SDL_FLIP_HORIZONTAL);
}