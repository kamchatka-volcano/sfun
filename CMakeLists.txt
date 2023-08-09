cmake_minimum_required(VERSION 3.18)
project(sfun VERSION 4.1.0 DESCRIPTION "stuff from unnamed namespaces - a c++17 helper library")
include(GNUInstallDirs)
include(external/seal_lake)

SealLake_Bundle(
        NAME sfun-utfcpp
        GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git
        GIT_TAG v3.2.3
        SKIP_LOAD
        DESTINATION include/sfun/detail/external
        FILES source/utf8.h
        DIRECTORIES source/utf8
        TEXT_REPLACEMENTS
        "namespace utf8" "namespace sfun::utf8"
        UTF8_ SFUN_UTF8_
)

SealLake_HeaderOnlyLibrary(
        COMPILE_FEATURES cxx_std_17
)
SealLake_OptionalBuildSteps(tests)

