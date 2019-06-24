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

    void saveCampData();
    void loadCampData();

    GameState gameState_ = Uninitialized;
    Settings* settings_;
    View* view_;
    SoundManager* soundManager_;
    GameObjectManager* gameObjectManager_;

    // Screens
    SplashScreen* splashScreen_;
    MainMenu* mainMenu_;
    Options* options_;

    Logger* logger_;
    sf::Clock deltaTime_;
};

GameController::GameController(Logger* logger)
{
    // Later: load async
    logger_ = logger;
    settings_ = new Settings(logger_);
    view_ = new View(settings_, logger_);
    soundManager_ = new SoundManager(logger_, settings_);

    // Screens
    splashScreen_ = new SplashScreen(logger_);
    mainMenu_ = new MainMenu(logger_);
    options_ = new Options(logger_);
}

void GameController::start()
{
    if (gameState_ != Uninitialized)
        return;

    view_->openFrame();
    soundManager_->playSoundTrack();

    gameState_ = ShowSplash;

    while (!isExiting()) {
        gameLoop();
    }
    view_->closeFrame();
}

inline bool GameController::isExiting()
{
    return gameState_ == Exiting;
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
    //delete gameObjectManager_;
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
                case MainMenu::MenuResult::KeepPlaying: {
                    // settings load gamestate
                    gameObjectManager_ = new GameObjectManager(logger_, settings_, soundManager_);
                    loadCampData();
                    gameState_ = Playing;
                    logger_->info("MenuResult", "KeepPlaying");
                    view_->hideCursor();
                    break;
                }
                case MainMenu::MenuResult::StartGame: {
                    gameObjectManager_ = new GameObjectManager(logger_, settings_, soundManager_);
                    gameObjectManager_->createNewGame();
                    logger_->info("MenuResult", "StartGame");
                    view_->hideCursor();
                    gameState_ = Playing;
                    break;
                }
                case MainMenu::MenuResult::Options: {
                    logger_->info("MenuResult", "Options");
                    view_->showCursor();
                    gameState_ = ShowOptions;
                    break;
                }
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
            case sf::Event::Closed: {
                gameState_ = Exiting;
                delete gameObjectManager_;
                break;
            }
            case sf::Event::EventType::KeyPressed: {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    view_->showCursor();
                    settings_->saveSettingsToFile();
                    saveCampData();
                    delete gameObjectManager_;
                    settings_->keepPlaying = true;
                    gameState_ = ShowMenu;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
                    gameObjectManager_->selectInventorySlot(0);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
                    gameObjectManager_->selectInventorySlot(1);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
                    gameObjectManager_->selectInventorySlot(2);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
                    gameObjectManager_->selectInventorySlot(3);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
                    gameObjectManager_->selectInventorySlot(4);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
                    gameObjectManager_->selectInventorySlot(5);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) {
                    gameObjectManager_->selectInventorySlot(6);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)) {
                    gameObjectManager_->selectInventorySlot(7);
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
                    gameObjectManager_->handleClick(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y, false);
                }
                break;
            }
            case sf::Event::EventType::MouseButtonPressed: {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    gameObjectManager_->handleClick(currentEvent.mouseButton.x, currentEvent.mouseButton.y);
                }
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                    gameObjectManager_->handleClick(currentEvent.mouseButton.x, currentEvent.mouseButton.y, false);
                }
                break;
            }
            case sf::Event::EventType::MouseWheelMoved: {
                gameObjectManager_->selectInventorySlot(gameObjectManager_->getSelectedInventorySlot() + currentEvent.mouseWheel.delta);
                break;
            }
            default:
                break;
            }
            break;
        }
        default:
            break;
        }
    }
    // Do drawing here
    view_->clearFrame();

    sf::Vector2i mousePos = sf::Mouse::getPosition(view_->window);
    if (gameState_ == Playing)
        gameObjectManager_->updateMousePosition(mousePos.x, mousePos.y);

    switch (gameState_) {
    case ShowSplash:
        splashScreen_->show(view_->window);
        break;
    case ShowMenu:
        mainMenu_->updateKeepPlaying(settings_->keepPlaying);
        mainMenu_->show(view_->window);
        break;
    case ShowOptions:
        options_->show(view_->window);
        break;
    case Playing:
        // check these here because hold down
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

void GameController::loadCampData()
{
    std::vector<Settings::CampPart> parts = settings_->readCampData();
    for (auto part : parts) {
        GameObject::Type type = (GameObject::Type)part.enumType;
        switch (type) {
        case GameObject::Type::Player: {
            Player* player = new Player(logger_, settings_);
            player->setAnimation();
            player->setPosition(part.x, part.y);
            gameObjectManager_->addGameObject(player);
            break;
        }
        case GameObject::Type::Grass: {
            Grass* grass = new Grass(logger_, settings_);
            grass->setPosition(part.x, part.y);
            gameObjectManager_->addGameObject(grass);
            break;
        }
        case GameObject::Type::Fire: {
            Fire* fire = new Fire(logger_, settings_);
            fire->setPosition(part.x, part.y);
            fire->setLevel(part.level);
            fire->setAnimation();
            gameObjectManager_->addGameObject(fire);
            break;
        }
        case GameObject::Type::Stone: {
            Stone* stone = new Stone(logger_, settings_, part.x, part.y);
            stone->setAnimation();
            gameObjectManager_->addGameObject(stone);
            break;
        }
        case GameObject::Type::Tree: {
            Tree* tree = new Tree(logger_, settings_, part.x, part.y);
            tree->setAnimation();
            gameObjectManager_->addGameObject(tree);
            break;
        }
        default:
            break;
        }
    }
    gameObjectManager_->setNewIterators();
    std::vector<std::string> inv = settings_->readInventory();
    for (std::string& s : inv) {
        gameObjectManager_->addInventoryItem(s);
    }

    gameObjectManager_->orderGameObjects();
}

void GameController::saveCampData()
{
    std::vector<Settings::CampPart> parts;
    for (auto gameObject : gameObjectManager_->getObjectVector()) {
        Settings::CampPart current;
        current.enumType = (int)gameObject->type;
        current.x = gameObject->getPosition().x;
        current.y = gameObject->getPosition().y;
        current.level = gameObject->getLevel();
        parts.push_back(current);
    }

    std::vector<std::string> inv;
    for (auto item : gameObjectManager_->getInventoryVector()) {
        inv.push_back(item->getName());
    }
    settings_->saveCampData(parts);
    settings_->saveInventory(inv);
}

#endif