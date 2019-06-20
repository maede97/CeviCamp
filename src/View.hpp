#ifndef VIEW_HPP
#define VIEW_HPP

#include "Settings.hpp"
#include "Logger.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class View {
public:
    View(Settings* settings, Logger* logger);

    void openFrame();
    void closeFrame();
    void updateFrame();

    sf::RenderWindow window;

private:
    Settings* settings_;
    Logger* logger_;
};

View::View(Settings* settings, Logger* logger)
{
    settings_ = settings;
    logger_ = logger;
}

void View::openFrame()
{
    sf::VideoMode vm = sf::VideoMode::getFullscreenModes().at(0);
    window.create(vm, settings_->title, sf::Style::Fullscreen);
    logger_->log("View","openFrame");
}
void View::closeFrame()
{
    logger_->log("View","closeFrame");
    window.close();
}
void View::updateFrame()
{
    window.clear(sf::Color::Black);
    // draw stuff here using window.draw for text
    window.display();
}

#endif