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
const std::string CREATED_KEY {"created"};
const std::string FINISHED_KEY {"finished"};
const std::string REGEX_PATTERN {"git@git((hub)|(lab))\\.com:\\S*\\/\\S*\\.git"};

#if defined(_WIN32) || defined(_WIN64)
    const std::string CHECK_IF_GIT_IS_INSTALLED_COMMAND {"git 2>nul 1>nul"};
    const int STATUS_CODE {1};
#elif defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
    const std::string CHECK_IF_GIT_IS_INSTALLED_COMMAND {"git 2> /dev/null 1> /dev/null"};
    const int STATUS_CODE {256};
#endif

std::pair<bool, nlohmann::json> remove_repository(nlohmann::json &j, const std::string &repository) {
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

bool check_if_can_insert_goal(const nlohmann::json &j, const std::string &name, const std::string &goal) {
    for (const auto &[_, curr_goal] : j[TODO_KEY][name].items()) {
        if (curr_goal.get<std::string>() == goal) {
            return false;
        }
    }

    return true;
}

void sort_json_category(nlohmann::json &j) {
    std::sort(j.begin(), j.end(), [](const nlohmann::json &a, const nlohmann::json &b) {
        return a[NAME_KEY] < b[NAME_KEY];
    });
}

void store::add_repository(const std::string &repository_name, const std::string &repository_clone_link, const std::string &category) {
    std::regex regex(REGEX_PATTERN);

    if (!std::regex_match(repository_clone_link, regex)) {
        std::cerr << colors::red << "Invalid repository link, must be a ssh clone link for the github or gitlab repository" << colors::reset << std::endl;

        std::exit(2);
    }

    std::fstream f {utils::open_config_file(std::ios::in | std::ios::out)};

    nlohmann::json j {nlohmann::json::parse(f)};
    nlohmann::json new_value = {
        {LINK_KEY, repository_clone_link},
        {NAME_KEY, repository_name}
    };

    j = j.at(0);

    for (const std::string &curr_category : store::VALID_CATEGORIES) {
        if (j[curr_category].is_null()) {
            continue;
        }

        for (auto &[_, value] : j[curr_category].items()) {
            if (value[NAME_KEY] == repository_name) {
                std::cerr << colors::red << "You already have this repository" << colors::reset << std::endl;

                std::exit(2);
            }
        }
    }

    j[category].push_back(new_value);
    j[TODO_KEY][repository_name] = nlohmann::json::array();

    sort_json_category(j[category]);

    f.seekp(0);
    f.clear();

    f << std::setw(4) << j;
}

void store::print_repositories() {
    std::fstream f {utils::open_config_file(std::ios::in)};

    nlohmann::json j {nlohmann::json::parse(f)};

    j = j.at(0);

    for (const std::string &category : store::VALID_CATEGORIES) {
        std::cout << colors::cyan << category << ":" << std::endl;

        if (j[category].size() == 0) {
            std::cout << colors::red << "\tEmpty category" << std::endl;

            continue;
        }

        for (auto &[_, value] : j[category].items()) {
            std::cout << colors::white << '\t' << value[NAME_KEY].get<std::string>() << std::endl;
        }
    }
}

void store::clone_repositories(const std::vector<std::string> &repository_names) {
    if (std::system(CHECK_IF_GIT_IS_INSTALLED_COMMAND.c_str()) != STATUS_CODE) {
        std::cerr << colors::red << "You should have git installed to execute this command" << colors::reset << std::endl;

        std::exit(2);
    }

    std::fstream f {utils::open_config_file(std::ios::in)};

    nlohmann::json j {nlohmann::json::parse(f)};

    j = j.at(0);

    for (const std::string &name : repository_names) {
        int status {};

        for (const std::string &category : store::VALID_CATEGORIES) {
            if (j[category].size() == 0) {
                continue;
            }

            for (auto &[_, value] : j[category].items()) {
                if (value[NAME_KEY] == name) {
                    std::stringstream ss;

                    ss << "git clone " << value[LINK_KEY].get<std::string>() << " 2> /dev/null 1> /dev/null";

                    if (std::system(ss.str().c_str()) == 32768) {
                        status = -1;
                    } else {
                        status = 1;
                    }

                    break;
                }
            }

            if (status != 0) {
                break;
            }
        }

        if (status == 1) {
            std::cout << colors::green << "Cloned the repository " << name << " in the current directory" << std::endl;
        } else if (status == 0) {
            std::cerr << colors::red << "Can not find the repository " << name << std::endl;
        } else {
            std::cerr << colors::red << "You already have this repository in the current directory or the clone link is invalid" << std::endl;
        }
    }
}

void store::remove_repositories(const std::vector<std::string> &repository_names) {
    std::fstream f {utils::open_config_file(std::ios::in | std::ios::out)};

    nlohmann::json j {nlohmann::json::parse(f)};

    j = j.at(0);

    for (const std::string &repository : repository_names) {
        bool removed {};

        for (const std::string &category : store::VALID_CATEGORIES) {
            if (remove_repository(j[category], repository).first) {
                j[TODO_KEY].erase(repository);

                removed = true;

                break;
            }
        }

        if (!removed) {
            std::cerr << colors::red << "Can not find the repository " << repository << std::endl;
        } else {
            std::cout << colors::green << "Removed the repository " << repository << std::endl;
        }
    }

    f.close();

    f = utils::open_config_file(std::ios::out);

    f << std::setw(4) << j;
}

void store::update_repository(const std::string &repository_name, const std::string &new_repository_name, const std::string &new_repository_clone_link, const std::string &new_category) {
    std::regex regex(REGEX_PATTERN);

    if (new_repository_clone_link != "" && !std::regex_match(new_repository_clone_link, regex)) {
        std::cerr << colors::red << "Invalid repository link, must be a ssh clone link for the github or gitlab repository" << colors::reset << std::endl;

        std::exit(2);
    }

    std::fstream f {utils::open_config_file(std::ios::in | std::ios::out)};

    nlohmann::json j {nlohmann::json::parse(f)};

    j = j.at(0);

    for (const std::string &category : VALID_CATEGORIES) {
        for (auto &[_, val] : j[category].items()) {
            if (val[NAME_KEY].get<std::string>() != repository_name) {
                continue;
            }

            if (new_repository_name != "" && new_repository_name != val[NAME_KEY].get<std::string>()) {
                val[NAME_KEY] = new_repository_name;
                j[TODO_KEY][new_repository_name] = j[TODO_KEY][repository_name];
                j[TODO_KEY].erase(repository_name);

                std::cout << colors::green << "Updated the repository name" << std::endl;
            } else if (new_repository_name != "" && val[NAME_KEY].get<std::string>() == new_repository_name) {
                std::cerr << colors::red << "The new given name is equal to the old name, so nothing changes" << std::endl;
            }

            if (new_repository_clone_link == val[LINK_KEY].get<std::string>()) {
                std::cerr << colors::red << "The new given clone link is equal to the old clone link, so nothing changes" << std::endl;
            } else if (new_repository_clone_link != "") {
                val[LINK_KEY] = new_repository_clone_link;

                std::cout << colors::green << "Updated the repository clone link" << std::endl;
            }

            if (new_category == category) {
                std::cerr << colors::red << "The new given category is equal to the old category, so nothing changes" << std::endl;
            }

            if (new_category == category || new_category == "") {
                sort_json_category(j[category]);
            } else {
                std::string target_name {new_repository_name != "" ? new_repository_name : repository_name};

                auto [erased, old_val] = remove_repository(j[category], target_name);

                j[new_category].push_back(old_val.at(0));

                sort_json_category(j[new_category]);

                std::cout << colors::green << "Updated the repository category" << std::endl;
            }

            f.close();

            f = utils::open_config_file(std::ios::out);

            f << std::setw(4) << j;

            return;
        }
    }

    std::cout << colors::green << "The given repository does not exists" << colors::reset << std::endl;

    std::exit(2);
}

void store::add_todo(const std::string &repository_name, const std::string &goal) {
    std::fstream f {utils::open_config_file(std::ios::in | std::ios::out)};

    nlohmann::json j {nlohmann::json::parse(f)};

    j = j.at(0);

    for (const std::string &category : {CREATED_KEY, FINISHED_KEY}) {
        for (auto &[_, val] : j[category].items()) {
            if (val[NAME_KEY].get<std::string>() == repository_name) {
                std::cerr << colors::red << "You can not insert a To Do if the repository is in the category created or finished" << colors::reset << std::endl;

                std::exit(2);
            }
        }
    }

    if (j[TODO_KEY].find(repository_name) == j[TODO_KEY].end()) {
        std::cerr << colors::red << "The given repository does not exists" << colors::reset << std::endl;

        std::exit(2);
    } else if (!check_if_can_insert_goal(j, repository_name, goal)) {
        std::cerr << colors::red << "The given To Do is already inserted in the repository " << repository_name << colors::reset << std::endl;

        std::exit(2);
    }

    j[TODO_KEY][repository_name].push_back(goal);

    f.close();

    f = utils::open_config_file(std::ios::out);

    f << std::setw(4) << j;
}

void store::print_todos(const std::string &repository_name) {
    std::fstream f {utils::open_config_file(std::ios::in | std::ios::out)};

    nlohmann::json j {nlohmann::json::parse(f)};

    j = j.at(0);

    if (j[TODO_KEY].find(repository_name) == j[TODO_KEY].end()) {
        std::cerr << colors::red << "The repository " << repository_name << " does not exists" << std::endl;
    } else if (j[TODO_KEY][repository_name].size() == 0) {
        std::cerr << colors::red << "The repository " << repository_name << " does not have any To Do's" << colors::reset << std::endl;

        std::exit(2);
    }

    for (auto &[i, goal] : j[TODO_KEY][repository_name].items()) {
        std::cout << colors::cyan << std::stoi(i) + 1 << ". " << colors::white << goal.get<std::string>() << std::endl;
    }
}

void store::remove_todo(const std::string &repository_name, std::vector<int> &todo_numbers) {
    std::fstream f {utils::open_config_file(std::ios::in | std::ios::out)};

    std::sort(todo_numbers.begin(), todo_numbers.end());

    nlohmann::json j {nlohmann::json::parse(f)};

    j = j.at(0);

    if (j[TODO_KEY][repository_name].size() == 0) {
        std::cerr << colors::red << "The repository " << repository_name << " does not have any To Do's" << colors::reset << std::endl;

        std::exit(2);
    }

    std::set<int> removed_numbers;

    for (const int number : todo_numbers) {
        if (removed_numbers.find(number) != removed_numbers.end()) {
            std::cerr << colors::red << "The To Do of number " << number << " is already removed" << std::endl;

            continue;
        }

        if (number <= 0 || number - removed_numbers.size() > j[TODO_KEY][repository_name].size()) {
            std::cerr << colors::red << "The To Do of number " << number << " does not exist" << std::endl;

            continue;
        }

        j[TODO_KEY][repository_name].erase((number - removed_numbers.size()) - 1);

        removed_numbers.insert(number);

        std::cout << colors::green << "Removed the To Do of number " << number << std::endl;
    }

    f.close();

    f = utils::open_config_file(std::ios::out);

    f << std::setw(4) << j;
}

void store::update_todo(const std::string &repository_name, const int todo_number, const std::string &new_goal) {
    std::fstream f {utils::open_config_file(std::ios::in | std::ios::out)};

    nlohmann::json j {nlohmann::json::parse(f)};

    j = j.at(0);

    if (j[TODO_KEY][repository_name].size() == 0) {
        std::cerr << colors::red << "The repository " << repository_name << " does not have any To Do's" << colors::reset << std::endl;

        std::exit(2);
    } else if (todo_number <= 0 || size_t(todo_number) > j[TODO_KEY][repository_name].size()) {
        std::cerr << colors::red << "The To Do of number " << todo_number << " does not exist" << colors::reset << std::endl;

        std::exit(2);
    } else if (!check_if_can_insert_goal(j, repository_name, new_goal)) {
        std::cerr << colors::red << "The given To Do is already inserted in the repository " << repository_name << colors::reset << std::endl;

        std::exit(2);
    }

    j[TODO_KEY][repository_name][todo_number - 1] = new_goal;

    f.close();

    f = utils::open_config_file(std::ios::out);

    f << std::setw(4) << j;
}