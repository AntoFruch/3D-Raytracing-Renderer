//
// Created by Antonin Fruchet on 05/08/2026.
//

#include "App.h"

const sf::Time App::timePerFrame = sf::seconds(1.f / 60.f);


void App::run()
{
    sf::Clock clock;
    static sf::Time timeSinceLastUpdate = sf::Time::Zero;
    mWindow.setVerticalSyncEnabled(true);
    while (mWindow.isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;

            processEvents();
            update(timePerFrame);
        }
        render();
    }
}

void App::update(const sf::Time& elapsedTime)
{

}

void App::render()
{
    mWindow.clear(sf::Color::White);
    mWindow.display();
}

void App::processEvents()
{
    while (const std::optional event = mWindow.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            mWindow.close();
        }
    }
}
