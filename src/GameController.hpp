#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include "View.hpp"
#include "Logger.hpp"

class GameController {
public:
    GameController(Logger* logger);
    void start();
    ~GameController();

private:
    enum GameState { Uninitialized,
        Paused,
        Playing,
        Exiting,
        MainMenu };
    bool isExiting();
    void gameLoop();

    GameState gameState_ = Uninitialized;
    Settings* settings_;
    View* view_;
    Logger* logger_;
};

GameController::GameController(Logger* logger)
{
    logger_ = logger;
    settings_ = new Settings(logger_);
    view_ = new View(settings_, logger_);
}

void GameController::start()
{
    if(gameState_ != Uninitialized) return;

    // read settings
    settings_->readSettingsFromFile();
    view_->openFrame();

    while (!isExiting()) {
        gameLoop();
        view_->updateFrame();
    }
    view_->closeFrame();
    // settings_->saveSettingsToFile();
}

bool GameController::isExiting()
{
    if (gameState_ == Exiting)
        return true;
    return false;
}

GameController::~GameController()
{
    delete view_;
    delete settings_;
}

void GameController::gameLoop() {
    sf::Event currentEvent;
    while(view_->window.pollEvent(currentEvent)) {
        switch(currentEvent.type) {
            case sf::Event::Closed:
                gameState_ = Exiting;
                break;
            case sf::Event::EventType::KeyPressed:
                logger_->log("GameController","Key pressed");
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    gameState_ = Exiting;
                }
                break;
            case sf::Event::EventType::MouseButtonPressed:
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    logger_->log("GameController","Mouse pressed");
                }
        }
    }
    
}

#endif