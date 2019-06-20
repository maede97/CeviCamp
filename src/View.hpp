#ifndef VIEW_HPP
#define VIEW_HPP

#include "Logger.hpp"
#include "Settings.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class View {
public:
    View(Settings* settings, Logger* logger);

    void openFrame();
    void closeFrame();
    void updateFrame();
    void clearFrame();

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
    sf::VideoMode vm = sf::VideoMode(1920, 1080);
    window.create(vm, settings_->title, sf::Style::Default);
    //window.setVerticalSyncEnabled(true);
    logger_->log("View", "openFrame");
}
void View::closeFrame()
{
    logger_->log("View", "closeFrame");
    window.close();
}
void View::clearFrame()
{
    window.clear(sf::Color::Black);
    window.display();
}
void View::updateFrame()
{
    window.clear(sf::Color::Black);
    // draw stuff here using window.draw for text
    window.display();
}

#endif