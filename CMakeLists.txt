cmake_minimum_required(VERSION 3.20)
project(labwork11)

set(CMAKE_CXX_STANDARD 20)

add_executable(BuildIndex
        BuildIndex/bin/main.cpp
        BuildIndex/lib/Spimi/Spimi.h
        BuildIndex/lib/Spimi/Spimi.cpp
        Search/lib/Parser/Parser.cpp
)
add_executable(Searcher
        Search/bin/main.cpp
        Search/lib/Parser/Parser.h
        Search/lib/Parser/Parser.cpp
        Search/lib/Terms/Terms.h
        Search/lib/Terms/Terms.cpp
        Search/lib/Rank/Rank.cpp
        Search/lib/Rank/Rank.h
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "-Wpedantic")

#target_include_directories(labwork11 PRIVATE bin/ lib/)

#enable_testing()
#add_subdirectory(tests)