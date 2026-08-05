//
// Created by Antonin Fruchet on 05/08/2026.
//

#include "InputManager.h"


void InputManager::processEvent(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        mKeys[keyPressed->code] = true;
    }

    if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>())
    {
        mKeys[keyReleased->code] = false;
    }
}


bool InputManager::isKeyDown(sf::Keyboard::Key key) const
{
    auto it = mKeys.find(key);

    if (it == mKeys.end())
        return false;

    return it->second;
}
