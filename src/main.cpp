#include <CLI/CLI.hpp>
#include <commands.hpp>
#include <iostream>

int main(int argc, char **argv) {
    CLI::App app {"PRM - A simple and ease-to-use personal repository manager"};
    Commands commands;

    commands.add(app);
    commands.remove(app);

    app.preparse_callback([](size_t arg_count) {
        if (arg_count == 0) {
            throw CLI::CallForHelp();
        }
    });

    CLI11_PARSE(app, argc, argv);

    return 0;
}