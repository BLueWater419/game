cmake_minimum_required(VERSION 3.10.0)
project(game VERSION 0.1.0 LANGUAGES C CXX)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/SFML-3.0.0-mingw/include)

add_executable(game "./code/main.cpp" "./code/parse.cpp" "./code/render.cpp" "./include/render.hpp" "./include/parse.hpp")

target_link_libraries(game 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/libraries/SFML-3.0.0-mingw/lib/libsfml-graphics.a
    ${CMAKE_SOURCE_DIR}/libraries/SFML-3.0.0-mingw/lib/libsfml-window.a
    ${CMAKE_SOURCE_DIR}/libraries/SFML-3.0.0-mingw/lib/libsfml-system.a
    ${CMAKE_SOURCE_DIR}/libraries/SFML-3.0.0-mingw/lib/libsfml-audio.a
    ${CMAKE_SOURCE_DIR}/libraries/SFML-3.0.0-mingw/lib/libsfml-network.a
)

#set(SFML_DIR "${CMAKE_SOURCE_DIR}/libraries/SFML-3.0.0/lib/cmake/SFML")

#find_package(SFML 3.0.0# COMPONENTS graphics window system REQUIRED)
#target_link_libraries(MySFMLProject PRIVATE sfml-graphics sfml-window sfml-system)

#target_link_libraries(game ${CMAKE_SOURCE_DIR}/libraries/SFML-3.0.0/lib/sfml-audio.lib)
#target_link_libraries(game ${CMAKE_SOURCE_DIR}/libraries/SFML-3.0.0/lib/sfml-graphics.lib)
#target_link_libraries(game ${CMAKE_SOURCE_DIR}/libraries/SFML-3.0.0/lib/sfml-window.lib)
#target_link_libraries(game ${CMAKE_SOURCE_DIR}/libraries/SFML-3.0.0/lib/sfml-system.lib)
#target_link_libraries(game ${CMAKE_SOURCE_DIR}/libraries/SFML-3.0.0/lib/sfml-network.lib)