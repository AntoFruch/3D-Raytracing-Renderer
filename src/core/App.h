//
// Created by Antonin Fruchet on 05/08/2026.
//

#ifndef RAYTRACING_APP_H
#define RAYTRACING_APP_H

#include "Camera.h"
#include "Inputs/InputAction.h"
#include "SFML/Graphics.hpp"

#define PROJECT_NAME "3D Raytracing renderer"

#define W_WIDTH 800
#define W_HEIGHT 600

struct Sphere {
    sf::Glsl::Vec3 center;
    float radius;
    sf::Glsl::Vec3 color;
};

class App {
    sf::RenderWindow mWindow;
    sf::RectangleShape mCanvas;
    sf::Shader mShader;

    Camera mCamera;
    std::vector<Sphere> mScene{};

    InputAction* moveAction;
    InputAction* jumpAction;
    InputAction* crouchAction;

    static const sf::Time timePerFrame;

    //FIXME : gerer la souris comme une inputAction
    sf::Vector2i mLastMousePos;
    bool mIsMouseDragging = false;
public:
    App();
    void run();

private:
    void update(const sf::Time& elapsedTime);
    void render();
    void processEvents();
};



#endif //RAYTRACING_APP_H
