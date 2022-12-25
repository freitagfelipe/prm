#pragma once

#include <string>
#include <vector>

namespace store {
    const std::vector<std::string> VALID_CATEGORIES {"created", "idle", "working", "finished", "others"};

    void add_repository(std::string &name, std::string &repository_clone_link, std::string &category);
    void remove_repositories(std::vector<std::string> &repositories);
    void update_repositories(std::vector<std::string> &repositories, std::string &new_category);
    void clone_repository(std::string &name);
    void print_repositories();
    void add_todo(std::string &name, std::string &goal);
    void remove_todo(std::string &name, std::vector<int> &todo_numbers);
    void update_todo(std::string &name, int todo_number, std::string &new_goal);
    void print_todo(std::string &name);
}