#pragma once

#include <CLI/CLI.hpp>
#include <vector>
#include <string>

class Commands {
public:
    void add(CLI::App &app);
    void remove(CLI::App &app);
private:
    std::vector<std::string> repositories_names;
    std::string repository_category {"created"};
};

bool check_repository_category_string(std::string &category);