cmake_minimum_required(VERSION 3.20)
project(MyProject1)
add_executable(MyProject1 main.cpp)
add_subdirectory(./libpqxx libpqxx-build) # ������� ���� � libpq++
target_compile_features(MyProject1 PRIVATE cxx_std_17) # �������� C++17
target_link_libraries(MyProject1 pqxx)