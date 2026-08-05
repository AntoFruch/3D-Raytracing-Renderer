cmake_minimum_required(VERSION 4.2)

include (FetchContent)

project(project VERSION 1.0.0 LANGUAGES CXX)

# =========== SFML ====================
fetchContent_Declare( sfml
        URL https://github.com/SFML/SFML/archive/refs/tags/3.0.2.tar.gz
)
set (BUILD_SHARED_LIBS FALSE)

option(SFML_BUILD_AUDIO "Build audio" OFF)
option(SFML_BUILD_NETWORK "Build network" OFF)
FetchContent_MakeAvailable(sfml)
# ========================================

# =========== PUGIXML ====================
FetchContent_Declare(
        pugixml
        URL https://github.com/zeux/pugixml/releases/download/v1.15/pugixml-1.15.tar.gz
)
FetchContent_MakeAvailable(pugixml)
# ========================================

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src/core)
add_subdirectory(src/main)