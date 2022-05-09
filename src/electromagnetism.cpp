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
    e.nbParticles = 2;
    initElectromagnetism(e.particles[0], initVector2f(0, 0), initVector2f(0, 0), 2, {0, 255, 0, 255});
    initElectromagnetism(e.particles[1], initVector2f(-10, 2), initVector2f(1, 0), 1, {0, 255, 0, 255});
    e.loaded = true;
}

// count the numbre of point who are inferior to the research answer and return an array of index of the point
int countInferior(float point[4], float answer, int index[4])
{
    int nb = 0;
    for(int i = 0; i < 4; i++)
    {
        if(point[i] >= answer)
        {
            index[nb] = i;
            nb++;
        }
    }
    return nb;
}

// return (c * d) / a
float ruleOfThree(float a, float b, float c)
{
    return (c * b) / a;
}

// calculate the actualle equation at the point
void calculateEquation(float beta[5], float & r_sqr, ElectromagnetismArray e, int i, int j)
{
    int nb = 0;
    r_sqr = 1;
    float i3, j3;
    for(float i2 = i; i2 <= i + 1; i2++)
    {
        for(float j2 = j; j2 >= j - 1; j2--)
        {
            i3 = i2/5;
            j3 = -j2/5;
            beta[nb] = 0;
            for(int x = 0; x < e.nbParticles; x++)
            {
                beta[nb] += e.particles[x].radius / sqrt(pow(i3 -e.particles[x].position.x, 2) +pow(j3 -e.particles[x].position.y, 2));
            }
            nb++;
        }
    }
    beta[4] = -1;
}

void drawCalculedLine(RenderWindow &window, ElectromagnetismArray e, int i, int j)
{
    int numInf;
    int index[4];
    float beta[5];
    float r_sqr;
    Vector2f drawPos[4];
    calculateEquation(beta, r_sqr, e, i, j);
    drawPos[0] = getDrawPosition(initVector2f(i * CASE_SIZE, j * CASE_SIZE), initVector2f(0, 0));
    drawPos[1] = getDrawPosition(initVector2f(i * CASE_SIZE, (j - 1) * CASE_SIZE), initVector2f(0, 0));
    drawPos[2] = getDrawPosition(initVector2f((i + 1) * CASE_SIZE, j * CASE_SIZE), initVector2f(0, 0));
    drawPos[3] = getDrawPosition(initVector2f((i + 1) * CASE_SIZE, (j - 1) * CASE_SIZE), initVector2f(0, 0));
    numInf = countInferior(beta, r_sqr, index);
    switch (numInf)
                {
                case 1:
                    switch (index[0])
                    {
                    case 0:
                        window.drawLine(drawPos[0].x + ruleOfThree(beta[2] - beta[0], CASE_SIZE, r_sqr - beta[0]), drawPos[0].y, drawPos[0].x, drawPos[0].y - ruleOfThree(beta[1] - beta[0], CASE_SIZE, r_sqr - beta[0]));
                        break;
                    case 1:
                        window.drawLine(drawPos[1].x + ruleOfThree(beta[3] - beta[1], CASE_SIZE, r_sqr - beta[1]), drawPos[1].y, drawPos[1].x, drawPos[1].y + ruleOfThree(beta[0] - beta[1], CASE_SIZE, r_sqr - beta[1]));
                        break;
                    case 2:
                        window.drawLine(drawPos[2].x - ruleOfThree(beta[0] - beta[2], CASE_SIZE, r_sqr - beta[2]), drawPos[2].y, drawPos[2].x, drawPos[2].y - ruleOfThree(beta[3] - beta[2], CASE_SIZE, r_sqr - beta[2]));
                        break;
                    case 3:
                        window.drawLine(drawPos[3].x - ruleOfThree(beta[1] - beta[3], CASE_SIZE, r_sqr - beta[3]), drawPos[3].y, drawPos[3].x, drawPos[3].y + ruleOfThree(beta[2] - beta[3], CASE_SIZE, r_sqr - beta[3]));
                        break;
                    default:
                        break;
                    }
                    break;
                case 2:
                    if (index[0] == 0 && index[1] == 1)
                    {
                        window.drawLine(drawPos[0].x + ruleOfThree(beta[2] - beta[0], CASE_SIZE, r_sqr - beta[0]), drawPos[0].y, drawPos[1].x + ruleOfThree(beta[3] - beta[1], CASE_SIZE, r_sqr - beta[1]), drawPos[1].y);
                    }
                    else if(index[0] == 0 && index[1] == 2)
                    {
                        window.drawLine(drawPos[0].x, drawPos[0].y - ruleOfThree(beta[1] - beta[0], CASE_SIZE, r_sqr - beta[0]), drawPos[2].x, drawPos[2].y - ruleOfThree(beta[3] - beta[2], CASE_SIZE, r_sqr - beta[2]));
                    }
                    else if(index[0] == 0 && index[1] == 3)
                    {
                        if(beta[4] > r_sqr)
                        {
                            window.drawLine(drawPos[1].x + ruleOfThree(beta[3] - beta[1], CASE_SIZE, r_sqr - beta[1]), drawPos[1].y, drawPos[1].x, drawPos[1].y + ruleOfThree(beta[0] - beta[1], CASE_SIZE, r_sqr - beta[1]));
                            window.drawLine(drawPos[2].x - ruleOfThree(beta[0] - beta[2], CASE_SIZE, r_sqr - beta[2]), drawPos[2].y, drawPos[2].x, drawPos[2].y - ruleOfThree(beta[3] - beta[2], CASE_SIZE, r_sqr - beta[2]));
                        }
                        else
                        {
                            window.drawLine(drawPos[0].x + ruleOfThree(beta[2] - beta[0], CASE_SIZE, r_sqr - beta[0]), drawPos[0].y, drawPos[0].x, drawPos[0].y - ruleOfThree(beta[1] - beta[0], CASE_SIZE, r_sqr - beta[0]));
                            window.drawLine(drawPos[3].x - ruleOfThree(beta[1] - beta[3], CASE_SIZE, r_sqr - beta[3]), drawPos[3].y, drawPos[3].x, drawPos[3].y + ruleOfThree(beta[2] - beta[3], CASE_SIZE, r_sqr - beta[3]));
                        }
                    }
                    else if(index[0] == 1 && index[1] == 2)
                    {
                        if(beta[4] > r_sqr)
                        {
                            window.drawLine(drawPos[0].x + ruleOfThree(beta[2] - beta[0], CASE_SIZE, r_sqr - beta[0]), drawPos[0].y, drawPos[0].x, drawPos[0].y - ruleOfThree(beta[1] - beta[0], CASE_SIZE, r_sqr - beta[0]));
                            window.drawLine(drawPos[3].x - ruleOfThree(beta[1] - beta[3], CASE_SIZE, r_sqr - beta[3]), drawPos[3].y, drawPos[3].x, drawPos[3].y + ruleOfThree(beta[2] - beta[3], CASE_SIZE, r_sqr - beta[3]));
                        }
                        else
                        {
                            window.drawLine(drawPos[1].x + ruleOfThree(beta[3] - beta[1], CASE_SIZE, r_sqr - beta[1]), drawPos[1].y, drawPos[1].x, drawPos[1].y + ruleOfThree(beta[0] - beta[1], CASE_SIZE, r_sqr - beta[1]));
                            window.drawLine(drawPos[2].x - ruleOfThree(beta[0] - beta[2], CASE_SIZE, r_sqr - beta[2]), drawPos[2].y, drawPos[2].x, drawPos[2].y - ruleOfThree(beta[3] - beta[2], CASE_SIZE, r_sqr - beta[2]));
                        }
                    }
                    else if(index[0] == 1 && index[1] == 3)
                    {
                        window.drawLine(drawPos[1].x, drawPos[1].y + ruleOfThree(beta[0] - beta[1], CASE_SIZE, r_sqr - beta[1]), drawPos[3].x, drawPos[3].y + ruleOfThree(beta[2] - beta[3], CASE_SIZE, r_sqr - beta[3]));
                    }
                    else if (index[0] == 2 && index[1] == 3)
                    {
                        window.drawLine(drawPos[2].x - ruleOfThree(beta[0] - beta[2], CASE_SIZE, r_sqr - beta[2]), drawPos[2].y, drawPos[3].x -ruleOfThree(beta[1] - beta[3], CASE_SIZE, r_sqr - beta[3]), drawPos[3].y);
                    }
                    break;
                case 3:
                    if(index[0] == 0 && index[1] == 1 && index[2] == 2)
                    {
                        window.drawLine(drawPos[3].x - ruleOfThree(beta[1] - beta[3], CASE_SIZE, r_sqr - beta[3]), drawPos[3].y, drawPos[3].x, drawPos[3].y + ruleOfThree(beta[2] - beta[3], CASE_SIZE, r_sqr - beta[3]));
                    }
                    else if(index[0] == 0 && index[1] == 1 && index[2] == 3)
                    {
                        window.drawLine(drawPos[2].x - ruleOfThree(beta[0] - beta[2], CASE_SIZE, r_sqr - beta[2]), drawPos[2].y, drawPos[2].x, drawPos[2].y - ruleOfThree(beta[3] - beta[2], CASE_SIZE, r_sqr - beta[2]));
                    }
                    else if(index[0] == 0 && index[1] == 2 && index[2] == 3)
                    {
                        window.drawLine(drawPos[1].x + ruleOfThree(beta[3] - beta[1], CASE_SIZE, r_sqr - beta[1]), drawPos[1].y, drawPos[1].x, drawPos[1].y + ruleOfThree(beta[0] - beta[1], CASE_SIZE, r_sqr - beta[1]));
                    }
                    else if(index[0] == 1 && index[1] == 2 && index[2] == 3)
                    {
                        window.drawLine(drawPos[0].x + ruleOfThree(beta[2] - beta[0], CASE_SIZE, r_sqr - beta[0]), drawPos[0].y, drawPos[0].x, drawPos[0].y - ruleOfThree(beta[1] - beta[0], CASE_SIZE, r_sqr - beta[0]));
                    }
                default:
                    break;
                }
}

void updateElectromagnetismArray(float timeStepSeconds, ElectromagnetismArray &e, bool pause)
{
    if(!pause)
    {
        for(int i = 0; i < e.nbParticles; i++)
        {
            e.particles[i].position += e.particles[i].velocity * timeStepSeconds;
            e.particles[i].position.print();
        }
    }
}
void drawElectromagnetismArray(ElectromagnetismArray e, RenderWindow &window)
{
    int i, j;
    for(i = -((WINDOW_WIDTH / CASE_SIZE)/2); i < (WINDOW_WIDTH / CASE_SIZE)/2; i++)
    {
        for(j = -((WINDOW_HEIGHT / CASE_SIZE)/2); j < (WINDOW_HEIGHT / CASE_SIZE)/2; j++)
        {
            window.color(0, 0, 255, 255);
            drawCalculedLine(window, e, i, j);
            float t = 1;
            for(int x = 0; x < e.nbParticles; x++)
                t +=  e.particles[x].radius / sqrt(pow(i - e.particles[x].position.x, 2) + pow(j - e.particles[x].position.y, 2));
            if(t <= 1)
                window.color(0, 255, 0, 255);
            else
                window.color(255, 0, 0, 255);
            Vector2f drawPos = initVector2f(i * CASE_SIZE, j * CASE_SIZE);
            window.drawPoint(drawPos.x + WINDOW_WIDTH / 2, drawPos.y + WINDOW_HEIGHT / 2);
        }
    }
}