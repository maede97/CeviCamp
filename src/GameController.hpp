#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include "GameObjects/Fire.hpp"
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

    // GameObjects (TEMP)
    Fire* fire_;

    Logger* logger_;
    sf::Clock deltaTime_;
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

    // GameObjects
    fire_ = new Fire(logger_);
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
    // Screens
    delete splashScreen_;
    delete mainMenu_;
    delete options_;

    delete view_;
    delete settings_;

    // GameObjects
    delete fire_;
}

void GameController::gameLoop()
{
    sf::Event currentEvent;
    while (view_->window.pollEvent(currentEvent)) {
        switch (gameState_) {
        case ShowSplash: {
            if (currentEvent.type == sf::Event::EventType::KeyPressed || currentEvent.type == sf::Event::EventType::MouseButtonPressed || currentEvent.type == sf::Event::EventType::Closed) {
                gameState_ = ShowMenu;
                logger_->log("SplashScreen", "Quit Splash, show Menu");
            }
            break;
        }
        case ShowMenu: {
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
            if (currentEvent.type == sf::Event::EventType::KeyPressed || currentEvent.type == sf::Event::EventType::MouseButtonPressed || currentEvent.type == sf::Event::EventType::Closed) {
                gameState_ = ShowMenu;
                logger_->log("Options", "Quit Options, show Menu");
            }
            break;
        }
        case Playing: {
            switch (currentEvent.type) {
            case sf::Event::Closed:
                gameState_ = Exiting;
                break;
            case sf::Event::EventType::KeyPressed:
                logger_->log("Playing", "Key pressed");
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    gameState_ = ShowMenu;
                }
                break;
            case sf::Event::EventType::MouseButtonPressed:
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    logger_->log("Playing", "Mouse pressed");
                    fire_->upgrade();
                }
            }
            break;
        }
        }
    }
    // Do drawing here
    view_->clearFrame();

    switch (gameState_) {
    case ShowSplash:
        splashScreen_->show(view_->window);
        break;
    case ShowMenu:
        mainMenu_->show(view_->window);
        break;
    case ShowOptions:
        options_->show(view_->window);
        break;
    case Playing:
        // TEMP: needs to be done using a GameObjectManager
        fire_->play();
        fire_->update(deltaTime_.getElapsedTime());
        //logger_->log("Time",std::to_string(deltaTime_.getElapsedTime().asMilliseconds()));
        view_->window.draw(fire_->getSprite());
        break;
    }

    view_->window.display();
    deltaTime_.restart();
}

#endif