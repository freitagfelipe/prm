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
    CLI::App *add_subcommand {app.add_subcommand("add", "Add a repository")};

    add_subcommand->add_option("name", this->repository_name, "Repository name")->required();

    add_subcommand->add_option("-l,--link", this->repository_link, "The link to clone the given repository")->required();

    add_subcommand->add_option("-c,--category", this->repository_category, "The category of the repositories (\"created\" | \"idle\" | \"working\" | \"finished\" | \"others\")")->capture_default_str();

    add_subcommand->callback([&]() {
        if (!check_repository_category_string(this->repository_category)) {
            std::cout << colors::red << "The category " << this->repository_category << " is not valid. The valid categories are: created, idle, working, and finished." << std::endl;

            return;
        }

        store::add_repository(this->repository_name, this->repository_link, this->repository_category);

        std::cout << colors::green << "Successfully added the repository in the " << this->repository_category << "!" << std::endl;
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
        store::print();
    });
}

void Commands::update(CLI::App &app) {
    CLI::App *update_subcommand {app.add_subcommand("update", "Update the category of the given repositories")};

    update_subcommand->add_option("name", this->repositories_names, "Repositories to update");

    update_subcommand->add_option("-c,--category", this->repository_category, "The new category of the repositories (\"created\" | \"idle\" | \"working\" | \"finished\" | \"others\")")->capture_default_str();

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

void Commands::clone(CLI::App &app) {
    CLI::App *clone_subcommand {app.add_subcommand("clone", "Clone the given repository in the current directory")};

    clone_subcommand->add_option("name", this->repository_name, "The name of the repository to clone")->required();

    clone_subcommand->callback([&]() {
        store::clone(this->repository_name);

        std::cout << colors::green << "Cloned " << this->repository_name << " in the current directory" << std::endl;
    });
}