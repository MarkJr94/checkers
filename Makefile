all: checkers.cpp checkers.hpp player.cpp player.hpp
	 g++ -Wall -g -std=c++0x checkers.cpp player.cpp -o draughts
