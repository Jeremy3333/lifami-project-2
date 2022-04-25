// include public libraries
#include <cmath>

// include my libraries
#include "Electromagnetism.hpp"
#include "Utils.hpp"

// init an electromagnetic particle
void initElectromagnetism(Electromagnetism &e, Vector2f position, Vector2f velocity, double radius, SDL_Color color)
{
    e.position = position;
    e.velocity = velocity;
    e.radius = radius;
    e.color = color;
}

// init an array of electromagnetic particles with 1 particules
void initElectromagnetismArray(ElectromagnetismArray &e)
{
    e.nbParticles = 1;
    initElectromagnetism(e.particles[0], initVector2f(0, 0), initVector2f(0, 0), 20, {0, 255, 0, 255});
    e.loaded = true;
}

// count the numbre of point who are inferior to the research answer and return an array of index of the point
int countInferior(float point[4], float answer, int index[4])
{
    int nb = 0;
    for(int i = 0; i < 4; i++)
    {
        if(point[i] <= answer)
        {
            index[nb] = i;
            nb++;
        }
    }
    return nb;
}

void drawElectromagnetismArray(ElectromagnetismArray e, RenderWindow &window)
{
    int x, i, j;
    int numInf, index[4];
    float beta[5];
    float r_sqr;
    Vector2f drawPos[4];
    for(x = 0; x < e.nbParticles; x++)
    {
        window.color(e.particles[x].color.r, e.particles[x].color.g, e.particles[x].color.b, e.particles[x].color.a);
        r_sqr = pow(e.particles[x].radius, 2);
        for(i = -((WINDOW_WIDTH / CASE_SIZE)/2); i < (WINDOW_WIDTH / CASE_SIZE)/2; i++)
        {
            for(j = -((WINDOW_HEIGHT / CASE_SIZE)/2); j < (WINDOW_HEIGHT / CASE_SIZE)/2; j++)
            {
                beta[0] = pow(i - e.particles[x].position.x, 2) + pow(j - e.particles[x].position.y, 2);
                beta[1] = pow(i - e.particles[x].position.x, 2) + pow(j - e.particles[x].position.y - 1, 2);
                beta[2] = pow(i - e.particles[x].position.x + 1, 2) + pow(j - e.particles[x].position.y, 2);
                beta[3] = pow(i - e.particles[x].position.x + 1, 2) + pow(j - e.particles[x].position.y - 1, 2);
                beta[4] = pow(i - e.particles[x].position.x + 0.5, 2) + pow(j - e.particles[x].position.y + 0.5, 2);
                numInf = countInferior(beta, r_sqr, index);
                drawPos[0] = getDrawPosition(initVector2f(i * CASE_SIZE, j * CASE_SIZE), initVector2f(0, 0));
                drawPos[1] = getDrawPosition(initVector2f(i * CASE_SIZE, (j - 1) * CASE_SIZE), initVector2f(0, 0));
                drawPos[2] = getDrawPosition(initVector2f((i + 1) * CASE_SIZE, j * CASE_SIZE), initVector2f(0, 0));
                drawPos[3] = getDrawPosition(initVector2f((i + 1) * CASE_SIZE, (j - 1) * CASE_SIZE), initVector2f(0, 0));
                switch (numInf)
                {
                case 1:
                    switch (index[0])
                    {
                    case 0:
                        window.drawLine(drawPos[0].x + CASE_SIZE / 2, drawPos[0].y, drawPos[0].x, drawPos[0].y - CASE_SIZE / 2);
                        break;
                    case 1:
                        window.drawLine(drawPos[1].x + CASE_SIZE / 2, drawPos[1].y, drawPos[1].x, drawPos[1].y + CASE_SIZE / 2);
                        break;
                    case 2:
                        window.drawLine(drawPos[2].x - CASE_SIZE / 2, drawPos[2].y, drawPos[2].x, drawPos[2].y - CASE_SIZE / 2);
                        break;
                    case 3:
                        window.drawLine(drawPos[3].x - CASE_SIZE / 2, drawPos[3].y, drawPos[3].x, drawPos[3].y + CASE_SIZE / 2);
                        break;
                    default:
                        break;
                    }
                    break;
                case 2:
                    if (index[0] == 0 && index[1] == 1)
                    {
                        window.drawLine(drawPos[0].x + CASE_SIZE / 2, drawPos[0].y, drawPos[1].x + CASE_SIZE / 2, drawPos[1].y);
                    }
                    else if(index[0] == 0 && index[1] == 2)
                    {
                        window.drawLine(drawPos[0].x, drawPos[0].y - CASE_SIZE / 2, drawPos[2].x, drawPos[2].y - CASE_SIZE / 2);
                    }
                    else if(index[0] == 0 && index[1] == 3)
                    {
                        if(beta[4] <= r_sqr)
                        {
                            window.drawLine(drawPos[1].x + CASE_SIZE / 2, drawPos[1].y, drawPos[1].x, drawPos[1].y + CASE_SIZE / 2);
                            window.drawLine(drawPos[2].x - CASE_SIZE / 2, drawPos[2].y, drawPos[2].x, drawPos[2].y - CASE_SIZE / 2);
                        }
                        else
                        {
                            window.drawLine(drawPos[0].x + CASE_SIZE / 2, drawPos[0].y, drawPos[0].x, drawPos[0].y - CASE_SIZE / 2);
                            window.drawLine(drawPos[3].x - CASE_SIZE / 2, drawPos[3].y, drawPos[3].x, drawPos[3].y + CASE_SIZE / 2);
                        }
                    }
                    else if(index[0] == 1 && index[1] == 2)
                    {
                        if(beta[4] <= r_sqr)
                        {
                            window.drawLine(drawPos[0].x + CASE_SIZE / 2, drawPos[0].y, drawPos[0].x, drawPos[0].y - CASE_SIZE / 2);
                            window.drawLine(drawPos[3].x - CASE_SIZE / 2, drawPos[3].y, drawPos[3].x, drawPos[3].y + CASE_SIZE / 2);
                        }
                        else
                        {
                            window.drawLine(drawPos[1].x + CASE_SIZE / 2, drawPos[1].y, drawPos[1].x, drawPos[1].y + CASE_SIZE / 2);
                            window.drawLine(drawPos[2].x - CASE_SIZE / 2, drawPos[2].y, drawPos[2].x, drawPos[2].y - CASE_SIZE / 2);
                        }
                    }
                    else if(index[0] == 1 && index[1] == 3)
                    {
                        window.drawLine(drawPos[1].x, drawPos[1].y + CASE_SIZE / 2, drawPos[3].x, drawPos[3].y + CASE_SIZE / 2);
                    }
                    else if (index[0] == 2 && index[1] == 3)
                    {
                        window.drawLine(drawPos[2].x - CASE_SIZE / 2, drawPos[2].y, drawPos[3].x - CASE_SIZE / 2, drawPos[3].y);
                    }
                    break;
                case 3:
                    if(index[0] == 0 && index[1] == 1 && index[2] == 2)
                    {
                        window.drawLine(drawPos[3].x - CASE_SIZE / 2, drawPos[3].y, drawPos[3].x, drawPos[3].y + CASE_SIZE / 2);
                    }
                    else if(index[0] == 0 && index[1] == 1 && index[2] == 3)
                    {
                        window.drawLine(drawPos[2].x - CASE_SIZE / 2, drawPos[2].y, drawPos[2].x, drawPos[2].y - CASE_SIZE / 2);
                    }
                    else if(index[0] == 0 && index[1] == 2 && index[2] == 3)
                    {
                        window.drawLine(drawPos[1].x + CASE_SIZE / 2, drawPos[1].y, drawPos[1].x, drawPos[1].y + CASE_SIZE / 2);
                    }
                    else if(index[0] == 1 && index[1] == 2 && index[2] == 3)
                    {
                        window.drawLine(drawPos[0].x + CASE_SIZE / 2, drawPos[0].y, drawPos[0].x, drawPos[0].y - CASE_SIZE / 2);
                    }
                default:
                    break;
                }
            }
        }
    }
}