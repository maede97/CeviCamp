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
};

View::View(Settings* settings, Logger* logger)
{
    settings_ = settings;
    logger_ = logger;
}


void View::hideCursor() {
    window.setMouseCursorVisible(true);
}
void View::showCursor() {
    window.setMouseCursorVisible(true);
}

void View::openFrame()
{
    sf::VideoMode vm = sf::VideoMode(settings_->screenWidth, settings_->screenHeight);
    window.create(vm, settings_->title, sf::Style::Default);   

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