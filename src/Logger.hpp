#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iomanip>
#include <iostream>
#include <string>

class Logger {
public:
    Logger()
    {
    }

    void info(std::string x, std::string y)
    {
        std::cout << "INFO    " << std::setfill(' ') << std::setw(20) << std::left << x << " ";
        std::cout << std::setfill(' ') << std::setw(100) << std::left << y << std::endl;
    }

    void error(std::string x, std::string y)
    {
        std::cout << "ERROR   " << std::setfill(' ') << std::setw(20) << std::left << x << " ";
        std::cout << std::setfill(' ') << std::setw(100) << std::left << y << std::endl;
    }
    void warning(std::string x, std::string y)
    {
        std::cout << "WARNING " << std::setfill(' ') << std::setw(20) << std::left << x << " ";
        std::cout << std::setfill(' ') << std::setw(100) << std::left << y << std::endl;
    }

private:
};

#endif