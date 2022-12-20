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

    add_subcommand->add_option("-n,--name", this->repository_name, "The name of the repository")->required();

    add_subcommand->add_option("-c,--category", this->repository_category, "The category of the repository (\"created\" | \"idle\" | \"working\" | \"finished\")")->capture_default_str();

    add_subcommand->callback([&]() {
        if (!check_repository_category_string(this->repository_category)) {
            std::cout << colors::red << "The category " << this->repository_category << " is not valid. The valid categories are: created, idle, working, and finished." << std::endl;

            return;
        }

        std::cout << colors::green << "Repository to be added name: " << this->repository_name << ", with the category: " << this->repository_category << std::endl;
    });
}

void Commands::remove(CLI::App &app) {
    CLI::App *remove_subcommand {app.add_subcommand("remove", "Remove a repository")};

    static std::string repository_name {};

    remove_subcommand->add_option("-n,--name", this->repository_name, "The name of the repository")->required();

    remove_subcommand->callback([&]() {
        std::cout << colors::green << "Repository to be removed name: " << this->repository_name << std::endl;
    });
}