cmake_minimum_required(VERSION 3.22.1)

project(prm)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Werror)

set(TARGET_LINK_LIBRARIES "CLI11")
set(SUBDIRECTORIES "extern/CLI11")
set(INCLUDE_DIRECTORIES "extern/CLI11")

set(CLI11_TESTING OFF)

include_directories(${INCLUDE_DIRECTORIES})

foreach(SUBDIRECTORIE IN LISTS SUBDIRECTORIES)
    add_subdirectory(${SUBDIRECTORIE})
endforeach()

add_executable(${PROJECT_NAME} src/main.cpp)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} ${TARGET_LINK_LIBRARIES})
endif (UNIX)