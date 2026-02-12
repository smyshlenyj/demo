#include <iostream>
#include "runner.hpp"

int main()
{
    std::cout << "MAIN START\n";
    try
    {
        ServerRunner server;
        server.run();

        return 0;
    }

    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown fatal error" << '\n';
        return 1;
    }
}