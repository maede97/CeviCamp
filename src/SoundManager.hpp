#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include "Logger.hpp"
#include "Settings.hpp"
#include <SFML/Audio.hpp>
#include <vector>

class SoundManager {
public:
    SoundManager(Logger* logger, Settings* settings)
    {
        logger_ = logger;
        settings_ = settings;
        if (!soundtrack_.openFromFile("res/Sounds/Soundtrack.wav")) {
            logger_->error("SoundManager", "Soundtrack.wav could not be loaded");
            return;
        }
        soundtrack_.setLoop(true);
        soundtrack_.setVolume(settings_->musicVolume);
    }
    ~SoundManager()
    {
        for (auto buffer : buffers_) {
            delete buffer;
        }
        for (auto sound : sounds_) {
            delete sound;
        }
    }

    void playSound(std::string soundName)
    {
        sf::SoundBuffer* buffer = new sf::SoundBuffer();
        if (!buffer->loadFromFile("res/Sounds/" + soundName + ".wav")) {
            logger_->error("SoundManager", "res/Sounds/" + soundName + ".wav could not be loaded");
            return;
        }
        sf::Sound* sound = new sf::Sound();
        sound->setBuffer(*buffer);
        sound->setVolume(settings_->soundVolume);
        sound->play();
        buffers_.push_back(buffer);
        sounds_.push_back(sound);
    }

    void playSoundTrack()
    {
        soundtrack_.play();
    }
    void stopSoundTrack()
    {
        soundtrack_.stop();
    }

    void setMusicVolume() {
        soundtrack_.setVolume(settings_->musicVolume);
    }

private:
    Logger* logger_;
    Settings* settings_;
    sf::Music soundtrack_;
    std::vector<sf::SoundBuffer*> buffers_;
    std::vector<sf::Sound*> sounds_;
};

#endif