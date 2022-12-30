#include <commands.hpp>
#include <store.hpp>
#include <colors.hpp>
#include <iostream>

bool check_repository_category_string(std::string &category) {
    for (const std::string &valid_category : store::VALID_CATEGORIES) {
        if (valid_category == category) {
            return true;
        }
    }

    return false;
}

void Commands::add(CLI::App &app) {
    CLI::App *add_subcommand {app.add_subcommand("add", "Adds a repository")};

    add_subcommand->add_option("name", this->repository_name, "The name of the repository to be added")->required();

    add_subcommand->add_option("-l,--link", this->repository_clone_link, "The link to clone the given repository")->required();

    add_subcommand->add_option("-c,--category", this->repository_category, "The category of the repositories (created | idle | working | finished | others)")->required();

    add_subcommand->callback([&]() {
        if (!check_repository_category_string(this->repository_category)) {
            std::cout << colors::red << "The category " << this->repository_category << " isn't valid. The valid categories are: created, idle, working, finished, and others." << std::endl;

            return;
        }

        store::add_repository(this->repository_name, this->repository_clone_link, this->repository_category);

        std::cout << colors::green << "Successfully added the given repository in the " << this->repository_category << std::endl;
    });
}

void Commands::remove(CLI::App &app) {
    CLI::App *remove_subcommand {app.add_subcommand("remove", "Removes the given repository")};

    remove_subcommand->add_option("name", this->repositories_names, "The name of the repositories to be removed")->required();

    remove_subcommand->callback([&]() {
        store::remove_repositories(this->repositories_names);

        std::cout << colors::green << "Finished removing the given repositories" << std::endl;
    });
}

void Commands::status(CLI::App &app) {
    CLI::App *status_subcommand {app.add_subcommand("status", "Shows all your repositories and their categories")};

    status_subcommand->callback([&]() {
        store::print_repositories();
    });
}

void Commands::update(CLI::App &app) {
    CLI::App *update_subcommand {app.add_subcommand("update", "Updates informations of a repository")};

    update_subcommand->add_option("name", this->repository_name, "The name of the repository to be updated")->required();

    update_subcommand->add_option("-l,--link", this->repository_clone_link, "The new clone link of the given repository");

    update_subcommand->add_option("-n,--name", this->new_repository_name, "The new name of the repository to clone");

    update_subcommand->add_option("-c,--category", this->repository_category, "The new category of the repositories (created | idle | working | finished | others)");

    update_subcommand->preparse_callback([&](size_t arg_count) {
        if (arg_count == 1) {
            throw CLI::CallForHelp();
        }
    });

    update_subcommand->callback([&]() {
        if (this->repository_category != "" && !check_repository_category_string(this->repository_category)) {
            std::cout << colors::red << "The category " << this->repository_category << " isn't valid. The valid categories are: created, idle, working, finished, and others." << std::endl;

            return;
        }

        store::update_repository(this->repository_name, this->new_repository_name, this->repository_clone_link, this->repository_category);

        std::cout << colors::green << "Finished updating the given repository " << this->repository_name << std::endl;
    });
}

void Commands::clone(CLI::App &app) {
    CLI::App *clone_subcommand {app.add_subcommand("clone", "Clones a repository in the current directory")};

    clone_subcommand->add_option("name", this->repositories_names, "The names of the repositories to be cloned")->required();

    clone_subcommand->callback([&]() {
        store::clone_repositories(this->repositories_names);

        std::cout << colors::green << "Finished clone the given repositories" << std::endl;
    });
}

void Commands::todo_add(CLI::App *todo_subcommand) {
    CLI::App *todo_add_subcommand {todo_subcommand->add_subcommand("add", "Adds a To Do for the given repository")};

    todo_add_subcommand->add_option("goal", this->todo_goal, "The objective of the To Do")->required();

    todo_add_subcommand->callback([&]() {
        store::add_todo(this->repository_name, this->todo_goal);

        std::cout << colors::green << "Added the given To Do in the repository " << this->repository_name << std::endl;
    });
}

void Commands::todo_remove(CLI::App *todo_subcommand) {
    CLI::App *todo_remove_subcommand {todo_subcommand->add_subcommand("remove", "Removes the specified To Do's of the given repository")};

    todo_remove_subcommand->add_option("numbers", this->todo_numbers, "A list of To Do numbers")->required();

    todo_remove_subcommand->callback([&]() {
        store::remove_todo(this->repository_name, this->todo_numbers);

        std::cout << colors::green << "Finished removing the given To Do's of the repository " << this->repository_name << std::endl;
    });
}

void Commands::todo_status(CLI::App *todo_subcommand) {
    CLI::App *todo_status_subcommand {todo_subcommand->add_subcommand("status", "Shows all To Do's of the given repository")};

    todo_status_subcommand->callback([&]() {
        store::print_todo(this->repository_name);
    });
}

void Commands::todo_update(CLI::App *todo_subcommand) {
    CLI::App *todo_update_subcommand {todo_subcommand->add_subcommand("update", "Updates a To Do of the given repository")};

    todo_update_subcommand->add_option("goal", this->todo_goal, "The new objective of the To Do")->required();

    todo_update_subcommand->add_option("-n,--number", this->todo_number, "The number of the To Do to be updated")->required();

    todo_update_subcommand->callback([&]() {
        store::update_todo(this->repository_name, this->todo_number, this->todo_goal);

        std::cout << colors::green << "Updated the To Do of the given repository " << this->repository_name << std::endl;
    });
}

void Commands::todo(CLI::App &app) {
    CLI::App *todo_subcommand {app.add_subcommand("todo", "Manages the To Do of a repository")};

    todo_subcommand->add_option("name", this->repository_name, "The name of the repository to be updated")->required();

    this->todo_add(todo_subcommand);
    this->todo_remove(todo_subcommand);
    this->todo_status(todo_subcommand);
    this->todo_update(todo_subcommand);

    todo_subcommand->preparse_callback([](size_t arg_count) {
        if (arg_count == 1) {
            throw CLI::CallForHelp();
        }
    });
}