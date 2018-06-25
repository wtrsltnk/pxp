cmake_minimum_required(VERSION 3.7)

project(pxp)

list(APPEND CMAKE_PREFIX_PATH "$ENV{userprofile}/.cpppm/install")

find_package(system.io REQUIRED)
find_package(system.xml REQUIRED)

add_executable(pxp
    src/program.cpp
    )

target_link_libraries(pxp
    PUBLIC
        system.io
        system.xml
    )

target_compile_features(pxp
    PRIVATE cxx_auto_type
    PRIVATE cxx_nullptr
    PRIVATE cxx_range_for
    )
