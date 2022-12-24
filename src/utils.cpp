#include <utils.hpp>
#include <colors.hpp>
#include <iostream>

const std::string config_file_path {".prm.json"};

bool utils::check_if_config_file_exists() {
    std::fstream f;

    f.open(config_file_path, std::ios::in);

    if (f.is_open()) {
        return true;
    }

    return false;
}

std::fstream utils::open_config_file(const std::ios_base::openmode mode) {
    std::fstream f;

    f.open(config_file_path, mode);

    if (!f.is_open()) {
        std::cerr << colors::red << "Can't open the configuration file" << std::endl;

        std::exit(1);
    }

    return f;
}