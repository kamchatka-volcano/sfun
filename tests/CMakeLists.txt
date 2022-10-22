cmake_minimum_required(VERSION 3.18)
project(test_sfun)

SealLake_GoogleTest(
        SOURCES
            test_string.cpp
            test_traits.cpp
        LIBRARIES
            sfun::sfun
)