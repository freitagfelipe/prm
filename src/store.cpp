#include <store.hpp>
#include <nlohmann/json.hpp>
#include <colors.hpp>
#include <fstream>
#include <iostream>
#include <vector>

void store::add_repository(std::string &name, std::string &repository_link, std::string &category) {
    std::fstream f;

    f.open(".prm.json", std::ios::in | std::ios::out);

    if (!f.is_open()) {
        std::cerr << colors::red << "Can't open the file" << std::endl;

        std::exit(1);
    }

    nlohmann::json j {nlohmann::json::parse(f)};
    nlohmann::json new_value = {
        {"link", repository_link},
        {"name", name}
    };

    j = j.at(0);

    for (auto &[curr_category, curr_value] : j.items()) { 
        if (curr_value.is_null()) {
            continue;
        }

        for (auto &[_, value] : curr_value.items()) {
            if (value["name"] == name) {
                std::cerr << colors::red << "You already have this repository!" << std::endl;

                std::exit(2);
            }
        }
    }

    if (j[category].is_null()) {
        j[category] = std::vector<nlohmann::json>({new_value});
    } else {
        j[category].push_back(new_value);

        std::sort(j[category].begin(), j[category].end());
    }

    f.seekp(0);
    f.clear();

    f << std::setw(4) << j;
}

void store::print() {
    std::fstream f;

    f.open(".prm.json", std::ios::in);

    if (!f.is_open()) {
        std::cerr << colors::red << "Can't open the file" << std::endl;

        std::exit(1);
    }

    nlohmann::json j {nlohmann::json::parse(f)};

    j = j.at(0);

    for (auto &[curr_category, curr_value] : j.items()) {
        std::cout << colors::cyan << curr_category << ":" << std::endl;

        if (curr_value.is_null()) {
            std::cout << colors::red << "\tEmpty category" << std::endl;

            continue;
        }

        for (auto &[_, value] : curr_value.items()) {
            std::cout << colors::white << '\t' << value["name"].get<std::string>() << std::endl;
        }
    }
}

void store::clone(std::string &name) {
    std::fstream f;

    f.open(".prm.json", std::ios::in);

    if (!f.is_open()) {
        std::cerr << colors::red << "Can't open the file" << std::endl;

        std::exit(1);
    }

    nlohmann::json j {nlohmann::json::parse(f)};

    j = j.at(0);

    for (auto &[curr_category, curr_value] : j.items()) {
        if (curr_value.is_null()) {
            continue;
        }

        for (auto &[_, value] : curr_value.items()) {
            if (value["name"] == name) {
                std::stringstream ss;

                ss << "git clone " << value["link"].get<std::string>();

                std::system(ss.str().c_str());

                return;
            }
        }
    }

    std::cerr << colors::red << "Can't find the given repository" << std::endl;

    std::exit(2);
}