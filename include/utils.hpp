#pragma once

#include <fstream>
#include <string>

namespace utils {
    std::fstream open_config_file(const std::ios_base::openmode mode);
    bool check_if_config_file_exists();
}