#include <setup.hpp>
#include <utils.hpp>
#include <store.hpp>
#include <nlohmann/json.hpp>
#include <colors.hpp>
#include <fstream>

void setup::init() {
    if (utils::check_if_config_file_exists()) {
        return;
    }

    nlohmann::json j;

    for (const std::string &category : store::VALID_CATEGORIES) {
        j[category] = nlohmann::json::array();
    }

    std::fstream f {utils::open_config_file(std::ios::out)};

    f << std::setw(4) << j;
}