cmake_minimum_required(VERSION 2.6)
project(CHECKERS)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0) # look into FindPkgConfig.cmake,
pkg_check_modules(GLIBMM glibmm-2.4)


set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++0x")
set(CORELIBS liboptparser.so)
set(SFML_LIBS sfml-graphics sfml-window sfml-system)
include_directories( ${GTKMM_INCLUDE_DIRS} ${GLIBMM_INCLUDE_DIRS} )

add_executable(game_test Game.cpp Game_test.cpp Save.cpp BitBoard.cpp)
target_link_libraries(game_test ${CORELIBS}) #Hash.cpp TranspositionTable.cpp

add_executable(ai_test Game.cpp Save.cpp BitBoard.cpp AI.cpp AI_test.cpp)
target_link_libraries(game_test ${CORELIBS}) #Hash.cpp TranspositionTable.cpp