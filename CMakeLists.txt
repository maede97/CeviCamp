cmake_minimum_required(VERSION 3.1)

project(CeviCamp)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RELEASE" CACHE STRING "Choose the type ot the build type" FORCE)
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_HOME_DIRECTORY}/install)

# on windows, do this:
if(WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
endif()
find_package(SFML 2.5 COMPONENTS audio graphics window system)

if(SFML_FOUND)
    add_executable(main src/main.cpp)
    target_link_libraries(main sfml-audio sfml-graphics sfml-window sfml-system)
    install(TARGETS main DESTINATION bin)
endif()
