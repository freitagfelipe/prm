#include <commands.hpp>
#include <setup.hpp>
#include <CLI/CLI.hpp>
#include <iostream>

int main(int argc, char **argv) {
    CLI::App app {"PRM - A simple and ease-to-use personal repository manager"};
    Commands commands;

    setup::init();

    commands.add(app);
    commands.remove(app);
    commands.status(app);
    commands.update(app);
    commands.clone(app);
    commands.todo(app);

    app.preparse_callback([](size_t arg_count) {
        if (arg_count == 0) {
            throw CLI::CallForHelp();
        }
    });

    CLI11_PARSE(app, argc, argv);

    return 0;
}