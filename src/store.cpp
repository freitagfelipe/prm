#include <store.hpp>
#include <utils.hpp>
#include <nlohmann/json.hpp>
#include <colors.hpp>
#include <fstream>
#include <iostream>

const std::string name_key {"name"};
const std::string link_key {"link"};

bool remove_repository(nlohmann::json &j, std::string &repository) {
    auto it {std::find_if(j.begin(), j.end(), [&](nlohmann::json &val) {
        return val[name_key].get<std::string>() == repository;
    })};

    if (it != j.end()) {
        j.erase(it);

        return true;
    }

    return false;
}

void store::add_repository(std::string &name, std::string &repository_link, std::string &category) {
    std::fstream f {utils::open_config_file(std::ios::in | std::ios::out)};

    nlohmann::json j {nlohmann::json::parse(f)};
    nlohmann::json new_value = {
        {link_key, repository_link},
        {name_key, name}
    };

    j = j.at(0);

    for (auto &[curr_category, curr_value] : j.items()) { 
        if (curr_value.is_null()) {
            continue;
        }

        for (auto &[_, value] : curr_value.items()) {
            if (value[name_key] == name) {
                std::cerr << colors::red << "You already have this repository!" << std::endl;

                std::exit(2);
            }
        }
    }

    j[category].push_back(new_value);

    std::sort(j[category].begin(), j[category].end());

    f.seekp(0);
    f.clear();

    f << std::setw(4) << j;
}

void store::print_repositories() {
    std::fstream f {utils::open_config_file(std::ios::in)};

    nlohmann::json j {nlohmann::json::parse(f)};

    j = j.at(0);

    std::vector<std::string> categories {"created", "idle", "working", "finished", "others"};

    for (std::string category : categories) {
        std::cout << colors::cyan << category << ":" << std::endl;

        if (j[category].is_null()) {
            std::cout << colors::red << "\tEmpty category" << std::endl;

            continue;
        }

        for (auto &[_, value] : j[category].items()) {
            std::cout << colors::white << '\t' << value["name"].get<std::string>() << std::endl;
        }
    }
}

void store::clone_repository(std::string &name) {
    std::fstream f {utils::open_config_file(std::ios::in)};

    nlohmann::json j {nlohmann::json::parse(f)};

    j = j.at(0);

    for (auto &[curr_category, curr_value] : j.items()) {
        if (curr_value.is_null()) {
            continue;
        }

        for (auto &[_, value] : curr_value.items()) {
            if (value[name_key] == name) {
                if (std::system("git 2> /dev/null 1> /dev/null") != 256) {
                    std::cerr << colors::red << "You should have git installed to execute this command" << std::endl;

                    std::exit(2);
                }

                std::stringstream ss;

                ss << "git clone " << value[link_key].get<std::string>() << " 2> /dev/null 1> /dev/null";

                if (std::system(ss.str().c_str()) == 32768) {
                    std::cerr << colors::red << "You already have this repository in the current directory" << std::endl;

                    std::exit(2);
                }

                return;
            }
        }
    }

    std::cerr << colors::red << "Can't find the given repository" << std::endl;

    std::exit(2);
}


void store::remove_repositories(std::vector<std::string> &repositories) {
    std::fstream f {utils::open_config_file(std::ios::in | std::ios::out)};

    nlohmann::json j {nlohmann::json::parse(f)};

    j = j.at(0);

    for (std::string &repository : repositories) {
        bool removed {};

        for (auto &[curr_category, curr_value] : j.items()) {
            if (remove_repository(curr_value, repository)) {
                removed = true;

                break;
            }
        }

        if (!removed) {
            std::cerr << colors::red << "Can't find the repository " << repository << std::endl;
        } else {
            std::cout << colors::green << "Removed the repository " << repository << std::endl;
        }
    }

    f.close();

    f = utils::open_config_file(std::ios::out);

    f << std::setw(4) << j;
}