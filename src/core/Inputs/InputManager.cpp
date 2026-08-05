//
// Created by Antonin Fruchet on 26/05/2026.
//

#include "InputManager.h"

#include <iostream>

#include "pugixml.hpp"

const std::unordered_map<std::string, sf::Keyboard::Key> stringToKey = {
    {"Unknown", sf::Keyboard::Key::Unknown},
    {"A", sf::Keyboard::Key::A},
    {"B", sf::Keyboard::Key::B},
    {"C", sf::Keyboard::Key::C},
    {"D", sf::Keyboard::Key::D},
    {"E", sf::Keyboard::Key::E},
    {"F", sf::Keyboard::Key::F},
    {"G", sf::Keyboard::Key::G},
    {"H", sf::Keyboard::Key::H},
    {"I", sf::Keyboard::Key::I},
    {"J", sf::Keyboard::Key::J},
    {"K", sf::Keyboard::Key::K},
    {"L", sf::Keyboard::Key::L},
    {"M", sf::Keyboard::Key::M},
    {"N", sf::Keyboard::Key::N},
    {"O", sf::Keyboard::Key::O},
    {"P", sf::Keyboard::Key::P},
    {"Q", sf::Keyboard::Key::Q},
    {"R", sf::Keyboard::Key::R},
    {"S", sf::Keyboard::Key::S},
    {"T", sf::Keyboard::Key::T},
    {"U", sf::Keyboard::Key::U},
    {"V", sf::Keyboard::Key::V},
    {"W", sf::Keyboard::Key::W},
    {"X", sf::Keyboard::Key::X},
    {"Y", sf::Keyboard::Key::Y},
    {"Z", sf::Keyboard::Key::Z},
    {"Num0", sf::Keyboard::Key::Num0},
    {"Num1", sf::Keyboard::Key::Num1},
    {"Num2", sf::Keyboard::Key::Num2},
    {"Num3", sf::Keyboard::Key::Num3},
    {"Num4", sf::Keyboard::Key::Num4},
    {"Num5", sf::Keyboard::Key::Num5},
    {"Num6", sf::Keyboard::Key::Num6},
    {"Num7", sf::Keyboard::Key::Num7},
    {"Num8", sf::Keyboard::Key::Num8},
    {"Num9", sf::Keyboard::Key::Num9},
    {"Escape", sf::Keyboard::Key::Escape},
    {"LControl", sf::Keyboard::Key::LControl},
    {"LShift", sf::Keyboard::Key::LShift},
    {"LAlt", sf::Keyboard::Key::LAlt},
    {"LSystem", sf::Keyboard::Key::LSystem},
    {"RControl", sf::Keyboard::Key::RControl},
    {"RShift", sf::Keyboard::Key::RShift},
    {"RAlt", sf::Keyboard::Key::RAlt},
    {"RSystem", sf::Keyboard::Key::RSystem},
    {"Menu", sf::Keyboard::Key::Menu},
    {"LBracket", sf::Keyboard::Key::LBracket},
    {"RBracket", sf::Keyboard::Key::RBracket},
    {"Semicolon", sf::Keyboard::Key::Semicolon},
    {"Comma", sf::Keyboard::Key::Comma},
    {"Period", sf::Keyboard::Key::Period},
    {"Apostrophe", sf::Keyboard::Key::Apostrophe},
    {"Slash", sf::Keyboard::Key::Slash},
    {"Backslash", sf::Keyboard::Key::Backslash},
    {"Grave", sf::Keyboard::Key::Grave},
    {"Equal", sf::Keyboard::Key::Equal},
    {"Hyphen", sf::Keyboard::Key::Hyphen},
    {"Space", sf::Keyboard::Key::Space},
    {"Enter", sf::Keyboard::Key::Enter},
    {"Backspace", sf::Keyboard::Key::Backspace},
    {"Tab", sf::Keyboard::Key::Tab},
    {"PageUp", sf::Keyboard::Key::PageUp},
    {"PageDown", sf::Keyboard::Key::PageDown},
    {"End", sf::Keyboard::Key::End},
    {"Home", sf::Keyboard::Key::Home},
    {"Insert", sf::Keyboard::Key::Insert},
    {"Delete", sf::Keyboard::Key::Delete},
    {"Add", sf::Keyboard::Key::Add},
    {"Subtract", sf::Keyboard::Key::Subtract},
    {"Multiply", sf::Keyboard::Key::Multiply},
    {"Divide", sf::Keyboard::Key::Divide},
    {"Left", sf::Keyboard::Key::Left},
    {"Right", sf::Keyboard::Key::Right},
    {"Up", sf::Keyboard::Key::Up},
    {"Down", sf::Keyboard::Key::Down},
    {"Numpad0", sf::Keyboard::Key::Numpad0},
    {"Numpad1", sf::Keyboard::Key::Numpad1},
    {"Numpad2", sf::Keyboard::Key::Numpad2},
    {"Numpad3", sf::Keyboard::Key::Numpad3},
    {"Numpad4", sf::Keyboard::Key::Numpad4},
    {"Numpad5", sf::Keyboard::Key::Numpad5},
    {"Numpad6", sf::Keyboard::Key::Numpad6},
    {"Numpad7", sf::Keyboard::Key::Numpad7},
    {"Numpad8", sf::Keyboard::Key::Numpad8},
    {"Numpad9", sf::Keyboard::Key::Numpad9},
    {"F1", sf::Keyboard::Key::F1},
    {"F2", sf::Keyboard::Key::F2},
    {"F3", sf::Keyboard::Key::F3},
    {"F4", sf::Keyboard::Key::F4},
    {"F5", sf::Keyboard::Key::F5},
    {"F6", sf::Keyboard::Key::F6},
    {"F7", sf::Keyboard::Key::F7},
    {"F8", sf::Keyboard::Key::F8},
    {"F9", sf::Keyboard::Key::F9},
    {"F10", sf::Keyboard::Key::F10},
    {"F11", sf::Keyboard::Key::F11},
    {"F12", sf::Keyboard::Key::F12},
    {"F13", sf::Keyboard::Key::F13},
    {"F14", sf::Keyboard::Key::F14},
    {"F15", sf::Keyboard::Key::F15},
    {"Pause", sf::Keyboard::Key::Pause},
};
std::unordered_map<std::string, std::unique_ptr<InputAction>> InputManager::actions{};
std::set<sf::Keyboard::Key> InputManager::pressed_keys{};

std::vector<DirectionalBindings> create_directional_bindings(pugi::xml_node node)
{
    std::vector<DirectionalBindings> vector;
    for (const auto& binding : node.children())
    {
        DirectionalBindings b{
            stringToKey.at(binding.child("up").attribute("bind").as_string()),
            stringToKey.at(binding.child("down").attribute("bind").as_string()),
            stringToKey.at(binding.child("left").attribute("bind").as_string()),
            stringToKey.at(binding.child("right").attribute("bind").as_string())
        };
        vector.push_back(b);
    }
    return vector;
}
std::vector<ButtonBinding> create_button_bindings(pugi::xml_node node)
{
    std::vector<ButtonBinding> vector;
    for (const auto& binding : node.children())
    {
        ButtonBinding b{
            stringToKey.at(binding.attribute("bind").as_string()),
        };
        vector.push_back(b);
    }
    return vector;
}

void InputManager::loadActions()
{
    pugi::xml_document doc;
    if (auto result = doc.load_file(input_config_file_path.c_str()); !result)
    {
        throw std::runtime_error(std::format("could not open file {} because : {}\n", input_config_file_path.string(), result.description()));
    }

    for (const auto& node : doc.child("InputActions").children())
    {
        if (std::string name = node.name(); name != "Action") continue;

        std::string label{node.attribute("label").as_string()};
        std::string type{node.attribute("type").as_string()};
        if (type == "Vector2D") {
            actions.try_emplace(label, std::make_unique<InputAction>(label, create_directional_bindings(node)));
            continue;
        }
        if (type == "Button") {
            actions.try_emplace(label, std::make_unique<InputAction>(label, create_button_bindings(node)));
            continue;
        }
        throw std::runtime_error("Other input types than Vector2D or Button are not supported");
    }
}

void InputManager::init(const std::filesystem::path& input_config_file_path)
{
    InputManager::input_config_file_path = input_config_file_path;
    loadActions();
}

InputAction* InputManager::findAction(const std::string& action_name)
{
    if (!actions.contains(action_name))
    {
        throw std::runtime_error(std::format("{} is not a known action", action_name));
    }
    return actions.at(action_name).get();
}

void InputManager::processEvents(std::optional<sf::Event> event)
{
    // On regarde les touches appuyées et on les met/enleve de pressed_key
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        pressed_keys.insert(keyPressed->code);
    } else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
        pressed_keys.erase(keyReleased->code);
    } else {
        return;
    }

    for (const auto& [name, action] : actions) {
        action->processEvent(event, pressed_keys);
    }
}

void InputManager::beginFrame()
{
    for (const auto& [name, action] : actions)
    {
        action->resetFrameState();
    }
}