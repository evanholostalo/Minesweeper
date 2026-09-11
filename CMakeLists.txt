cmake_minimum_required(VERSION 3.10)
project(Minesweeper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Help CMake find Homebrew SFML on macOS (Apple Silicon or Intel).
if(APPLE)
    if(EXISTS "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")
        set(SFML_DIR "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")
    elseif(EXISTS "/opt/homebrew/opt/sfml/lib/cmake/SFML")
        set(SFML_DIR "/opt/homebrew/opt/sfml/lib/cmake/SFML")
    elseif(EXISTS "/usr/local/opt/sfml@2/lib/cmake/SFML")
        set(SFML_DIR "/usr/local/opt/sfml@2/lib/cmake/SFML")
    elseif(EXISTS "/usr/local/opt/sfml/lib/cmake/SFML")
        set(SFML_DIR "/usr/local/opt/sfml/lib/cmake/SFML")
    endif()
endif()

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(Minesweeper
    src/main.cpp
    src/TextureManager.cpp
    src/Tile.cpp
)

target_include_directories(Minesweeper PRIVATE src)

target_link_libraries(Minesweeper
    sfml-graphics
    sfml-window
    sfml-system
)

# Copy runtime assets next to the executable when they exist.
if(EXISTS "${CMAKE_SOURCE_DIR}/images")
    file(COPY "${CMAKE_SOURCE_DIR}/images" DESTINATION "${CMAKE_BINARY_DIR}")
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/boards")
    file(COPY "${CMAKE_SOURCE_DIR}/boards" DESTINATION "${CMAKE_BINARY_DIR}")
endif()
