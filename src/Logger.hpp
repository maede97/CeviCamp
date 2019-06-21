#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iomanip>
#include <iostream>
#include <string>

class Logger {
public:
    enum Level { ALL,
        ERROR,
        WARNING,
        INFO,
        NOTHING };

    Logger(Level l)
    {
        level_ = l;
    }

    void info(std::string x, std::string y)
    {
        log(x, y, INFO);
    }

    void error(std::string x, std::string y)
    {
        log(x, y, ERROR);
    }
    void warning(std::string x, std::string y)
    {
        log(x, y, WARNING);
    }
    void log(std::string x, std::string y, Level l = ALL)
    {
        if (l <= level_) {
            std::cout << std::setfill(' ') << std::setw(20) << std::left << x << " ";
            std::cout << std::setfill(' ') << std::setw(100) << std::left << y << std::endl;
        }
    }

private:
    Level level_;
};

#endif