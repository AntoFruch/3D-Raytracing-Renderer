//
// Created by Antonin Fruchet on 05/08/2026.
//

#ifndef RAYTRACING_APP_H
#define RAYTRACING_APP_H
#include "SFML/Graphics.hpp"

#define W_WIDTH 800
#define W_HEIGHT 600

class App {
    sf::RenderWindow mWindow{
        sf::VideoMode({W_WIDTH, W_HEIGHT}),
        "Ray-tracing"
    };

    static const sf::Time timePerFrame;

public:
    App() = default;
    void run();

private:
    void update(const sf::Time& elapsedTime);
    void render();
    void processEvents();

};



#endif //RAYTRACING_APP_H
