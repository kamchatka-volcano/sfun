cmake_minimum_required(VERSION 3.18)
project(sfun VERSION 2.4.0 DESCRIPTION "stuff from unnamed namespaces - a c++17 helper library")
include(GNUInstallDirs)
include(CTest)
include(external/seal_lake)

SealLake_HeaderOnlyLibrary(
        COMPILE_FEATURES cxx_std_17
)
SealLake_OptionalBuildSteps(tests)

