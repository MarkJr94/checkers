cmake_minimum_required(VERSION 2.6)
project(CHECKERS)

find_package(PkgConfig)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++0x")
set(CORELIBS liboptparser.so)
set(SFML_LIBS sfml-graphics sfml-window sfml-system)

add_executable(game_test Game.cpp Game_test.cpp Save.cpp BitBoard.cpp)
target_link_libraries(game_test ${CORELIBS}) #Hash.cpp TranspositionTable.cpp

add_executable(ai_test Game.cpp Save.cpp BitBoard.cpp AI.cpp AI_test.cpp)
target_link_libraries(game_test ${CORELIBS}) #Hash.cpp TranspositionTable.cpp

add_executable(win_test BitBoard.cpp Game.cpp Save.cpp AI.cpp GameWin.cpp GameWin_test.cpp)
target_link_libraries(win_test ${CORELIBS} ${SFML_LIBS})