#include "runner.hpp"

int main(int argc, char* argv[])
{
    Runner runner;
    runner.run(argc < 2 ? "" : argv[1]);

    return 0;
}