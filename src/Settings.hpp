#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "Logger.hpp"
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
    std::ifstream cFile("cevicamp.save");
    if (cFile.is_open()) {
        logger_->info("Settings","Save found");
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
            }
        }
    } else {
        // no save found, generate new one
        keepPlaying = false;
        logger_->error("Settings", "cevicamp.save could not be openend");
    }
    cFile.close();
}
void Settings::saveSettingsToFile()
{
    logger_->log("Settings", "saveSettingsToFile");
    std::ofstream out("cevicamp.save");
    out << "width=" << screenWidth << std::endl;
    out << "height=" << screenHeight << std::endl;
    out.close();
}

#endif