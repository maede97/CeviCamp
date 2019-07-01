#ifndef CLICKABLE_MESSAGE_BOX_HPP
#define CLICKABLE_MESSAGE_BOX_HPP

#include "../Logger.hpp"
#include "../Settings.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

// TODO SCALING

class Button {
public:
    Button() {}
    Button(Logger* logger, Settings* settings, int x, int y, int buttonSizeX, int buttonSizeY, std::string image)
    {
        logger_ = logger;
        settings_ = settings;

        if (!image_.loadFromFile("res/CampParts/" + image + ".png")) {
            logger_->error("MenuButton", "res/CampParts/" + image + ".png could not be opened");
            return;
        }

        background_ = sf::Sprite(image_);
        background_.setPosition(x, y);

        boundary_.setOutlineColor(sf::Color::Black);
        boundary_.setOutlineThickness(-8);
        boundary_.setPosition(x, y);
        boundary_.setSize(sf::Vector2f(buttonSizeX, buttonSizeY));
    }
    bool checkClick(int x, int y)
    {
        return background_.getGlobalBounds().contains(x, y);
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(boundary_);
        window.draw(background_);
    }

private:
    Settings* settings_;
    Logger* logger_;

    sf::Texture image_;
    sf::Sprite background_;

    sf::RectangleShape boundary_;
};

class ClickableMessageBox {
public:
    ClickableMessageBox(Logger* logger, Settings* settings, const wchar_t* title, const wchar_t* question)
    {
        logger_ = logger;
        settings_ = settings;

        background_.setPosition(settings->screenWidth / 4, settings->screenHeight / 4);
        background_.setSize(sf::Vector2f(settings_->screenWidth / 2, settings_->screenHeight / 2));
        background_.setFillColor(sf::Color::White);

        background_.setOutlineColor(sf::Color::Black);
        background_.setOutlineThickness(8);

        x_ = settings->screenWidth / 4;
        y_ = settings->screenHeight / 4;

        title_.setCharacterSize(80 * settings_->scalingFactor);
        title_.setFont(settings_->font);
        title_.setString(title);
        title_.setFillColor(sf::Color::Black);
        title_.setPosition(x_ + background_.getSize().x / 2 - title_.getLocalBounds().width / 2, y_ + 20 * settings_->scalingFactor);

        question_.setFont(settings_->font);
        question_.setCharacterSize(60 * settings_->scalingFactor);
        question_.setString(question);
        question_.setFillColor(sf::Color::Black);
        question_.setPosition(x_ + background_.getSize().x / 2 - question_.getLocalBounds().width / 2, y_ + title_.getLocalBounds().height + 40*settings_->scalingFactor);

        currX_ = x_ + 20 * settings_->scalingFactor; // initial offset
        currY_ = y_ + title_.getLocalBounds().height + question_.getLocalBounds().height + 80 * settings_->scalingFactor;
    }

    ~ClickableMessageBox()
    {
        for (auto pair : buttons_) {
            delete pair.first;
        }
    }

    void addNewButtonRow()
    {
        currY_ += 40 * settings_->scalingFactor + buttonSizeY_;
        currX_ = x_ + 20 * settings_->scalingFactor;
    }

    // returns index of button
    void addButton(std::string image, std::function<void()> callback)
    {
        int amount = buttons_.size();
        // offset:
        // x: spacing + buttons
        // y: title + text
        Button* button = new Button(logger_, settings_, currX_, currY_, buttonSizeX_, buttonSizeY_, image);
        buttons_.push_back(std::make_pair(button, callback));
        currX_ += 40 * settings_->scalingFactor + buttonSizeX_;
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(background_);
        window.draw(title_);
        window.draw(question_);
        for (auto pair : buttons_) {
            pair.first->draw(window);
        }
    }

    bool checkAndHandleClick(int x, int y)
    {
        for (auto pair : buttons_) {
            if (pair.first->checkClick(x, y)) {
                pair.second();
                return true;
            }
        }
        return false;
    }

private:
    int x_, y_, currX_, currY_;
    int buttonSizeX_ = 112;
    int buttonSizeY_ = 112;
    Logger* logger_;
    Settings* settings_;
    sf::Text title_;
    sf::Text question_;
    sf::RectangleShape background_;
    std::vector<std::pair<Button*, std::function<void()>>> buttons_;
};

#endif