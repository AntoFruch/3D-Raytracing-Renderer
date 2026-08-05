//
// Created by Antonin Fruchet on 05/08/2026.
//

#ifndef RAYTRACING_INPUTMANAGER_H
#define RAYTRACING_INPUTMANAGER_H


#include <unordered_map>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
class InputManager
{
public:
    void processEvent(const sf::Event& event);

    bool isKeyDown(sf::Keyboard::Key key) const;

private:
    std::unordered_map<sf::Keyboard::Key, bool> mKeys;
};



#endif //RAYTRACING_INPUTMANAGER_H
