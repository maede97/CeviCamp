#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "Screen.hpp"

class CheckBox {
public:
    CheckBox(Logger* logger, Settings* settings, const wchar_t* name, int x, int y, bool checked, bool* toChange)
    {
        logger_ = logger;
        settings_ = settings;
        currentValue_ = *toChange;
        toChange_ = toChange;

        x_ = x;
        y_ = y;

        background_.setSize(sf::Vector2f(50.0f * settings_->getGUIFactor(), 50.0f * settings_->getGUIFactor()));
        background_.setOutlineThickness(2.f);
        background_.setFillColor(sf::Color::Black);
        background_.setOutlineColor(sf::Color(100, 100, 100));

        value_.setSize(sf::Vector2f(50.0f * settings_->getGUIFactor(), 50.0f * settings_->getGUIFactor()));

        name_.setFont(settings_->font);

        name_.setCharacterSize(36 * settings_->getGUIFactor());

        name_.setString(name);

        updateValue();

        background_.setPosition(x, y);
        value_.setPosition(x, y);

        name_.setPosition(x + background_.getGlobalBounds().width + 10 * settings_->getGUIFactor(),
            y + background_.getGlobalBounds().height / 2 - name_.getGlobalBounds().height / 2);
    }

    void updateValue()
    {
        if (currentValue_) {
            value_.setFillColor(sf::Color(200, 200, 200));
        } else {
            value_.setFillColor(sf::Color::Black);
        }
        *toChange_ = currentValue_;
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(background_);
        window.draw(name_);
        window.draw(value_);
    }
    void checkClick(int x, int y)
    {
        if (background_.getGlobalBounds().contains(x, y)) {
            currentValue_ = !currentValue_;
            updateValue();
        }
    }

private:
    Logger* logger_;
    Settings* settings_;

    bool currentValue_;
    bool* toChange_;

    int x_;
    int y_;

    sf::RectangleShape background_;

    sf::Text name_;
    sf::RectangleShape value_;

    int spacing_;
};

class Slider {
public:
    // Class to show a slider, using multiple sf elements
    Slider(Logger* logger, Settings* settings, const wchar_t* name, int x, int y, int minValue, int maxValue, int* toChange)
    {
        logger_ = logger;
        settings_ = settings;
        minValue_ = minValue;
        maxValue_ = maxValue;
        currentValue_ = *toChange;
        toChange_ = toChange;

        x_ = x;
        y_ = y;

        background_.setSize(sf::Vector2f(400.0f * settings_->getGUIFactor(), 50.0f * settings_->getGUIFactor()));
        slider_.setSize(sf::Vector2f(400.0f * settings_->getGUIFactor(), 50.0f * settings_->getGUIFactor()));

        background_.setFillColor(sf::Color(100, 100, 100));
        slider_.setFillColor(sf::Color(200, 200, 200, 100)); // add some alpha

        boxLeftBackground_.setSize(sf::Vector2f(background_.getGlobalBounds().height, background_.getGlobalBounds().height));
        boxRightBackground_.setSize(sf::Vector2f(background_.getGlobalBounds().height, background_.getGlobalBounds().height));

        name_.setFont(settings_->font);
        value_.setFont(settings_->font);

        name_.setCharacterSize(36 * settings_->getGUIFactor());
        value_.setCharacterSize(32 * settings_->getGUIFactor());

        name_.setString(name);

        updateValue();

        // set positions
        spacing_ = 10 * settings_->getGUIFactor(); // spacing after text before slider
        textHeight_ = 50 * settings_->getGUIFactor();

        name_.setPosition(x, y);

        // value_ will be positioned each time it is drawn
        boxLeftBackground_.setPosition(x - boxLeftBackground_.getGlobalBounds().width, y + textHeight_ + spacing_);
        boxRightBackground_.setPosition(x + background_.getGlobalBounds().width, y + textHeight_ + spacing_);
        background_.setPosition(x, y + textHeight_ + spacing_);
        slider_.setPosition(x, y + textHeight_ + spacing_);
    }

    void updateValue()
    {
        // update slider based on current selected value
        value_.setString(std::to_string(currentValue_));
        float frac;
        if (currentValue_ == minValue_) {
            frac = 0;
        } else {
            frac = 1.0f / (maxValue_ - minValue_) * (currentValue_ - minValue_);
        }
        slider_.setScale(frac, 1.f);
        *toChange_ = currentValue_;
    }

    void draw(sf::RenderWindow& window)
    {
        // change value position to be placed centered inside slider
        value_.setPosition(x_ + background_.getGlobalBounds().width / 2 - value_.getGlobalBounds().width / 2,
            y_ + textHeight_ + spacing_ + background_.getGlobalBounds().height / 2 - value_.getGlobalBounds().height / 2);

        window.draw(background_);
        window.draw(boxLeftBackground_);
        window.draw(boxRightBackground_);
        window.draw(slider_);
        window.draw(value_);
        window.draw(name_);
    }

    void checkClick(int x, int y)
    {
        if (boxLeftBackground_.getGlobalBounds().contains(x, y)) {
            currentValue_ = std::max(minValue_, currentValue_ - 1);
            updateValue();
        } else if (boxRightBackground_.getGlobalBounds().contains(x, y)) {
            currentValue_ = std::min(maxValue_, currentValue_ + 1);
            updateValue();
        } else if (background_.getGlobalBounds().contains(x, y)) {
            float length = background_.getLocalBounds().width;
            float part = (x - background_.getPosition().x) / length;
            currentValue_ = minValue_ + part * (maxValue_ - minValue_);
            updateValue();
        }
    }

private:
    int minValue_;
    int maxValue_;
    int currentValue_;

    int textHeight_;

    int x_;
    int y_;

    int* toChange_;

    sf::RectangleShape background_;
    sf::RectangleShape slider_;

    sf::RectangleShape boxLeftBackground_;
    sf::RectangleShape boxRightBackground_;

    sf::Text name_;
    sf::Text value_;

    Settings* settings_;
    Logger* logger_;

    int spacing_;
};

class Options : public Screen {
public:
    Options(Logger* logger, Settings* settings);
    ~Options();
    void show(sf::RenderWindow& window);
    bool handleClick(int x, int y);

private:
    std::vector<Slider*> sliders_;
    std::vector<CheckBox*> checkboxes_;
    sf::Text back_;
    sf::Text keyMap_;
    sf::Text keyMapKeys_;
};

Options::Options(Logger* logger, Settings* settings)
    : Screen(logger, settings)
{
    int perSlider = (settings_->screenHeight - 200) / 7;
    sliders_.push_back(new Slider(logger, settings, L"MUSIKLAUTSTÄRKE", 100, 100, 0, 100, &settings_->musicVolume));
    sliders_.push_back(new Slider(logger, settings, L"EFFEKTLAUTSTÄRKE", 100, 100 + perSlider * 1, 0, 100, &settings_->soundVolume));
    sliders_.push_back(new Slider(logger, settings, L"SPIELERGESCHWINDIGKEIT", 100, 100 + perSlider * 2, 0, 100, &settings_->playerSpeed));
    sliders_.push_back(new Slider(logger, settings, L"KARTENGESCHWINDIGKEIT", 100, 100 + perSlider * 3, 0, 100, &settings_->movementSpeed));
    sliders_.push_back(new Slider(logger, settings, L"KARTENBREITE", 100, 100 + perSlider * 4, settings_->screenWidth, 10000, &settings_->mapWidth));
    sliders_.push_back(new Slider(logger, settings, L"KARTENHÖHE", 100, 100 + perSlider * 5, settings_->screenHeight, 10000, &settings_->mapHeight));
    sliders_.push_back(new Slider(logger, settings, L"GUI-GRÖSSE (BRAUCHT NEUSTART)", 100, 100 + perSlider * 6, 1, 301, &settings_->guiSize));

    int perCheckBox = (settings_->screenHeight / 2) / 1;
    checkboxes_.push_back(new CheckBox(logger, settings, L"MINIMAP ANZEIGEN", settings_->screenWidth / 2, settings_->screenHeight / 2, settings_->showMiniMap, &settings_->showMiniMap));

    keyMap_.setFont(settings_->font);
    keyMap_.setString(L"Tastaturbelegung\n\nVorwärts\nRückwärts\nLinks\nRechts\nBauen\nCheat-Menu\nZoom zurücksetzen");
    keyMap_.setCharacterSize(36 * settings_->getGUIFactor());
    keyMap_.setPosition(settings_->screenWidth / 2 + 100, 50);

    keyMapKeys_.setFont(settings_->font);
    keyMapKeys_.setString(L"\n\nW\nS\nA\nD\nB\nM\nR");
    keyMapKeys_.setCharacterSize(36 * settings_->getGUIFactor());
    keyMapKeys_.setPosition(settings_->screenWidth / 2, 50);

    back_.setFont(settings_->font);
    back_.setString(L"ZURÜCK");
    back_.setCharacterSize(48 * settings_->getGUIFactor());
    back_.setStyle(sf::Text::Bold);
    back_.setPosition(settings_->screenWidth / 2 - back_.getLocalBounds().width / 2, settings_->screenHeight - 100 - back_.getLocalBounds().height);
}

Options::~Options()
{
    for (auto slider : sliders_) {
        delete slider;
    }
}

void Options::show(sf::RenderWindow& window)
{
    for (auto slider : sliders_) {
        slider->draw(window);
    }
    for (auto checkbox : checkboxes_) {
        checkbox->draw(window);
    }

    window.draw(keyMapKeys_);
    window.draw(keyMap_);
    window.draw(back_);
}

bool Options::handleClick(int x, int y)
{
    for (auto slider : sliders_) {
        slider->checkClick(x, y);
    }
    for (auto checkbox : checkboxes_) {
        checkbox->checkClick(x, y);
    }

    if (back_.getGlobalBounds().contains(x, y)) {
        return true;
    } else {
        return false;
    }
}

#endif