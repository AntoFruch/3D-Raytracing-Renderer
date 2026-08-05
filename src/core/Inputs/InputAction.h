//
// Created by Antonin Fruchet on 26/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_INPUTACTION_H
#define COMPONENT_BASED_ARCH_INPUTACTION_H
#include <optional>
#include <set>
#include <string>
#include <variant>
#include <vector>

#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"

#include "SFML/Window/Event.hpp"

struct DirectionalBindings
{
    sf::Keyboard::Key UP;
    sf::Keyboard::Key DOWN;
    sf::Keyboard::Key LEFT;
    sf::Keyboard::Key RIGHT;
};

struct ButtonBinding
{
    sf::Keyboard::Key key;
};

/** @brief Represents an Action with bindings attached to it.
 *
 */
class InputAction {
    /**
     * Name of the Action
     */
    const std::string_view label;
    /**
     * Value of the Action (boolean, or 2DVector)
     */
    std::variant<bool, sf::Vector2f> m_value;
    /**
     * True if the Action has been triggered this frame
     */
    bool pressedThisFrame{false};

    /**
     * List of bindings attached to the Action (ButtonBindings or DirectionalBinding depending of type of Action)
     */
    std::variant<std::vector<ButtonBinding>, std::vector<DirectionalBindings>> bindings;

public:
    explicit InputAction(const std::string& label, const std::variant<std::vector<ButtonBinding>, std::vector<DirectionalBindings>>& bindings);

    /**
     * @breaf resets pressedThisFrame, performed by InputManager::beginFrame() for each InputAction
     */
    void resetFrameState();
    /**
     * @breaf looks at pressed inputs if the SFML Event and compares the bindings of this action to
     */
    void processEvent(std::optional<sf::Event>, const std::set<sf::Keyboard::Key>& pressed_keys);

    bool wasPerformedThisFrame();

    template<typename T>
    T ReadValue() const;
};

#endif //COMPONENT_BASED_ARCH_INPUTACTION_H
