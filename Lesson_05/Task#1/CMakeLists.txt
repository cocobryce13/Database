cmake_minimum_required(VERSION 3.20)
project(ClientManager)

add_executable(ClientManager src/main.cpp)

add_subdirectory(libpqxx libpqxx-build)

target_compile_features(ClientManager PRIVATE cxx_std_17)

target_link_libraries(ClientManager pqxx)