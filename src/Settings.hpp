#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "Logger.hpp"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <random>
#include <vector>

#ifndef WIN32
    #include <curlpp/cURLpp.hpp>
    #include <curlpp/Options.hpp>
#endif

class Settings {
public:
    std::string VERSION = std::string("1.0.3-alpha");
    struct CampPart {
        int enumType;
        int x;
        int y;
        int level;
        std::string varia;
    };
    Settings(Logger* logger);

    void readSettingsFromFile();
    void saveSettingsToFile();

    std::vector<CampPart> readCampData();
    void saveCampData(std::vector<CampPart>& parts);

    std::vector<std::string> readInventory();
    void saveInventory(std::vector<std::string>& inv);

    float getGUIFactor() const;

    void addMessage(std::wstring message);

    const char* title = "CeviCamp";
    int screenWidth = 1920;
    int screenHeight = 1080;
    bool keepPlaying = false;
    int musicVolume = 100;
    int soundVolume = 100;
    sf::Font font;

    int playerSpeed = 15;
    int movementSpeed = 10;

    // Random Number Generator
    std::mt19937 rng;
    int seed = 42;

    int guiSize = 100; // percent

    // map size
    int mapWidth = 5000;
    int mapHeight = 5000;

    bool showMiniMap = true;

    bool showTutorial = true;

    bool checkForUpdates = true;

    std::deque<std::wstring> messages;

private:
    Logger* logger_;
    std::random_device r_dev_;
};

Settings::Settings(Logger* logger)
{
    logger_ = logger;
    if (!font.loadFromFile("res/CeviBold.ttf")) {
        logger_->error("Settings", "Could not open res/CeviBold.ttf");
        return;
    }
    readSettingsFromFile();
    rng = std::mt19937(r_dev_());
    //rng.seed(seed);

    if(checkForUpdates) {
        #ifdef WIN32
            addMessage(L"Automatische Updates funktionieren momentan nicht auf Windows!");
        #else
            curlpp::Cleanup myCleanup;

            std::ostringstream os;
            os << curlpp::options::Url(std::string("https://raw.githubusercontent.com/maede97/CeviCamp/master/version"));

            std::string versions = os.str();
            size_t position = versions.find(VERSION);
            if(position > 0) {
                addMessage(L"Deine Version ist veraltet.");
            } else if(position == std::string::npos){
                addMessage(L"Deine Version wurde nicht gefunden.");
            } else {
                addMessage(L"Deine Version ist aktuell.");
            }
        #endif
    }
}

void Settings::addMessage(std::wstring message)
{
    messages.push_back(message);
    if (messages.size() > 10) {
        messages.pop_front();
    }
}

float Settings::getGUIFactor() const
{
    return guiSize / 100.f;
}

void Settings::readSettingsFromFile()
{
    std::ifstream gameSave("cevicamp_parts.save");
    if (gameSave.is_open()) {
        keepPlaying = true; // save found, show keepPlaying
    } else {
        keepPlaying = false;
    }
    std::ifstream cFile("cevicamp.save");
    if (cFile.is_open()) {
        logger_->info("Settings", "Save found");
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
            } else if (name == "map-width") {
                mapWidth = std::stoi(value);
            } else if (name == "map-height") {
                mapHeight = std::stoi(value);
            } else if (name == "seed") {
                seed = std::stoi(value);
            } else if (name == "player-speed") {
                playerSpeed = std::stoi(value);
            } else if (name == "movement-speed") {
                movementSpeed = std::stoi(value);
            } else if (name == "gui-size") {
                guiSize = std::stoi(value);
            } else if (name == "show-minimap") {
                showMiniMap = std::stoi(value);
            } else if (name == "show-tutorial") {
                showTutorial = std::stoi(value);
            } else if(name == "check-updates") {
                checkForUpdates = std::stoi(value);
            }
        }
    } else {
        logger_->warning("Settings", "cevicamp.save could not be openend");
    }
}
void Settings::saveSettingsToFile()
{
    std::ofstream out("cevicamp.save");
    out << "width=" << screenWidth << std::endl;
    out << "height=" << screenHeight << std::endl;
    out << "music-volume=" << musicVolume << std::endl;
    out << "sound-volume=" << soundVolume << std::endl;
    out << "map-width=" << mapWidth << std::endl;
    out << "map-height=" << mapHeight << std::endl;
    out << "seed=" << seed << std::endl;
    out << "player-speed=" << playerSpeed << std::endl;
    out << "movement-speed=" << movementSpeed << std::endl;
    out << "gui-size=" << guiSize << std::endl;
    out << "show-minimap=" << showMiniMap << std::endl;
    out << "show-tutorial=" << showTutorial << std::endl;
    out << "check-updates=" << checkForUpdates << std::endl;
    out.close();
}

std::vector<Settings::CampPart> Settings::readCampData()
{
    std::vector<CampPart> parts;
    std::ifstream cFile("cevicamp_parts.save");
    if (cFile.is_open()) {
        std::string line;
        while (getline(cFile, line)) {
            if (line[0] == '#' || line.empty())
                continue;
            auto delimiterPos = line.find("=");
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);

            auto delim1 = value.find("/");
            auto delim2 = value.find("/", delim1 + 1);
            auto delim3 = value.find("/", delim2 + 1);

            CampPart current;
            current.enumType = std::stoi(name);
            current.x = std::stoi(value.substr(0, delim1));
            current.y = std::stoi(value.substr(delim1 + 1, delim2));
            current.level = std::stoi(value.substr(delim2 + 1, delim3));
            current.varia = value.substr(delim3 + 1);
            parts.push_back(current);
        }
    } else {
        logger_->error("Settings", "cevicamp_parts.save could not be openend");
    }
    return parts;
}

std::vector<std::string> Settings::readInventory()
{
    std::vector<std::string> inv;
    std::ifstream cFile("cevicamp_inventory.save");
    if (cFile.is_open()) {
        std::string line;
        while (getline(cFile, line)) {
            if (line[0] == '#' || line.empty())
                continue;
            inv.push_back(line);
        }
    } else {
        logger_->error("Settings", "cevicamp_inventory.save could not be openend");
    }
    return inv;
}
void Settings::saveInventory(std::vector<std::string>& inv)
{
    logger_->info("Settings", "Saving inventory");
    std::ofstream out("cevicamp_inventory.save");
    for (auto s : inv) {
        out << s << std::endl;
    }
    out.close();
}

void Settings::saveCampData(std::vector<Settings::CampPart>& parts)
{
    logger_->info("Settings", "Saving camp data");
    std::ofstream out("cevicamp_parts.save");
    for (auto campPart : parts) {
        out << campPart.enumType << "=" << campPart.x << "/" << campPart.y << "/" << campPart.level << "/" << campPart.varia << std::endl;
    }
    out.close();
}

#endif