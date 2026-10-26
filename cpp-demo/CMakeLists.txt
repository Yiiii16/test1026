cmake_minimum_required(VERSION 3.16)
project(CppDemo LANGUAGES CXX)

add_executable(CppDemo main.cpp Calculator.cpp Calculator.h)
target_compile_features(CppDemo PRIVATE cxx_std_11)
