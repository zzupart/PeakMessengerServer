add_executable(server_start main.cpp sha256.cpp)

target_include_directories(server_start PUBLIC ${CMAKE_HOME_DIRECTORY}/include
											   include
											   ${CMAKE_HOME_DIRECTORY}/libs/SQLite3/include)