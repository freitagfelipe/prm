#pragma once

#include <CLI/CLI.hpp>
#include <vector>
#include <string>

class Commands {
public:
    void add(CLI::App &app);
    void remove(CLI::App &app);
    void status(CLI::App &app);
    void update(CLI::App &app);
    void clone(CLI::App &app);
    void todo(CLI::App &app);
private:
    std::vector<std::string> repositories_names;
    std::vector<int> todo_numbers;
    int todo_number {};
    std::string todo_goal {};
    std::string repository_name {};
    std::string repository_clone_link {};
    std::string repository_category {"created"};

    void todo_add(CLI::App *todo_subcommand);
    void todo_remove(CLI::App *todo_subcommand);
    void todo_status(CLI::App *todo_subcommand);
    void todo_update(CLI::App *todo_subcommand);
};