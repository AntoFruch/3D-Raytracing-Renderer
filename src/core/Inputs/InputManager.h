//
// Created by Antonin Fruchet on 26/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_INPUTMANAGER_H
#define COMPONENT_BASED_ARCH_INPUTMANAGER_H
#include <filesystem>
#include <map>
#include <unordered_map>

#include "InputAction.h"

class InputManager {


    inline static std::filesystem::path input_config_file_path{"res/input_config.xml"};

    const std::unordered_map<std::string, sf::Keyboard::Key> stringToKey;

    static std::unordered_map<std::string, std::unique_ptr<InputAction>> actions;

    /**
     * Currently pressed keys
     */
    static std::set<sf::Keyboard::Key> pressed_keys;

    static void loadActions();
public:
    static void init(const std::filesystem::path& input_config_file_path);
    static InputAction* findAction(const std::string& action_name);

    static void beginFrame();
    static void processEvents(std::optional<sf::Event> event);
};



#endif //COMPONENT_BASED_ARCH_INPUTMANAGER_H
