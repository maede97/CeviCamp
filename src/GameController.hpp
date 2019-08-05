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
    // GameState internally
    enum GameState { Uninitialized,
        Paused,
        Playing,
        Exiting,
        ShowMenu,
        ShowSplash,
        ShowOptions };
    bool isExiting(); // checks if we should exit the game
    void gameLoop(); // do a gametick

    void saveCampData(); // save camp data (after game exit)
    void loadCampData(); // before keepplaying

    GameState gameState_ = Uninitialized;

    // create pointers to all classes we use
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
    // Later: load async?
    logger_ = logger;
    settings_ = new Settings(logger_);
    view_ = new View(settings_, logger_);
    soundManager_ = new SoundManager(logger_, settings_);

    // Screens
    splashScreen_ = new SplashScreen(logger_, settings_);
    mainMenu_ = new MainMenu(logger_, settings_);
    options_ = new Options(logger_, settings_);

    view_->gameView.setSize(sf::Vector2f(settings_->screenWidth, settings_->screenHeight));
}

void GameController::start()
{
    // setup game, and start gameloop
    if (gameState_ != Uninitialized)
        return;

    view_->openFrame();
    soundManager_->playSoundTrack();

    gameState_ = ShowSplash;

    view_->window.requestFocus();
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
    // poll all events, then do drawing
    // switch event handling based on current screen
    sf::Event currentEvent;
    while (view_->window.pollEvent(currentEvent)) {
        switch (gameState_) {
        case ShowSplash: {
            if (currentEvent.type == sf::Event::EventType::KeyPressed || currentEvent.type == sf::Event::EventType::MouseButtonPressed || currentEvent.type == sf::Event::EventType::Closed) {
                gameState_ = ShowMenu;
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
                    gameObjectManager_ = new GameObjectManager(logger_, settings_, soundManager_, view_);
                    loadCampData();
                    gameState_ = Playing;
                    logger_->info("MenuResult", "KeepPlaying");
                    break;
                }
                case MainMenu::MenuResult::StartGame: {
                    gameObjectManager_ = new GameObjectManager(logger_, settings_, soundManager_, view_);
                    gameObjectManager_->createNewGame();
                    logger_->info("MenuResult", "StartGame");
                    gameState_ = Playing;
                    break;
                }
                case MainMenu::MenuResult::Options: {
                    logger_->info("MenuResult", "Options");
                    gameState_ = ShowOptions;
                    break;
                }
                case MainMenu::MenuResult::Exiting: {
                    logger_->info("MenuResult", "Exiting");
                    gameState_ = Exiting;
                    break;
                }
                }
            }
            break;
        }
        case ShowOptions: {
            switch (currentEvent.type) {
            case sf::Event::Closed: {
                gameState_ = ShowMenu;
                logger_->info("Options", "Quit Options, show Menu");
                settings_->saveSettingsToFile();
                break;
            }
            case sf::Event::KeyPressed: {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    gameState_ = ShowMenu;
                    logger_->info("Options", "Quit Options, show Menu");
                    settings_->saveSettingsToFile();
                }
                break;
            }
            case sf::Event::EventType::MouseButtonPressed: {
                if (options_->handleClick(currentEvent.mouseButton.x, currentEvent.mouseButton.y)) {
                    gameState_ = ShowMenu;
                    logger_->info("Options", "Quit Options, show Menu");
                    settings_->saveSettingsToFile();
                }
                break;
            }
            case sf::Event::EventType::MouseMoved: {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    if (options_->handleClick(sf::Mouse::getPosition(view_->window).x, sf::Mouse::getPosition(view_->window).y)) {
                        gameState_ = ShowMenu;
                        logger_->info("Options", "Quit Options, show Menu");
                        settings_->saveSettingsToFile();
                    }
                }
            }
            default:
                break;
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
                // Do calulations only if window is in focus
                if (!view_->window.hasFocus()) {
                    break;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    if (gameObjectManager_->ifMenuCloseMenu()) {
                        break;
                    }
                    settings_->saveSettingsToFile();
                    saveCampData();
                    delete gameObjectManager_;
                    settings_->keepPlaying = true;
                    gameState_ = ShowMenu;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
                    sf::Vector2f worldPos = view_->window.mapPixelToCoords(sf::Mouse::getPosition(), view_->gameView);
                    gameObjectManager_->handleClick(worldPos.x, worldPos.y, false);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
                    gameObjectManager_->addCheatMenu();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                    view_->resetZoomLevel();
                    gameObjectManager_->moveCameraToPlayer();
                }
                break;
            }
            case sf::Event::EventType::MouseButtonPressed: {
                // Do calulations only if window is in focus
                if (!view_->window.hasFocus()) {
                    break;
                }
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    sf::Vector2f worldPos = view_->window.mapPixelToCoords(sf::Vector2i(currentEvent.mouseButton.x, currentEvent.mouseButton.y), view_->gameView);
                    gameObjectManager_->handleClick(worldPos.x, worldPos.y);
                }
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                    sf::Vector2f worldPos = view_->window.mapPixelToCoords(sf::Vector2i(currentEvent.mouseButton.x, currentEvent.mouseButton.y), view_->gameView);
                    gameObjectManager_->handleClick(worldPos.x, worldPos.y, false);
                }
                break;
            }
            case sf::Event::EventType::MouseWheelMoved: {
                // Do calulations only if window is in focus
                if (!view_->window.hasFocus()) {
                    break;
                }
                view_->gameView.zoom(0.5f + -(currentEvent.mouseWheel.delta - 1.0f) / 2.0f);
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

    // stop event handling, do "just" drawing here
    // also, switch here based on gamestate

    view_->clearFrame(); // clear frame for later drawing

    sf::Vector2f mousePos = view_->window.mapPixelToCoords(sf::Mouse::getPosition(view_->window), view_->gameView);
    if (gameState_ == Playing) {
        gameObjectManager_->updateMousePosition(mousePos.x, mousePos.y);
    }

    switch (gameState_) {
    case ShowSplash:
        //view_->resetViews();
        view_->window.setView(view_->originalView);
        splashScreen_->show(view_->window);
        break;
    case ShowMenu:
        //view_->resetViews();
        view_->window.setView(view_->originalView);
        mainMenu_->updateKeepPlaying(settings_->keepPlaying);
        mainMenu_->show(view_->window);
        break;
    case ShowOptions:
        //view_->resetViews();
        view_->window.setView(view_->originalView);
        soundManager_->setMusicVolume(); // do this here, because if options are open, maybe user changed music volume
        options_->show(view_->window);
        break;
    case Playing:
        // Do calulations only if window is in focus
        if (view_->window.hasFocus()) {
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
        }
        // check these here because hold down
        gameObjectManager_->drawAll(view_->window, deltaTime_.getElapsedTime());
        break;
    }

    view_->displayFrame();
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
        case GameObject::Type::Paloxe: {
            Paloxe* paloxe = new Paloxe(logger_, settings_, part.x, part.y);
            gameObjectManager_->addGameObject(paloxe);
            break;
        }
        case GameObject::Type::Trash: {
            Trash* trash = new Trash(logger_, settings_, part.x, part.y);
            gameObjectManager_->addGameObject(trash);
            break;
        }
        case GameObject::Type::Blache: {
            Blache* blache = new Blache(logger_, settings_, part.x, part.y);
            gameObjectManager_->addGameObject(blache);
            break;
        }
        case GameObject::Type::Sarasani: {
            Sarasani* sarasani = new Sarasani(logger_, settings_, part.x, part.y);
            gameObjectManager_->addGameObject(sarasani);
            break;
        }
        case GameObject::Type::Child: {
            Child* child = new Child(logger_, settings_, std::to_string(part.level));
            child->setPosition(part.x, part.y);
            child->setLevel(part.level);
            gameObjectManager_->addGameObject(child);
            break;
        }
        default:
            break;
        }
    }
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
        for (unsigned int i = 0; i < item->getAmount(); i++)
            inv.push_back(item->getName());
    }
    settings_->saveCampData(parts);
    settings_->saveInventory(inv);
}

#endif