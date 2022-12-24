#include <commands.hpp>
#include <store.hpp>
#include <colors.hpp>
#include <iostream>

bool check_repository_category_string(std::string &category) {
    std::string valid_categories[] = {"created", "idle", "working", "finished", "others"};

    for (const std::string &valid_category : valid_categories) {
        if (valid_category == category) {
            return true;
        }
    }

    return false;
}

void Commands::add(CLI::App &app) {
    CLI::App *add_subcommand {app.add_subcommand("add", "Adds a repository")};

    add_subcommand->add_option("name", this->repository_name, "Repository name")->required();

    add_subcommand->add_option("-l,--link", this->repository_link, "The link to clone the given repository")->required();

    add_subcommand->add_option("-c,--category", this->repository_category, "The category of the repositories (\"created\" | \"idle\" | \"working\" | \"finished\" | \"others\")")->capture_default_str();

    add_subcommand->callback([&]() {
        if (!check_repository_category_string(this->repository_category)) {
            std::cout << colors::red << "The category " << this->repository_category << " is not valid. The valid categories are: created, idle, working, finished, and others." << std::endl;

            return;
        }

        store::add_repository(this->repository_name, this->repository_link, this->repository_category);

        std::cout << colors::green << "Successfully added the repository in the " << this->repository_category << "!" << std::endl;
    });
}

void Commands::remove(CLI::App &app) {
    CLI::App *remove_subcommand {app.add_subcommand("remove", "Removes a repository")};

    remove_subcommand->add_option("name", this->repositories_names, "Repositories to remove")->required();

    remove_subcommand->callback([&]() {
        store::remove_repositories(this->repositories_names);

        std::cout << colors::green << "Finished removing the repositories" << std::endl;
    });
}

void Commands::status(CLI::App &app) {
    CLI::App *status_subcommand {app.add_subcommand("status", "Shows all your repositories and their categories")};

    status_subcommand->callback([&]() {
        store::print_repositories();
    });
}

void Commands::update(CLI::App &app) {
    CLI::App *update_subcommand {app.add_subcommand("update", "Updates the category of your repositories")};

    update_subcommand->add_option("name", this->repositories_names, "Repositories to update");

    update_subcommand->add_option("-c,--category", this->repository_category, "The new category of the repositories (\"created\" | \"idle\" | \"working\" | \"finished\" | \"others\")")->capture_default_str();

    update_subcommand->callback([&]() {
        if (!check_repository_category_string(this->repository_category)) {
            std::cout << colors::red << "The category " << this->repository_category << " is not valid. The valid categories are: created, idle, working, finished, and others." << std::endl;

            return;
        }

        store::update_repositories(this->repositories_names, this->repository_category);

        std::cout << colors::green << "Finished updating the repositories to the category " << this->repository_category << std::endl;
    });
}

void Commands::clone(CLI::App &app) {
    CLI::App *clone_subcommand {app.add_subcommand("clone", "Clones a repository in the current directory")};

    clone_subcommand->add_option("name", this->repository_name, "The name of the repository to clone")->required();

    clone_subcommand->callback([&]() {
        store::clone_repository(this->repository_name);

        std::cout << colors::green << "Cloned " << this->repository_name << " in the current directory" << std::endl;
    });
}

void Commands::todo_add(CLI::App *todo_subcommand) {
    CLI::App *todo_add_subcommand {todo_subcommand->add_subcommand("add", "Adds a To Do for the given repository")};

    todo_add_subcommand->add_option("-g,--goal", this->todo_goal, "The objective of the To Do")->required();

    todo_add_subcommand->callback([&]() {
        std::cout << colors::green << "Adding the following To Do " << this->todo_goal << " to the following repository " << this->repository_name << std::endl;
    });
}

void Commands::todo_remove(CLI::App *todo_subcommand) {
    CLI::App *todo_remove_subcommand {todo_subcommand->add_subcommand("remove", "Removes To Dos of the given repository")};

    todo_remove_subcommand->add_option("-n,--number", this->todo_numbers, "The number of the To-Do")->required();

    todo_remove_subcommand->callback([&]() {
        std::cout << colors::green << "Removing the following To Dos of the repository " << this->repository_name << ":";

        for (int todo_number : this->todo_numbers) {
            std::cout << ' ' << todo_number;
        }

        std::cout << std::endl;
    });
}

void Commands::todo_status(CLI::App *todo_subcommand) {
    CLI::App *todo_status_subcommand {todo_subcommand->add_subcommand("status", "Shows all To Dos of the given repository")};

    todo_status_subcommand->callback([&]() {
        std::cout << colors::green << "Showing the To Dos of the repository " << this->repository_name << std::endl;
    });
}

void Commands::todo_update(CLI::App *todo_subcommand) {
    CLI::App *todo_update_subcommand {todo_subcommand->add_subcommand("update", "Updates a To Do of the given repository")};

    todo_update_subcommand->add_option("-n,--number", this->todo_number, "The number of the To Do to be updated")->required();

    todo_update_subcommand->add_option("-g,--goal", this->todo_goal, "The objective of the To Do")->required();

    todo_update_subcommand->callback([&]() {
        std::cout << colors::green << "Updating the To Do of number " << this->todo_number << " of the repository " << this->repository_name << " with the following task " << this->todo_goal << std::endl;
    });
}

void Commands::todo(CLI::App &app) {
    CLI::App *todo_subcommand {app.add_subcommand("todo", "Manages the To Do of your repositories")};

    todo_subcommand->add_option("name", this->repository_name, "The name of the repository")->required();

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