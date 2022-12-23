#pragma once

#include <string>

namespace store {
    void add_repository(std::string &name, std::string &clone_link, std::string &category);
    void clone_repository(std::string &name);
    void print_repositories();
}