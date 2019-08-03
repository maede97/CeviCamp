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
    void clearFrame();
    void displayFrame();

    void resetViews();
    void resetZoomLevel();

    sf::RenderWindow window;

    sf::View gameView;
    sf::View miniMapView;
    sf::View originalView;

private:
    Settings* settings_;
    Logger* logger_;
    sf::VideoMode vm_;
    sf::Clock frameClock_;
};

View::View(Settings* settings, Logger* logger)
{
    settings_ = settings;
    logger_ = logger;

    vm_ = sf::VideoMode::getFullscreenModes().at(0);
    settings_->screenHeight = vm_.height;
    settings_->screenWidth = vm_.width;

    settings_->recalculateScaling();

    // set up views
    resetViews();
}

void View::resetZoomLevel() {
    gameView.reset(sf::FloatRect(0.f, 0.f, settings_->mapWidth, settings_->mapHeight));
    gameView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    gameView.setSize(originalView.getSize());
}

void View::resetViews()
{
    gameView.reset(sf::FloatRect(0.f, 0.f, settings_->mapWidth, settings_->mapHeight));
    gameView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

    miniMapView.reset(sf::FloatRect(0.f, 0.f, settings_->mapWidth, settings_->mapHeight));
    miniMapView.setViewport(sf::FloatRect(0.8f, 0.f, 0.2f, 0.2f)); // TODO: fix scaling issue (aspect ratio)

    originalView.reset(sf::FloatRect(0.f, 0.f, settings_->screenWidth, settings_->screenHeight));
    originalView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
}

void View::openFrame()
{
    window.create(vm_, settings_->title, sf::Style::Fullscreen);

    // only one of those:
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(60);

    sf::Image icon;
    if (!icon.loadFromFile("res/icon.png")) {
        logger_->error("View", "res/icon.png could not be opened");
        return;
    }
    window.setIcon(128, 128, icon.getPixelsPtr());
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
}

#endif