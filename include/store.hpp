#pragma once

#include <string>
#include <vector>

namespace store {
    void add_repository(std::string &name, std::string &clone_link, std::string &category);
    void remove_repository(std::vector<std::string> &repositories);
    void clone_repository(std::string &name);
    void print_repositories();
}