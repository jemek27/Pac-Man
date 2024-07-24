cmake_minimum_required(VERSION 3.26)
project(Pac-Man)

set(CMAKE_CXX_STANDARD 20)

add_executable(Projekt src/main.cpp
        src/MovingObj/MovingObj.cpp
        headers/MovingObj/MovingObj.h
        src/Game.cpp
        headers/Game.h
        src/Tiles/Tile.cpp
        headers/Tiles/Tile.h
        src/Tiles/Wall.cpp
        headers/Tiles/Wall.h
        src/Tiles/Gate.cpp
        headers/Tiles/Gate.h
        src/Tiles/Path.cpp
        headers/Tiles/Path.h
        src/Tiles/Portal.cpp
        headers/Tiles/Portal.h
        src/MovingObj/PacMan.cpp
        headers/MovingObj/PacMan.h
        headers/MovingObj/PacMan.h
        src/Tiles/Background.cpp
        headers/Tiles/Background.h
        src/StationaryObj/StationaryObj.cpp
        headers/StationaryObj/StationaryObj.h
        headers/StationaryObj/StationaryObj.h
        src/StationaryObj/Point.cpp
        headers/StationaryObj/Point.h
        src/StationaryObj/Fruit.cpp
        headers/StationaryObj/Fruit.h
        src/StationaryObj/Upgrade.cpp
        headers/StationaryObj/Upgrade.h
        src/NumberDisplay.cpp
        headers/NumberDisplay.h
        src/Menu/Menu.cpp
        headers/Menu/Menu.h
        src/Menu/MainMenu.cpp
        headers/Menu/MainMenu.h
        src/FileInputOutputManager.cpp
        headers/FileInputOutputManager.h
        src/Menu/PauseMenu.cpp
        headers/Menu/PauseMenu.h
        src/MovingObj/Ghost.cpp
        headers/MovingObj/Ghost.h
        src/GameMap.cpp
        headers/GameMap.h
)

include_directories(headers /usr/include c:/SFML/include)

set(SFML_ROOT c:/SFML)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(Projekt ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# Copy dlls to build
if(WIN32)
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    file(GLOB MINGW_DEP_DLLS "C:/mingw64/bin/*.dll")
    file(COPY ${MINGW_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()