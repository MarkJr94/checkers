cmake_minimum_required(VERSION 2.6)
project(CHECKERS)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0) # look into FindPkgConfig.cmake,
pkg_check_modules(GLIBMM glibmm-2.4)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++0x")
set(CORELIBS World ${GLIBMM_LIBRARIES} ${GTKMM_LIBRARIES})
include_directories( ${GTKMM_INCLUDE_DIRS} ${GLIBMM_INCLUDE_DIRS} )
#link_directories( 

add_executable(draughts checkers.cpp game.cpp bst.cpp main.cpp)
target_link_libraries(draughts ${CORELIBS} libargparse.so)

add_executable(visual checkers.cpp game.cpp bst.cpp DrawGame.cpp GameWindow.cpp)
target_link_libraries(visual ${CORELIBS}  libargparse.so libWorld.so)