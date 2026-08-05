//
// Created by Antonin Fruchet on 05/08/2026.
//

#ifndef RAYTRACING_APP_H
#define RAYTRACING_APP_H
#include "Inputs/InputAction.h"
#include "SFML/Graphics.hpp"

#define PROJECT_NAME "Interactive SFML project"

#define W_WIDTH 800
#define W_HEIGHT 600

class App {
    sf::RenderWindow mWindow;

    static const sf::Time timePerFrame;

    InputAction* moveAction;
public:
    App();
    void run();

private:
    void update(const sf::Time& elapsedTime);
    void render();
    void processEvents();

};



#endif //RAYTRACING_APP_H
