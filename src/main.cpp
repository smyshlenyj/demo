#include <iostream>

#include "runner.hpp"

int main(int argc, char* argv[])
{
    Runner runner;
    runner.run(argc, argv[1]);

    return 0;
}