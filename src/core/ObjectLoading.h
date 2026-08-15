//
// Created by Antonin Fruchet on 15/08/2026.
//
#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H

#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "Triangle.h"

void loadOBJ(std::vector<Triangle>& buffer, const std::string& filePath, const sf::Vector3f& defaultColor = {0.8f, 0.8f, 0.8f});

struct SceneTextureInfo {
    sf::Vector3f minBounds{};
    sf::Vector3f maxBounds{};
};

SceneTextureInfo createSceneTexture(const std::vector<Triangle>& scene, sf::Texture& texture);


#endif //OBJECT_LOADER_H
