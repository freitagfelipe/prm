#include <CLI/CLI.hpp>
#include <iostream>

int main(int argc, char **argv) {
    CLI::App app {"PRM - A simple and ease-to-use personal repository manager"};

    CLI11_PARSE(app, argc, argv);

    std::cout << app.help() << std::endl;

    return 0;
}