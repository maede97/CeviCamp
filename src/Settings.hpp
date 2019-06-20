#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "Logger.hpp"

class Settings {
public:
    Settings(Logger* logger);
    void readSettingsFromFile();
    void saveSettingsToFile();

    const char* title = "CeviCamp";
    const unsigned int screenWidth = 1920;
    const unsigned int screenHeight = 1080;
    bool keepPlaying = false;

private:
    Logger* logger_;
};

Settings::Settings(Logger* logger)
{
    logger_ = logger;
}

void Settings::readSettingsFromFile()
{
    logger_->log("Settings", "readSettingsFromFile");
}
void Settings::saveSettingsToFile()
{
    logger_->log("Settings", "saveSettingsToFile");
}

#endif