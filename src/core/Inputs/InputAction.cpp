//
// Created by Antonin Fruchet on 26/05/2026.
//

#include "InputAction.h"

#include <algorithm>
#include <any>

#include "InputManager.h"

InputAction::InputAction(const std::string& label, const std::variant<std::vector<ButtonBinding>, std::vector<DirectionalBindings>>& bindings)
: label(label), bindings(bindings)
{
    if (std::holds_alternative<std::vector<ButtonBinding>>(this->bindings)) {
        m_value = false;
    } else {
        m_value = sf::Vector2f{0, 0};
    }
}

void InputAction::resetFrameState()
{
    pressedThisFrame = false;
}

void InputAction::processEvent(const std::optional<sf::Event> event, const std::set<sf::Keyboard::Key>& pressed_keys ) {
    if (!event.has_value()) {
        return;
    }

    // on met a jour m_value en fonction des clés appuyées dans pressed_key.
    // POUR Button
    if (const auto* buttonBindings = std::get_if<std::vector<ButtonBinding>>(&bindings)) {
        bool isPressed = false;
        for (const auto& binding : *buttonBindings) {
            if (pressed_keys.contains(binding.key)) {
                isPressed = true;
                break;
            }
        }

        bool wasPressedBefore = std::get<bool>(m_value);
        pressedThisFrame = !wasPressedBefore && isPressed;
        m_value = isPressed;
        return;
    }

    // on met a jour m_value en fonction des clés appuyées dans pressed_key.
    // POUR DirectionalBindings
    const auto& directionalBindings = std::get<std::vector<DirectionalBindings>>(bindings);
    sf::Vector2f value{0, 0};
    for (const auto& binding : directionalBindings) {
        if (pressed_keys.contains(binding.LEFT)) {
            value.x -= 1;
        }
        if (pressed_keys.contains(binding.RIGHT)) {
            value.x += 1;
        }
        if (pressed_keys.contains(binding.UP)) {
            value.y -= 1;
        }
        if (pressed_keys.contains(binding.DOWN)) {
            value.y += 1;
        }
    }

    value.x = std::clamp(value.x, -1.f, 1.f);
    value.y = std::clamp(value.y, -1.f, 1.f);
    m_value = value;
}

bool InputAction::wasPerformedThisFrame()
{
    return pressedThisFrame;
}

template <typename T>
T InputAction::ReadValue() const
{
    return std::get<T>(m_value);
}

template bool InputAction::ReadValue<bool>() const;
template sf::Vector2f InputAction::ReadValue<sf::Vector2f>() const;
