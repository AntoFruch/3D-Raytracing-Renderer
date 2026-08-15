add_library(lib_core
        App.cpp
        App.h
        Camera.cpp
        Camera.h
        InputManager.cpp
        InputManager.h
        tiny_obj_loader.h
        ObjectLoading.h
        Triangle.h
        ObjectLoading.cpp
)

target_include_directories(lib_core PUBLIC ./)
target_link_libraries(lib_core PUBLIC sfml-graphics)