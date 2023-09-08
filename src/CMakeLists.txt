add_executable(server_start main.cpp sha256.cpp database.cpp)

target_include_directories(server_start PUBLIC ${CMAKE_HOME_DIRECTORY}/include
											   include
											   ${CMAKE_HOME_DIRECTORY}/libs/SQLite3/include
											   ${CMAKE_HOME_DIRECTORY}/libs/spdlog/include)

target_link_libraries(server_start PRIVATE sqlite3 spdlog)