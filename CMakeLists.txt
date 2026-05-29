cmake_minimum_required(VERSION 3.20)

project(MatchingEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(MatchingEngine
    src/main.cpp
 "src/OrderBook.cpp" "src/MatchingEngine.cpp")

target_include_directories(MatchingEngine
    PRIVATE
        include
)