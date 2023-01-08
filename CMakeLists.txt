cmake_minimum_required(VERSION 3.22.1)

project(prm)

set(CMAKE_CXX_STANDARD 20)
set(JSON_BuildTests OFF CACHE INTERNAL "")

add_compile_options(-Wall -Wextra -Werror)

set(TARGET_LINK_LIBRARIES "CLI11" "nlohmann_json::nlohmann_json")
set(TARGET_EXECUTABLES "src/main.cpp" "src/main.cpp" "src/commands.cpp" "src/store.cpp" "src/setup.cpp" "src/utils.cpp")
set(SUBDIRECTORIES "extern/CLI11" "extern/json")

set(CLI11_TESTING OFF)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/extern/colors.hpp/include/colors)

foreach(SUBDIRECTORY IN LISTS SUBDIRECTORIES)
    add_subdirectory(${SUBDIRECTORY})
endforeach()

add_executable(${PROJECT_NAME} ${TARGET_EXECUTABLES})

target_link_libraries(${PROJECT_NAME} ${TARGET_LINK_LIBRARIES})