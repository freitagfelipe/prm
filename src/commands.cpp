#include <commands.hpp>
#include <colors.hpp>
#include <iostream>

bool check_repository_category_string(std::string &category) {
    std::string valid_categories[] = {"created", "idle", "working", "finished"};

    for (const std::string &valid_category : valid_categories) {
        if (valid_category == category) {
            return true;
        }
    }

    return false;
}

void Commands::add(CLI::App &app) {
    CLI::App *add_subcommand {app.add_subcommand("add", "Add a repository")};

    add_subcommand->add_option("name", this->repositories_names, "Repositories to add")->required();

    add_subcommand->add_option("-c,--category", this->repository_category, "The category of the repositories (\"created\" | \"idle\" | \"working\" | \"finished\")")->capture_default_str();

    add_subcommand->callback([&]() {
        if (!check_repository_category_string(this->repository_category)) {
            std::cout << colors::red << "The category " << this->repository_category << " is not valid. The valid categories are: created, idle, working, and finished." << std::endl;

            return;
        }

        std::cout << colors::green << "Adding the following repositories in the category " << this->repository_category << ":";
        
        for (std::string repository_name : this->repositories_names) {
            std::cout << " " << repository_name;
        }

        std::cout << std::endl;
    });
}

void Commands::remove(CLI::App &app) {
    CLI::App *remove_subcommand {app.add_subcommand("remove", "Remove a repository")};

    remove_subcommand->add_option("name", this->repositories_names, "Repositories to remove")->required();

    remove_subcommand->callback([&]() {
        std::cout << colors::green << "Removing the following repositories:";

        for (std::string repository_name : this->repositories_names) {
            std::cout << " " << repository_name;
        }

        std::cout << std::endl;
    });
}

void Commands::status(CLI::App &app) {
    CLI::App *status_subcommand {app.add_subcommand("status", "The status of your repositories")};

    status_subcommand->callback([&]() {
        std::cout << colors::green << "Status subcommand executed." << std::endl;
    });
}

void Commands::update(CLI::App &app) {
    CLI::App *update_subcommand {app.add_subcommand("update", "Update the category of the given repositories")};

    update_subcommand->add_option("name", this->repositories_names, "Repositories to update");

    update_subcommand->add_option("-c,--category", this->repository_category, "The new category of the repositories (\"created\" | \"idle\" | \"working\" | \"finished\")")->capture_default_str();

    update_subcommand->callback([&]() {
        if (!check_repository_category_string(this->repository_category)) {
            std::cout << colors::red << "The category " << this->repository_category << " is not valid. The valid categories are: created, idle, working, and finished." << std::endl;

            return;
        }

        std::cout << colors::green << "Updating the following repositories to the category " << this->repository_category << ":";
        
        for (std::string repository_name : this->repositories_names) {
            std::cout << " " << repository_name;
        }

        std::cout << std::endl;
    });
}