#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "../Settings.hpp"
#include "../Logger.hpp"

class Screen {
public:
    Screen(Logger* logger, Settings* settings) {
        settings_ = settings;
        logger_ = logger;
    }
    virtual void show(sf::RenderWindow&) = 0;
protected:
    Settings* settings_;
    Logger* logger_;
};

#endif