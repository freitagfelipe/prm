#include <store.hpp>
#include <utils.hpp>
#include <nlohmann/json.hpp>
#include <colors.hpp>
#include <fstream>
#include <iostream>
#include <set>
#include <regex>

const std::string NAME_KEY {"name"};
const std::string LINK_KEY {"link"};
const std::string TODO_KEY {"todo"};
const std::string REGEX_PATTERN {"git@git((hub)|(lab))\\.com:\\S*\\/\\S*\\.git"};

std::pair<bool, nlohmann::json> remove_repository(nlohmann::json &j, std::string &repository) {
    auto it {std::find_if(j.begin(), j.end(), [&](nlohmann::json &val) {
        return val[NAME_KEY].get<std::string>() == repository;
    })};

    if (it != j.end()) {
        nlohmann::json old_value {*it};

        j.erase(it);

        return {true, old_value};
    }

    return {false, {}};
}

bool check_if_can_insert_goal(nlohmann::json &j, std::string &name, std::string &goal) {
    for (auto &[_, curr_goal] : j[TODO_KEY][name].items()) {
        if (curr_goal.get<std::string>() == goal) {
            return false;
        }
    }

    return true;
}

void store::add_repository(std::string &name, std::string &repository_link, std::string &category) {
    std::regex regex(REGEX_PATTERN);

    if (!std::regex_match(repository_link, regex)) {
        std::cerr << colors::red << "Invalid repository link, must be a ssh clone link for the github or gitlab" << std::endl;

        std::exit(2);
    }

    std::fstream f {utils::open_config_file(std::ios::in | std::ios::out)};

    nlohmann::json j {nlohmann::json::parse(f)};
    nlohmann::json new_value = {
        {LINK_KEY, repository_link},
        {NAME_KEY, name}
    };

    j = j.at(0);

    for (const std::string &curr_category : store::VALID_CATEGORIES) {
        if (j[curr_category].is_null()) {
            continue;
        }

        for (auto &[_, value] : j[curr_category].items()) {
            if (value[NAME_KEY] == name) {
                std::cerr << colors::red << "You already have this repository!" << std::endl;

                std::exit(2);
            }
        }
    }

    j[category].push_back(new_value);
    j[TODO_KEY][name] = nlohmann::json::array();

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
            if (value[NAME_KEY] == name) {
                if (std::system("git 2> /dev/null 1> /dev/null") != 256) {
                    std::cerr << colors::red << "You should have git installed to execute this command" << std::endl;

                    std::exit(2);
                }

                std::stringstream ss;

                ss << "git clone " << value[LINK_KEY].get<std::string>() << " 2> /dev/null 1> /dev/null";

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

        for (const std::string &category : store::VALID_CATEGORIES) {
            if (remove_repository(j[category], repository).first) {
                j[TODO_KEY].erase(repository);

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

void store::update_repositories(std::vector<std::string> &repositories, std::string &new_category) {
    std::fstream f {utils::open_config_file(std::ios::in | std::ios::out)};

    nlohmann::json j {nlohmann::json::parse(f)};

    j = j.at(0);

    std::vector<nlohmann::json> removed_repositories;

    for (std::string &repository : repositories) {
        bool removed {};
        nlohmann::json old_value {};

        for (auto &[key, val] : j.items()) {
            auto [erased, aux] = remove_repository(val, repository);

            if (erased) {
                old_value = aux;
                removed = true;

                break;
            }
        }

        if (!removed) {
            std::cerr << colors::red << "Can't find the repository " << repository << std::endl;
        } else {
            removed_repositories.push_back(old_value.at(0));
        }
    }

    for (nlohmann::json &removed_repository : removed_repositories) {
        j[new_category].push_back(removed_repository);
    }

    std::sort(j[new_category].begin(), j[new_category].end());

    f.close();

    f = utils::open_config_file(std::ios::out);

    f << std::setw(4) << j;
}

void store::add_todo(std::string &name, std::string &goal) {
    std::fstream f {utils::open_config_file(std::ios::in | std::ios::out)};

    nlohmann::json j {nlohmann::json::parse(f)};

    j = j.at(0);

    if (!check_if_can_insert_goal(j, name, goal)) {
        std::cerr << colors::red << "The given To Do is already inserted in the repository " << name << std::endl;

        std::exit(2);
    }

    j[TODO_KEY][name].push_back(goal);

    f.close();

    f = utils::open_config_file(std::ios::out);

    f << std::setw(4) << j;
}

void store::print_todo(std::string &name) {
    std::fstream f {utils::open_config_file(std::ios::in | std::ios::out)};

    nlohmann::json j {nlohmann::json::parse(f)};

    j = j.at(0);

    if (j[TODO_KEY][name].size() == 0) {
        std::cerr << colors::red << "The repository " << name << " doesn't have any To Dos" << std::endl;

        std::exit(2);
    }

    for (auto &[i, goal] : j[TODO_KEY][name].items()) {
        std::cout << colors::cyan << std::stoi(i) + 1 << ". " << colors::white << goal.get<std::string>() << std::endl;
    }
}

void store::remove_todo(std::string &name, std::vector<int> &todo_numbers) {
    std::fstream f {utils::open_config_file(std::ios::in | std::ios::out)};

    nlohmann::json j {nlohmann::json::parse(f)};

    j = j.at(0);

    if (j[TODO_KEY][name].size() == 0) {
        std::cerr << colors::red << "The repository " << name << " doesn't have any To Dos" << std::endl;

        std::exit(2);
    }

    std::set<int> removed_numbers;

    for (int number : todo_numbers) {
        if (removed_numbers.find(number) != removed_numbers.end()) {
            std::cerr << colors::red << "The To Do of number " << number << " is already removed" << std::endl;

            continue;
        }

        if (number <= 0 || number - removed_numbers.size() > j[TODO_KEY][name].size()) {
            std::cerr << colors::red << "The To Do of number " << number << " doesn't exist" << std::endl;

            continue;
        }

        j[TODO_KEY][name].erase((number - removed_numbers.size()) - 1);

        removed_numbers.insert(number);

        std::cout << colors::green << "Removed the To Do of number " << number << std::endl;
    }

    f.close();

    f = utils::open_config_file(std::ios::out);

    f << std::setw(4) << j;
}

void store::update_todo(std::string &name, int todo_number, std::string &new_goal) {
    std::fstream f {utils::open_config_file(std::ios::in | std::ios::out)};

    nlohmann::json j {nlohmann::json::parse(f)};

    j = j.at(0);

    if (j[TODO_KEY][name].size() == 0) {
        std::cerr << colors::red << "The repository " << name << " doesn't have any To Dos" << std::endl;

        std::exit(2);
    } else if (todo_number <= 0 || size_t(todo_number) > j[TODO_KEY][name].size()) {
        std::cerr << colors::red << "The To Do of number " << todo_number << " doesn't exist" << std::endl;

        std::exit(2);
    } else if (!check_if_can_insert_goal(j, name, new_goal)) {
        std::cerr << colors::red << "The given To Do is already inserted in the repository " << name << std::endl;

        std::exit(2);
    }

    j[TODO_KEY][name][todo_number - 1] = new_goal;

    f.close();

    f = utils::open_config_file(std::ios::out);

    f << std::setw(4) << j;
}