#ifndef MESSAGEBOX_HPP
#define MESSAGEBOX_HPP

#include "../Settings.hpp"
#include <SFML/Graphics.hpp>

class MessageBox {
public:
    MessageBox(Logger* logger, Settings* settings)
    {
        logger_ = logger;
        settings_ = settings;

        text_.setCharacterSize(24 * settings_->getGUIFactor());
        text_.setFont(settings_->font);
        text_.setFillColor(sf::Color::White);
    }

    void setPosition(int x, int y) {
        text_.setPosition(x,y);
    }

    void draw(sf::RenderWindow& window)
    {
        std::wstring message;
        for(auto entry : settings_->messages) {
            message += entry+L"\n";
        }
        text_.setString(message);
        window.draw(text_);
    }

private:
    Logger* logger_;
    Settings* settings_;
    sf::Text text_;
};

#endif /* MESSAGEBOX_HPP */