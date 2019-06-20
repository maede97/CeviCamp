#include "GameController.hpp"
#include "Logger.hpp"

int main()
{
    Logger logger(Logger::INFO);
    GameController controller(&logger);
    logger.info("main", "Game started");
    controller.start();
    return 0;
}