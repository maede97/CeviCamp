cmake_minimum_required(VERSION 3.1)

project(CeviCamp)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RELEASE" CACHE STRING "Choose the type ot the build type" FORCE)
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_HOME_DIRECTORY}/install)

if(WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    if(CMAKE_BUILD_TYPE MATCHES RELEASE)
        add_executable(main WIN32 src/main.cpp ${SRCS} project.rc)
    else()
        add_executable(main src/main.cpp ${SRCS} project.rc)
    endif()
else()
    add_executable(main src/main.cpp)
endif()

find_package(SFML 2.5 COMPONENTS audio graphics window system main)

if(SFML_FOUND)
    if(CMAKE_BUILD_TYPE MATCHES RELEASE)
        target_link_libraries(main sfml-audio sfml-graphics sfml-window sfml-system sfml-main)
    else()
        target_link_libraries(main sfml-audio sfml-graphics sfml-window sfml-system)
    endif()
    install(TARGETS main DESTINATION bin)
endif()
