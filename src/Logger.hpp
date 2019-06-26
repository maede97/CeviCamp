#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iomanip>
#include <string>

#ifdef NDEBUG

#include <fstream>
class Logger {
public:
    Logger()
    {
        out = std::ofstream("cevicamp_log.txt");
    }
    ~Logger()
    {
        out.close();
    }
    void info(std::string x, std::string y)
    {
        out << "INFO    " << std::setfill(' ') << std::setw(20) << std::left << x << " ";
        out << std::setfill(' ') << std::setw(100) << std::left << y << std::endl;
    }
    void error(std::string x, std::string y)
    {
        out << "ERROR   " << std::setfill(' ') << std::setw(20) << std::left << x << " ";
        out << std::setfill(' ') << std::setw(100) << std::left << y << std::endl;
    }
    void warning(std::string x, std::string y)
    {
        out << "WARNING " << std::setfill(' ') << std::setw(20) << std::left << x << " ";
        out << std::setfill(' ') << std::setw(100) << std::left << y << std::endl;
    }
private:
    std::ofstream out;
};
#else
#include <iostream>
class Logger {
public:
    Logger() {}

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

#endif /* if RELEASE */

#endif /* LOGGER_HPP */