//
// Created by Antonin Fruchet on 15/08/2026.
//

#ifndef RAYTRACING_TRIANGLE_H
#define RAYTRACING_TRIANGLE_H
#include "SFML/Graphics.hpp"

struct Triangle {
    sf::Vector3f v0;
    sf::Vector3f v1;
    sf::Vector3f v2;
    sf::Vector3f color;
};

#endif //RAYTRACING_TRIANGLE_H
