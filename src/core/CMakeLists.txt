add_library(lib_core
        App.cpp
        App.h
        Inputs/InputAction.cpp
        Inputs/InputManager.cpp
)

target_include_directories(lib_core PUBLIC ./)
target_link_libraries(lib_core PUBLIC sfml-graphics pugixml)