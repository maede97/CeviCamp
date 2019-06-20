cmake_minimum_required(VERSION 3.1)

project(CeviCamp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_HOME_DIRECTORY}/cmake_modules/")

set(CMAKE_INSTALL_PREFIX ${CMAKE_HOME_DIRECTORY}/install)
find_package(SFML 2.4 REQUIRED system window graphics network audio)

add_executable(main src/main.cpp)

target_link_libraries(main ${SFML_LIBRARIES})

install(TARGETS main DESTINATION bin)
