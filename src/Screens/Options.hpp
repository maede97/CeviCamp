#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "Screen.hpp"

class Slider {
public:
    Slider(Logger* logger, Settings* settings, const wchar_t* name, int x, int y, int minValue, int maxValue, int* toChange)
    {
        logger_ = logger;
        settings_ = settings;
        minValue_ = minValue;
        maxValue_ = maxValue;
        currentValue_ = *toChange;
        toChange_ = toChange;

        spacing_ *= settings_->scalingFactor;

        background_.setSize(sf::Vector2f(800.0f * settings_->scalingFactor, 100.0f * settings_->scalingFactor));
        slider_.setSize(sf::Vector2f(800.0f * settings_->scalingFactor, 100.0f * settings_->scalingFactor));

        background_.setFillColor(sf::Color(100, 100, 100));
        slider_.setFillColor(sf::Color(200, 200, 200, 100)); // add some alpha

        name_.setFont(settings_->font);
        value_.setFont(settings_->font);

        name_.setCharacterSize(100);
        value_.setCharacterSize(80);

        name_.setScale(settings_->scalingFactor, settings_->scalingFactor);
        value_.setScale(settings_->scalingFactor, settings_->scalingFactor);

        name_.setString(name);

        updateValue();

        // set positions
        name_.setPosition(x, y);
        int textHeight = name_.getLocalBounds().height * settings_->scalingFactor;
            value_.setPosition(x + background_.getLocalBounds().width * settings_->scalingFactor / 2 - value_.getLocalBounds().width * settings_->scalingFactor / 2,
        y + textHeight + spacing_ + background_.getLocalBounds().height * settings_->scalingFactor / 2 - value_.getLocalBounds().height * settings_->scalingFactor / 2);
        background_.setPosition(x, y + textHeight + spacing_);
        slider_.setPosition(x, y + textHeight + spacing_);
    }

    void updateValue()
    {
        value_.setString(std::to_string(currentValue_));
        double frac;
        if (currentValue_ == minValue_) {
            frac = 0;
        } else {
            frac = 1.0f / (maxValue_ - minValue_) * (currentValue_ - minValue_);
        }
        slider_.setScale(frac, 1);
        *toChange_ = currentValue_;
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(background_);
        window.draw(slider_);
        window.draw(value_);
        window.draw(name_);
    }

    void checkClick(int x, int y)
    {
        if (background_.getGlobalBounds().contains(x, y)) {
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

    int* toChange_;

    int spacing_ = 50;

    sf::RectangleShape background_;
    sf::RectangleShape slider_;

    sf::Text name_;
    sf::Text value_;

    Settings* settings_;
    Logger* logger_;
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
};

Options::Options(Logger* logger, Settings* settings)
    : Screen(logger, settings)
{
    sliders_.push_back(new Slider(logger, settings, L"MUSIKLAUTSTÄRKE", 200 * settings_->scalingFactor, 200 * settings_->scalingFactor, 0, 100, &settings_->musicVolume));
    sliders_.push_back(new Slider(logger, settings, L"SOUNDLAUTSTÄRKE", 200 * settings_->scalingFactor, 500 * settings_->scalingFactor, 0, 100, &settings_->soundVolume));
    sliders_.push_back(new Slider(logger, settings, L"SPIELERGESCHWINDIGKEIT", 200 * settings_->scalingFactor, 800 * settings_->scalingFactor, 0, 100, &settings_->playerSpeed));
    sliders_.push_back(new Slider(logger, settings, L"KARTENGESCHWINDIGKEIT", 200 * settings_->scalingFactor, 1100 * settings_->scalingFactor, 0, 100, &settings_->movementSpeed));
    sliders_.push_back(new Slider(logger, settings, L"KARTENBREITE", 200 * settings_->scalingFactor, 1400 * settings_->scalingFactor, settings_->screenWidth, 10000, &settings_->mapWidth));
    sliders_.push_back(new Slider(logger, settings, L"KARTENHÖHE", 200 * settings_->scalingFactor, 1700 * settings_->scalingFactor, settings_->screenHeight, 10000, &settings_->mapHeight));

    back_.setFont(settings_->font);
    back_.setScale(settings_->scalingFactor, settings_->scalingFactor);
    back_.setString(L"ZURÜCK");
    back_.setCharacterSize(200 * settings_->scalingFactor);
    back_.setStyle(sf::Text::Bold);
    back_.setPosition(settings_->screenWidth / 2 - back_.getLocalBounds().width*settings_->scalingFactor / 2, settings_->screenHeight - 100 - back_.getLocalBounds().height*settings_->scalingFactor);
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