#include <utils.hpp>
#include <exit.hpp>
#include <colors.hpp>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
    const std::string CONFIG_FILE_PATH {std::string(std::getenv("APPDATA")) + "\\.prm.json"};
#elif defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
    const std::string CONFIG_FILE_PATH {std::string(std::getenv("HOME")) + "/.config/.prm.json"};
#endif

bool utils::check_if_config_file_exists() {
    std::fstream f;

    f.open(CONFIG_FILE_PATH, std::ios::in);

    if (f.is_open()) {
        return true;
    }

    return false;
}

std::fstream utils::open_config_file(const std::ios_base::openmode mode) {
    std::fstream f;

    f.open(CONFIG_FILE_PATH, mode);

    if (!f.is_open()) {
        std::cerr << colors::red << "Can not open the PRM's storage file" << colors::reset << std::endl;

        std::exit(EXIT_BY_PROGRAM_ERROR);
    }

    return f;
}