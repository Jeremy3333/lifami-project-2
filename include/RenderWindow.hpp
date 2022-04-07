#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class RenderWindow
{
public:
    RenderWindow(const char *p_title, int p_w, int p_h);
    SDL_Texture *loadTexture(const char *p_filePath);
    int getRefreshRate();
    void cleanUp();
    void clear();
    void display();
    void color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void FillCircle(float x, float y, float r);
    void drawLine(float x1, float y1, float x2, float y2);
    void drawPoint(float x, float y);
    void drawBackground();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width, height;
};