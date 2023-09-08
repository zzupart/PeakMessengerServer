cmake_minimum_required (VERSION 3.13)

set(CMAKE_CXX_STANDART_REQUIRED 20)

project ("PeakMessengerServer")

add_subdirectory ("src")
add_subdirectory("tests")
add_subdirectory ("libs/SQLite3")
add_subdirectory ("libs/spdlog")