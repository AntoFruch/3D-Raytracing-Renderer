add_library(lib_core
        App.cpp
        App.h
        Camera.cpp
        Camera.h
        InputManager.cpp
        InputManager.h
)

target_include_directories(lib_core PUBLIC ./)
target_link_libraries(lib_core PUBLIC sfml-graphics)