#include "GameController.hpp"
#include "Logger.hpp"

int main()
{
    Logger logger(Logger::ALL);
    GameController controller(&logger);
    logger.log("main", "Game started");
    controller.start();
    return 0;
}