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
    ),
    mCanvas(static_cast<sf::Vector2f>(mWindow.getSize())),
    mCamera(
        {0,0,1},
        {0,0,-1},
        1)
{
    InputManager::init("res/input_config.xml");
    moveAction = InputManager::findAction("Move");
    jumpAction = InputManager::findAction("Jump");
    crouchAction =  InputManager::findAction("Crouch");

    mScene.push_back({
        {0.0, 0.0, -4.0},
        0.5,
        {1.0, 0.2, 0.3}
    });
    mScene.push_back({
        {1.0, 1.0, -3.0},
        0.7,
        {0.3, 0.2, 1.0}
    });

    // shader init
    std::filesystem::path shader_path = "res/shaders/sphere-raytracing.frag";
    if (!mShader.loadFromFile(shader_path, sf::Shader::Type::Fragment))
    {
        throw std::runtime_error(std::format(".frag shader file at {} could not be loaded.",shader_path.string()));
    }
    mShader.setUniform("u_resolution", static_cast<sf::Vector2f>(mWindow.getSize()));
    mCamera.updateForShader(&mShader);
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
    mCamera.move(moveAction, jumpAction, crouchAction, elapsedTime);
    mCamera.updateForShader(&mShader);

    // --- MISE À JOUR DU SHADER ---
    mShader.setUniform("u_resolution", sf::Glsl::Vec2(mWindow.getSize().x, mWindow.getSize().y));
    mShader.setUniform("u_sphereCount", static_cast<int>(mScene.size()));

    for (size_t i = 0; i < mScene.size(); ++i) {
        std::string prefix = "u_spheres[" + std::to_string(i) + "].";
        mShader.setUniform(prefix + "center", mScene[i].center);
        mShader.setUniform(prefix + "radius", mScene[i].radius);
        mShader.setUniform(prefix + "color", mScene[i].color);
    }
}

void App::render()
{
    mWindow.clear();

    mWindow.draw(mCanvas, &mShader);
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
            mShader.setUniform("u_resolution", newSize);
            mCanvas.setSize(newSize);
            mWindow.setView(sf::View(sf::FloatRect({0.f, 0.f}, newSize)));
        }

        // --- GESTION DE LA SOURIS (PAN/LOOK) ---
        if (const auto* mouseBtnPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseBtnPressed->button == sf::Mouse::Button::Right) {
                mIsMouseDragging = true;
                mLastMousePos = sf::Mouse::getPosition(mWindow);
            }
        }

        if (const auto* mouseBtnReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseBtnReleased->button == sf::Mouse::Button::Right) {
                mIsMouseDragging = false;
            }
        }

        if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
            if (mIsMouseDragging) {
                sf::Vector2i currentPos = mouseMoved->position;
                float deltaX = static_cast<float>(currentPos.x - mLastMousePos.x);
                float deltaY = static_cast<float>(currentPos.y - mLastMousePos.y);

                mCamera.look(deltaX, deltaY);
                mCamera.updateForShader(&mShader);
                mLastMousePos = currentPos;
            }
        }

        InputManager::processEvents(event);
    }
}
