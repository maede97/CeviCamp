#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "Logger.hpp"

class Settings {
public:
    Settings(Logger* logger);
    void readSettingsFromFile();
    void saveSettingsToFile();

    const char* title = "CeviCamp";
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
    logger_->log("Settings","readSettingsFromFile");
}
void Settings::saveSettingsToFile()
{
    logger_->log("Settings","saveSettingsToFile");
}

#endif