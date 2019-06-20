#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iomanip>
#include <iostream>

class Logger {
public:
    enum Level { ALL,
        NOTHING };

    Logger(Level l)
    {
        level_ = l;
    }

    void log(const char* x, Level l = ALL)
    {
        if (l == NOTHING)
            return;
        std::cout << x << std::endl;
    }
    void log(const char* x, const char* y, Level l = ALL)
    {
        if (l == NOTHING)
            return;
        std::cout << std::setfill(' ') << std::setw(15) << std::left << x << " ";
        std::cout << std::setfill(' ') << std::setw(100) << std::left << y << std::endl;
    }

private:
    Level level_;
};

#endif