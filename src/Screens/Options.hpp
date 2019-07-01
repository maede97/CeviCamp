#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "Screen.hpp"

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

        background_.setSize(sf::Vector2f(800.0f, 100.0f));
        slider_.setSize(sf::Vector2f(800.0f, 100.0f));

        background_.setFillColor(sf::Color(100, 100, 100));
        slider_.setFillColor(sf::Color(200, 200, 200, 100)); // add some alpha

        background_.setScale(settings_->scalingFactor, settings_->scalingFactor);
        slider_.setScale(settings_->scalingFactor, settings_->scalingFactor);

        name_.setFont(settings_->font);
        value_.setFont(settings_->font);

        name_.setCharacterSize(100);
        value_.setCharacterSize(80);

        name_.setScale(settings_->scalingFactor, settings_->scalingFactor);
        value_.setScale(settings_->scalingFactor, settings_->scalingFactor);

        name_.setString(name);

        updateValue();

        // set positions
        spacing_ = 50 * settings_->scalingFactor; // spacing after text before slider

        name_.setPosition(x, y);
        int textHeight = 100 * settings_->scalingFactor;
        // value_ will be positioned each time it is drawn
        background_.setPosition(x, y + textHeight + spacing_);
        slider_.setPosition(x, y + textHeight + spacing_);
    }

    void updateValue()
    {
        // update slider based on current selected value
        value_.setString(std::to_string(currentValue_));
        double frac;
        if (currentValue_ == minValue_) {
            frac = 0;
        } else {
            frac = 1.0f / (maxValue_ - minValue_) * (currentValue_ - minValue_);
        }
        slider_.setScale(frac * settings_->scalingFactor, settings_->scalingFactor);
        *toChange_ = currentValue_;
    }

    void draw(sf::RenderWindow& window)
    {
        // change value position to be placed centered inside slider
        int textHeight = 100 * settings_->scalingFactor;
        value_.setPosition(x_ + background_.getLocalBounds().width * settings_->scalingFactor / 2 - value_.getLocalBounds().width * settings_->scalingFactor / 2,
            y_ + textHeight + spacing_ + background_.getLocalBounds().height * settings_->scalingFactor / 2 - 48 * settings_->scalingFactor);

        window.draw(background_);
        window.draw(slider_);
        window.draw(value_);
        window.draw(name_);
    }

    void checkClick(int x, int y)
    {
        if (background_.getGlobalBounds().contains(x, y)) {
            float length = background_.getLocalBounds().width * settings_->scalingFactor;
            float part = (x - background_.getPosition().x) / length;
            currentValue_ = minValue_ + part * (maxValue_ - minValue_);
            updateValue();
        }
    }

private:
    int minValue_;
    int maxValue_;
    int currentValue_;

    int x_;
    int y_;

    int* toChange_;

    sf::RectangleShape background_;
    sf::RectangleShape slider_;

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
    sf::Text back_;
    sf::Text keyMap_;
    sf::Text keyMapKeys_;
};

Options::Options(Logger* logger, Settings* settings)
    : Screen(logger, settings)
{
    sliders_.push_back(new Slider(logger, settings, L"MUSIKLAUTSTÄRKE", 200 * settings_->scalingFactor, 200 * settings_->scalingFactor, 0, 100, &settings_->musicVolume));
    sliders_.push_back(new Slider(logger, settings, L"EFFEKTLAUTSTÄRKE", 200 * settings_->scalingFactor, 500 * settings_->scalingFactor, 0, 100, &settings_->soundVolume));
    sliders_.push_back(new Slider(logger, settings, L"SPIELERGESCHWINDIGKEIT", 200 * settings_->scalingFactor, 800 * settings_->scalingFactor, 0, 100, &settings_->playerSpeed));
    sliders_.push_back(new Slider(logger, settings, L"KARTENGESCHWINDIGKEIT", 200 * settings_->scalingFactor, 1100 * settings_->scalingFactor, 0, 100, &settings_->movementSpeed));
    sliders_.push_back(new Slider(logger, settings, L"KARTENBREITE", 200 * settings_->scalingFactor, 1400 * settings_->scalingFactor, settings_->screenWidth, 10000, &settings_->mapWidth));
    sliders_.push_back(new Slider(logger, settings, L"KARTENHÖHE", 200 * settings_->scalingFactor, 1700 * settings_->scalingFactor, settings_->screenHeight, 10000, &settings_->mapHeight));

    keyMap_.setFont(settings_->font);
    keyMap_.setScale(settings_->scalingFactor, settings_->scalingFactor);
    keyMap_.setString(L"Tastaturbelegung\n\nVorwärts\nRückwärts\nLinks\nRechts\nBauen\nCheat-Menu");
    keyMap_.setCharacterSize(200 * settings_->scalingFactor);
    keyMap_.setPosition(settings_->screenWidth / 2 + 200*settings_->scalingFactor, 100 * settings_->scalingFactor);

    keyMapKeys_.setFont(settings_->font);
    keyMapKeys_.setScale(settings_->scalingFactor, settings_->scalingFactor);
    keyMapKeys_.setString(L"\n\nW\nS\nA\nD\nB\nM");
    keyMapKeys_.setCharacterSize(200 * settings_->scalingFactor);
    keyMapKeys_.setPosition(settings_->screenWidth / 2, 100 * settings_->scalingFactor);

    back_.setFont(settings_->font);
    back_.setScale(settings_->scalingFactor, settings_->scalingFactor);
    back_.setString(L"ZURÜCK");
    back_.setCharacterSize(200 * settings_->scalingFactor);
    back_.setStyle(sf::Text::Bold);
    back_.setPosition(settings_->screenWidth / 2 - back_.getLocalBounds().width * settings_->scalingFactor / 2, settings_->screenHeight - 100 - back_.getLocalBounds().height * settings_->scalingFactor);
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

    window.draw(keyMapKeys_);
    window.draw(keyMap_);
    window.draw(back_);
}

bool Options::handleClick(int x, int y)
{
    for (auto slider : sliders_) {
        slider->checkClick(x, y);
    }

    if (back_.getGlobalBounds().contains(x, y)) {
        return true;
    } else {
        return false;
    }
}

#endif