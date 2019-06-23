#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "Logger.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <fstream>

class Settings {
public:
    Settings(Logger* logger);
    void readSettingsFromFile();
    void saveSettingsToFile();

    const char* title = "CeviCamp";
    unsigned int screenWidth = 1920;
    unsigned int screenHeight = 1080;
    bool keepPlaying = false;
    int musicVolume = 100;
    int soundVolume = 100;
    sf::Font font;

private:
    Logger* logger_;
};

Settings::Settings(Logger* logger)
{
    logger_ = logger;
    readSettingsFromFile();
    if (!font.loadFromFile("res/FreeSans.ttf")) {
        logger_->error("Settings", "Could not open res/FreeSans.ttf");
        return;
    }
}

void Settings::readSettingsFromFile()
{
    logger_->log("Settings", "readSettingsFromFile");
    std::ifstream cFile("cevicamp.save");
    if (cFile.is_open()) {
        logger_->info("Settings", "Save found");
        keepPlaying = true; // save found, show keepPlaying
        std::string line;
        while (getline(cFile, line)) {
            if (line[0] == '#' || line.empty())
                continue;
            auto delimiterPos = line.find("=");
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);
            if (name == "width") {
                screenWidth = std::stoi(value);
            } else if (name == "height") {
                screenHeight = std::stoi(value);
            } else if (name == "music-volume") {
                musicVolume = std::stoi(value);
            } else if (name == "sound-volume") {
                soundVolume = std::stoi(value);
            }
        }
    } else {
        // no save found, generate new one
        keepPlaying = false;
        logger_->error("Settings", "cevicamp.save could not be openend");
    }
}
void Settings::saveSettingsToFile()
{
    logger_->log("Settings", "saveSettingsToFile");
    std::ofstream out("cevicamp.save");
    out << "width=" << screenWidth << std::endl;
    out << "height=" << screenHeight << std::endl;
    out << "music-volume=" << musicVolume << std::endl;
    out << "sound-volume=" << soundVolume << std::endl;
    out.close();
}

#endif