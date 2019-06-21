#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include "Logger.hpp"
#include <SFML/Audio.hpp>
#include <vector>

class SoundManager {
public:
    SoundManager(Logger* logger)
    {
        logger_ = logger;
        if (!soundtrack_.openFromFile("res/Sounds/Soundtrack.wav")) {
            logger_->error("SoundManager", "Soundtrack.wav could not be loaded");
            return;
        }
        soundtrack_.setLoop(true);
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

    void playSound(std::string item)
    {
        sf::SoundBuffer* buffer = new sf::SoundBuffer();
        if (!buffer->loadFromFile("res/Sounds/" + item + ".wav")) {
            logger_->error("SoundManager", "res/Sounds/" + item + ".wav could not be loaded");
            return;
        }
        sf::Sound* sound = new sf::Sound();
        sound->setBuffer(*buffer);
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

    void setVolume(float level)
    {
        soundtrack_.setVolume(level);
    }

private:
    Logger* logger_;
    sf::Music soundtrack_;
    std::vector<sf::SoundBuffer*> buffers_;
    std::vector<sf::Sound*> sounds_;
};

#endif