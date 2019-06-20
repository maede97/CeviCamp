#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include "Logger.hpp"
#include <SFML/Audio.hpp>

class SoundManager {
public:
    SoundManager(Logger* logger)
    {
        logger_ = logger;
        if (!soundtrack_.openFromFile("res/Sounds/Soundtrack.wav")) {
            logger_->log("SoundManager", "Soundtrack.wav could not be loaded");
            return;
        }
        soundtrack_.setLoop(true);
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
};

#endif