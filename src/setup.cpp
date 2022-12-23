#include <setup.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include <colors.hpp>

void setup::init() {
    std::fstream f;

    f.open(".prm.json", std::ios::in);

    if (f.is_open()) {
        return;
    }

    nlohmann::json j = {
        {"created", nlohmann::json::array()},
        {"idle", nlohmann::json::array()},
        {"working", nlohmann::json::array()},
        {"finished", nlohmann::json::array()},
        {"others",nlohmann::json::array()}
    };

    f.open(".prm.json", std::ios::out);

    if (f.is_open()) {
        f << std::setw(4) << j;

        return;
    }

    std::cerr << colors::red << "Can't open the file" << std::endl;

    std::exit(1);
}