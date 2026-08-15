//
// Created by Antonin Fruchet on 05/08/2026.
//

#include "App.h"

#include <iostream>

#include "SFML/Graphics/Shader.hpp"

const sf::Time App::timePerFrame = sf::seconds(1.f / 60.f);

App::App() :
    mWindow(
        sf::VideoMode({W_WIDTH, W_HEIGHT}),
        "Ray-tracing"
    ),
    mCanvas(static_cast<sf::Vector2f>(mWindow.getSize())),
    mCamera(
        {0,0,4},
        {0,0,-1},
        1
        )
{

    // shader init
    std::filesystem::path shader_path = "res/shaders/obj-rtx.frag";
    if (!mShader.loadFromFile(shader_path, sf::Shader::Type::Fragment))
    {
        throw std::runtime_error(std::format(".frag shader file at {} could not be loaded.",shader_path.string()));
    }

    loadOBJ(mScene, "res/obj/cow.obj");
    const SceneTextureInfo sceneTextureInfo = createSceneTexture(mScene, mSceneTexture);

    mShader.setUniform("u_sceneTexture", mSceneTexture);
    mShader.setUniform("u_triangleCount", static_cast<int>(mScene.size()));
    mShader.setUniform("u_sceneMinBounds", sceneTextureInfo.minBounds);
    mShader.setUniform("u_sceneMaxBounds", sceneTextureInfo.maxBounds);
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
    float forward = 0.f;
    float right = 0.f;
    float up = 0.f;


    if (mInput.isKeyDown(sf::Keyboard::Key::Z))
        forward += 1.f;

    if (mInput.isKeyDown(sf::Keyboard::Key::S))
        forward -= 1.f;


    if (mInput.isKeyDown(sf::Keyboard::Key::D))
        right += 1.f;

    if (mInput.isKeyDown(sf::Keyboard::Key::Q))
        right -= 1.f;


    if (mInput.isKeyDown(sf::Keyboard::Key::Space))
        up += 1.f;

    if (mInput.isKeyDown(sf::Keyboard::Key::LShift))
        up -= 1.f;


    if (forward != 0.f || right != 0.f || up != 0.f)
    {
        mCamera.move(
            forward,
            right,
            up,
            elapsedTime.asSeconds()
        );

        mCamera.updateForShader(&mShader);
    }

    // --- MISE À JOUR DU SHADER ---
    mShader.setUniform("u_resolution", sf::Glsl::Vec2(mWindow.getSize().x, mWindow.getSize().y));
}

void App::render()
{
    mWindow.clear();

    mWindow.draw(mCanvas, &mShader);
    mWindow.display();
}


void App::processEvents()
{
    while (const std::optional event = mWindow.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            mWindow.close();
        }

        if (const auto* resizeEvent = event->getIf<sf::Event::Resized>())
        {
            sf::Vector2f newSize(static_cast<float>(resizeEvent->size.x), static_cast<float>(resizeEvent->size.y));

            // 1. On met à jour le shader
            mShader.setUniform("u_resolution", newSize);

            // 2. On met à jour la taille du canvas pour qu'il remplisse la fenêtre
            mCanvas.setSize(newSize);

            // 3. On met à jour la vue de la fenêtre pour éviter l'étirement (stretching) par défaut de SFML
            mWindow.setView(sf::View(sf::FloatRect({0.f, 0.f}, newSize)));
        }

        // --- GESTION DE LA SOURIS (PAN/LOOK) ---
        if (const auto* mouseBtnPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseBtnPressed->button == sf::Mouse::Button::Left) {
                mIsMouseDragging = true;
                mLastMousePos = sf::Mouse::getPosition(mWindow);
            }
        }

        if (const auto* mouseBtnReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseBtnReleased->button == sf::Mouse::Button::Left) {
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

        mInput.processEvent(*event);
    }
}
