#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include "Logger.hpp"
#include "Screens/MainMenu.hpp"
#include "Screens/Options.hpp"
#include "Screens/SplashScreen.hpp"
#include "View.hpp"

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
        ShowMenu,
        ShowSplash,
        ShowOptions };
    bool isExiting();
    void gameLoop();

    GameState gameState_ = Uninitialized;
    Settings* settings_;
    View* view_;

    // Screens
    SplashScreen* splashScreen_;
    MainMenu* mainMenu_;
    Options* options_;

    Logger* logger_;
};

GameController::GameController(Logger* logger)
{
    logger_ = logger;
    settings_ = new Settings(logger_);
    view_ = new View(settings_, logger_);

    // Screens
    splashScreen_ = new SplashScreen(logger_);
    mainMenu_ = new MainMenu(logger_, settings_->keepPlaying);
    options_ = new Options(logger_);
}

void GameController::start()
{
    if (gameState_ != Uninitialized)
        return;

    // read settings
    settings_->readSettingsFromFile();
    view_->openFrame();

    gameState_ = ShowSplash;

    while (!isExiting()) {
        gameLoop();
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
    delete splashScreen_;
    delete mainMenu_;
    delete view_;
    delete settings_;
    delete options_;
}

void GameController::gameLoop()
{
    sf::Clock dt;
    sf::Event currentEvent;
    while (view_->window.pollEvent(currentEvent)) {
        switch (gameState_) {
        case ShowSplash: {
            splashScreen_->show(view_->window);
            if (currentEvent.type == sf::Event::EventType::KeyPressed || currentEvent.type == sf::Event::EventType::MouseButtonPressed || currentEvent.type == sf::Event::EventType::Closed) {
                gameState_ = ShowMenu;
                view_->clearFrame();
                logger_->log("SplashScreen", "Quit Splash, show Menu");
            }
            break;
        }
        case ShowMenu: {
            mainMenu_->show(view_->window);
            if (currentEvent.type == sf::Event::EventType::Closed || currentEvent.type == sf::Event::EventType::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                gameState_ = Exiting;
                logger_->log("MainMenu", "exit game");
            }
            if (currentEvent.type == sf::Event::EventType::MouseButtonPressed) {
                MainMenu::MenuResult result = mainMenu_->handleClick(currentEvent.mouseButton.x, currentEvent.mouseButton.y);
                switch (result) {
                case MainMenu::MenuResult::Nothing:
                    break;
                case MainMenu::MenuResult::KeepPlaying:
                    // settings load gamestate
                    gameState_ = Playing;
                    logger_->log("MenuResult", "KeepPlaying");
                    break;
                case MainMenu::MenuResult::StartGame:
                    logger_->log("MenuResult", "StartGame");
                    gameState_ = Playing;
                    break;
                case MainMenu::MenuResult::Options:
                    logger_->log("MenuResult", "Options");
                    gameState_ = ShowOptions;
                    break;
                }
            }
            break;
        }
        case ShowOptions: {
            options_->show(view_->window);
            if (currentEvent.type == sf::Event::EventType::KeyPressed || currentEvent.type == sf::Event::EventType::MouseButtonPressed || currentEvent.type == sf::Event::EventType::Closed) {
                gameState_ = ShowMenu;
                view_->clearFrame();
                logger_->log("Options", "Quit Options, show Menu");
            }
            break;
        }
        case Playing: {
            view_->updateFrame();
            int milliSeconds = dt.restart().asMilliseconds();
            logger_->log("DT", std::to_string(milliSeconds));
            switch (currentEvent.type) {
            case sf::Event::Closed:
                gameState_ = Exiting;
                break;
            case sf::Event::EventType::KeyPressed:
                logger_->log("GameController", "Key pressed");
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    gameState_ = ShowMenu;
                }
                break;
            case sf::Event::EventType::MouseButtonPressed:
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    logger_->log("GameController", "Mouse pressed");
                }
            }
            break;
        }
        }
    }
}

#endif