#pragma once

// include public libraries
#include <SDL2/SDL.h>

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
    void fillCircle(float x, float y, float r);
    void drawRectangle(int x, int y, int w, int h);
    void drawLine(float x1, float y1, float x2, float y2);
    void drawPoint(float x, float y);
    void drawBackground();
    void drawTexture(SDL_Texture *p_texture, int p_x, int p_y, float heightModifier);
    void drawTextureFlip(SDL_Texture *p_texture, int p_x, int p_y, float heightModifier);
    void drawTextureRect(SDL_Texture *p_texture, int p_x, int p_y, float heightModifier, SDL_Rect p_rect);
    void drawTextureRectFlip(SDL_Texture *p_texture, int p_x, int p_y, float heightModifier, SDL_Rect p_rect);
    void textureFlip(SDL_Texture *p_texture);

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width, height;
};