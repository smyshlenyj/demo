#include <iostream>

#include "loggerWrapper.hpp"
#include "runner.hpp"

int main(int argc, char* argv[])
{
    try
    {
        LoggerWrapper::init();
        auto log = LoggerWrapper::get();
        log->info("=========================");
        log->info("Calc started");

        if (argc < 2)
        {
            std::string error = "Input file path is not provided";
            log->error(error);
            throw std::invalid_argument(error);
        }
        Runner runner(log);
        runner.run(argv[1]);
        log->info("Calc finished successfully");
        return 0;
    }
    catch (const std::invalid_argument& e)
    {
        auto log = LoggerWrapper::get();
        log->critical("Invalid arguments: {}", e.what());
        std::cerr << "Invalid arguments: " << e.what() << "\n";
        return 2;
    }
    catch (const std::exception& e)
    {
        auto log = LoggerWrapper::get();
        log->critical("Calc terminated due to fatal error");
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
}