add_executable(server_test test.cpp)

target_include_directories(server_test PUBLIC include ${CMAKE_HOME_DIRECTORY}/include)

find_library(GTEST_LIBRARY REQUIRED NAMES gtest)

target_link_libraries(server_test PRIVATE gtest)