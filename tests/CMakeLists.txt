cmake_minimum_required(VERSION 3.18)
project(test_sfun)

SealLake_GoogleTest(
        SOURCES
        test_optional_ref.cpp
        test_optional_cpref.cpp
        test_precondition.cpp
        test_string.cpp
        test_traits.cpp
        test_type_list.cpp
        test_functional.cpp
        test_utility.cpp
        test_member.cpp
        test_indirect_member.cpp
        LIBRARIES
        sfun::sfun
)