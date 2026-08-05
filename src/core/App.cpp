//
// Created by Antonin Fruchet on 05/08/2026.
//

#include "App.h"

#include <iostream>
#include "Inputs/InputManager.h"

const sf::Time App::timePerFrame = sf::seconds(1.f / 60.f);

App::App() :
    mWindow(
        sf::VideoMode({W_WIDTH, W_HEIGHT}),
        PROJECT_NAME
    )
{
    InputManager::init("res/input_config.xml");
    moveAction = InputManager::findAction("Move");
}

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
    std::cout << moveAction->ReadValue<sf::Vector2f>().x
              << " "
              << moveAction->ReadValue<sf::Vector2f>().y
              << std::endl;
}

void App::render()
{
    mWindow.clear();
    // draw things here
    mWindow.display();
}


void App::processEvents()
{
    InputManager::beginFrame();

    while (const std::optional event = mWindow.pollEvent()) {
        // close window
        if (event->is<sf::Event::Closed>()) {
            mWindow.close();
        }
        // resize window
        if (const auto* resizeEvent = event->getIf<sf::Event::Resized>())
        {
            sf::Vector2f newSize(static_cast<float>(resizeEvent->size.x), static_cast<float>(resizeEvent->size.y));
            mWindow.setView(sf::View(sf::FloatRect({0.f, 0.f}, newSize)));
        }

        InputManager::processEvents(event);
    }
}
