#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <SFML/Graphics.hpp>

class Screen {
public:
    virtual void show(sf::RenderWindow&) = 0;
};

#endif