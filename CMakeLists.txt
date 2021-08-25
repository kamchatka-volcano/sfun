cmake_minimum_required(VERSION 3.18)

if(NOT DEFINED SFUN_SUBPROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(SFUN_SUBPROJECT OFF)
    else()
        set(SFUN_SUBPROJECT ON)
    endif()
endif()

project(sfun DESCRIPTION "stuff from unnamed namespaces")

add_library(sfun INTERFACE)
target_compile_features(sfun INTERFACE cxx_std_11)
target_include_directories(
        sfun
        INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

option(ENABLE_TESTS "Enable tests" OFF)
if (ENABLE_TESTS AND NOT SFUN_SUBPROJECT)
    enable_testing()
    add_subdirectory(tests)
endif()

include(GNUInstallDirs)
if(NOT SFUN_SUBPROJECT)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/sfun DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()
