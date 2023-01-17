#pragma once

#include <string>
#include <vector>

namespace store {
    const std::vector<std::string> VALID_CATEGORIES {"created", "idle", "working", "finished", "others"};

    void add_repository(const std::string &repository_name, const std::string &repository_clone_link, const std::string &category);
    void remove_repositories(const std::vector<std::string> &repository_names);
    void update_repository(const std::string &repository_name, const std::string &new_repository_name, const std::string &new_repository_clone_link, const std::string &new_category);
    void clone_repositories(const std::vector<std::string> &repository_names);
    void print_repositories();
    void add_todo(const std::string &repository_name, const std::string &goal);
    void remove_todo(const std::string &repository_name, std::vector<int> &todo_numbers);
    void update_todo(const std::string &repository_name, const int todo_number, const std::string &new_goal);
    void print_todos(const std::string &repository_name);
}