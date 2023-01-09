cmake_minimum_required(VERSION 3.18)
project(test_sfun)

SealLake_GoogleTest(
        SOURCES
        test_string.cpp
        test_traits.cpp
        test_type_list.cpp
        test_functional.cpp
        test_utility.cpp
        LIBRARIES
        sfun::sfun
)