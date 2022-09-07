cmake_minimum_required(VERSION 3.18)

project(sfun VERSION 2.3.0 DESCRIPTION "stuff from unnamed namespaces - a c++17 helper library")

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(SFUN_SUBPROJECT OFF)
else()
    set(SFUN_SUBPROJECT ON)
endif()


add_library(sfun INTERFACE)
add_library(sfun::sfun ALIAS sfun)

target_compile_features(sfun INTERFACE cxx_std_17)
target_include_directories(
        sfun
        INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set(PUBLIC_HEADERS
    "include/sfun/string_utils.h"
    "include/sfun/traits.h"
)
set_target_properties(sfun PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

option(ENABLE_TESTS "Enable tests" OFF)
if (ENABLE_TESTS AND NOT SFUN_SUBPROJECT)
    enable_testing()
    add_subdirectory(tests)
endif()

option(INSTALL_SFUN "Install sfun library unconditionally" OFF)
if (NOT SFUN_SUBPROJECT OR INSTALL_SFUN)
    include(GNUInstallDirs)

    set(SFUN_PACK_PATH ${CMAKE_INSTALL_LIBDIR}/cmake/sfun)
    install(TARGETS sfun
            EXPORT sfun-targets
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sfun)
    install(EXPORT sfun-targets
            FILE sfunTargets.cmake
            NAMESPACE sfun::
            DESTINATION ${SFUN_PACK_PATH})

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
            ${CMAKE_CURRENT_BINARY_DIR}/sfunConfigVersion.cmake
            COMPATIBILITY SameMajorVersion
            ARCH_INDEPENDENT
    )

    configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/sfunConfig.cmake.in
            ${CMAKE_CURRENT_BINARY_DIR}/sfunConfig.cmake
            INSTALL_DESTINATION ${SFUN_PACK_PATH}
    )

    install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/sfunConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/sfunConfigVersion.cmake
            DESTINATION ${SFUN_PACK_PATH}
    )
endif()
