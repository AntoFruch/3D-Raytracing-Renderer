//
// Created by Antonin Fruchet on 05/08/2026.
//

#ifndef RAYTRACING_APP_H
#define RAYTRACING_APP_H
#include "Camera.h"
#include "InputManager.h"
#include "SFML/Graphics.hpp"

#define W_WIDTH 800
#define W_HEIGHT 600

struct Sphere {
    sf::Glsl::Vec3 center;
    float radius;
    sf::Glsl::Vec3 color;
};

class App {
    sf::RenderWindow mWindow;
    InputManager mInput;

    sf::RectangleShape mCanvas;
    sf::Shader mShader;

    Camera mCamera;
    std::vector<Sphere> mScene{};

    static const sf::Time timePerFrame;

    sf::Vector2i mLastMousePos;
    bool mIsMouseDragging = false;

public:
    App();
    void run();

private:
    void update(const sf::Time& elapsedTime);
    void render();
    void processEvents();
    void handleInput();

};



#endif //RAYTRACING_APP_H
