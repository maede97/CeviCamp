#ifndef VIEW_HPP
#define VIEW_HPP

#include "Logger.hpp"
#include "Settings.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class View
{
public:
    View(Settings *settings, Logger *logger);

    void openFrame();
    void closeFrame();
    void clearFrame();
    void displayFrame();

    sf::RenderWindow window;

private:
    Settings *settings_;
    Logger *logger_;
    sf::VideoMode vm_;
    sf::Clock frameClock_;
};

View::View(Settings *settings, Logger *logger)
{
    settings_ = settings;
    logger_ = logger;

    vm_ = sf::VideoMode::getFullscreenModes().at(0);
    settings_->screenHeight = vm_.height;
    settings_->screenWidth = vm_.width;

    settings_->recalculateScaling();
}

void View::openFrame()
{
    window.create(vm_, settings_->title, sf::Style::Fullscreen);

    // only one of those:
    //window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

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

void View::displayFrame()
{
    window.display();

    sf::Time frameDuration = frameClock_.restart();
    // 60 FPS means 1000ms / 60F = 16.66 ms per Frame, meaning if frameDuration < 17ms, wait the difference
    if (frameDuration.asMicroseconds() < sf::Int64(16667))
    {
        //sf::sleep(sf::microseconds(sf::Int64(16667) - frameDuration.asMicroseconds()));
        //std::cout << "TIME " << sf::Int64(16667) - frameDuration.asMicroseconds() << std::endl;
    }
}

#endif