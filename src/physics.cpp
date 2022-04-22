// include public libraries
#include <cmath>

// include my libraries
#include "Physics.hpp"
#include "Math.hpp"
#include "Galaxy.hpp"

// calculate the Gravity force between two planets
Vector2f gForce(Planet p1, Planet p2)
{
    Vector2f force;
    double dist = distance(p1.position, p2.position);
    double forceMagnitude = (G * p1.mass * p2.mass) / pow(dist, 2);
    force.x = forceMagnitude * (p2.position.x - p1.position.x) / dist;
    force.y = forceMagnitude * (p2.position.y - p1.position.y) / dist;
    return force;
}