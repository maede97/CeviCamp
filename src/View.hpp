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
    void hideCursor();
    void showCursor();

    sf::RenderWindow window;

private:
    Settings* settings_;
    Logger* logger_;
    sf::VideoMode vm_;
};

View::View(Settings* settings, Logger* logger)
{
    settings_ = settings;
    logger_ = logger;

    vm_ = sf::VideoMode::getFullscreenModes().at(0);
    settings_->screenHeight = vm_.height;
    settings_->screenWidth = vm_.width;
    settings_->mapHeight = 2*vm_.height;
    settings_->mapWidth = 2*vm_.width;
}


void View::hideCursor() {
    window.setMouseCursorVisible(true);
}
void View::showCursor() {
    window.setMouseCursorVisible(true);
}

void View::openFrame()
{
    window.create(vm_, settings_->title, sf::Style::Fullscreen);  

    // only one of those:
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(60);

    logger_->info("View", "openFrame");
}
void View::closeFrame()
{
    logger_->info("View", "closeFrame");
    window.close();
}
void View::clearFrame()
{
    window.clear(sf::Color::Black);
}
void View::updateFrame()
{
    window.clear(sf::Color::Black);
    // draw stuff here using window.draw for text
    window.display();
}

#endif