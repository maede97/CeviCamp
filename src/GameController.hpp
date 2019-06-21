#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include "GameObjectManager.hpp"
#include "Logger.hpp"
#include "Screens/MainMenu.hpp"
#include "Screens/Options.hpp"
#include "Screens/SplashScreen.hpp"
#include "SoundManager.hpp"
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
    SoundManager* soundManager_;
    GameObjectManager* gameObjectManager_;

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
    // Later: load async
    logger_ = logger;
    settings_ = new Settings(logger_);
    view_ = new View(settings_, logger_);
    soundManager_ = new SoundManager(logger_);
    gameObjectManager_ = new GameObjectManager(logger_, settings_);

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
    soundManager_->playSoundTrack();

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
    delete soundManager_;
    delete gameObjectManager_;
}

void GameController::gameLoop()
{
    sf::Event currentEvent;
    while (view_->window.pollEvent(currentEvent)) {
        switch (gameState_) {
        case ShowSplash: {
            if (currentEvent.type == sf::Event::EventType::KeyPressed || currentEvent.type == sf::Event::EventType::MouseButtonPressed || currentEvent.type == sf::Event::EventType::Closed) {
                gameState_ = ShowMenu;
                view_->showCursor();
                logger_->info("SplashScreen", "Quit Splash, show Menu");
            }
            break;
        }
        case ShowMenu: {
            if (currentEvent.type == sf::Event::EventType::Closed || currentEvent.type == sf::Event::EventType::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                gameState_ = Exiting;
                logger_->info("MainMenu", "exit game");
            }
            if (currentEvent.type == sf::Event::EventType::MouseButtonPressed) {
                MainMenu::MenuResult result = mainMenu_->handleClick(currentEvent.mouseButton.x, currentEvent.mouseButton.y);
                switch (result) {
                case MainMenu::MenuResult::Nothing:
                    break;
                case MainMenu::MenuResult::KeepPlaying:
                    // settings load gamestate
                    gameState_ = Playing;
                    logger_->info("MenuResult", "KeepPlaying");
                    view_->hideCursor();
                    break;
                case MainMenu::MenuResult::StartGame:
                    logger_->info("MenuResult", "StartGame");
                    view_->hideCursor();
                    gameState_ = Playing;
                    break;
                case MainMenu::MenuResult::Options:
                    logger_->info("MenuResult", "Options");
                    view_->showCursor();
                    gameState_ = ShowOptions;
                    break;
                }
            }
            break;
        }
        case ShowOptions: {
            if (currentEvent.type == sf::Event::EventType::KeyPressed || currentEvent.type == sf::Event::EventType::MouseButtonPressed || currentEvent.type == sf::Event::EventType::Closed) {
                gameState_ = ShowMenu;
                view_->showCursor();
                logger_->info("Options", "Quit Options, show Menu");
            }
            break;
        }
        case Playing: {
            switch (currentEvent.type) {
            case sf::Event::Closed:
                gameState_ = Exiting;
                break;
            case sf::Event::EventType::KeyPressed:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    view_->showCursor();
                    gameState_ = ShowMenu;
                }
                break;
            case sf::Event::EventType::MouseButtonPressed:
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    gameObjectManager_->handleClick(currentEvent.mouseButton.x, currentEvent.mouseButton.y);
                }
            }
            break;
        }
        }
    }
    // Do drawing here
    view_->clearFrame();

    sf::Vector2i mousePos = sf::Mouse::getPosition(view_->window);
    gameObjectManager_->updateMousePosition(mousePos.x, mousePos.y);

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            gameObjectManager_->down();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            gameObjectManager_->up();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            gameObjectManager_->left();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            gameObjectManager_->right();
        }
        gameObjectManager_->drawAll(view_->window, deltaTime_.getElapsedTime());
        break;
    }

    view_->window.display();
    deltaTime_.restart();
}

#endif