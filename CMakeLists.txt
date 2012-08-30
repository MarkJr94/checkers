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

add_executable(draughts Piece.cpp Save.cpp Game.cpp AI.cpp main.cpp)
target_link_libraries(draughts ${CORELIBS})

#add_executable(visual Piece.cpp Game.cpp bst.cpp DrawGame.cpp GameWindow.cpp)
#target_link_libraries(visual ${CORELIBS} ${GLIBMM_LIBRARIES} ${GTKMM_LIBRARIES} ${SFML_LIBS} )

add_executable(new Piece.cpp Save.cpp  Game.cpp DrawGame.cpp AI.cpp new.cpp)
target_link_libraries(new ${CORELIBS} ${SFML_LIBS} )
