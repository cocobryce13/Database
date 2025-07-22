cmake_minimum_required(VERSION 3.20)
project(BookORM)

add_executable(BookORM src/main.cpp)

find_package(Wt REQUIRED COMPONENTS Dbo)

target_compile_features(BookORM PRIVATE cxx_std_17)
target_link_libraries(BookORM PRIVATE Wt::Dbo Wt::DboPostgres)