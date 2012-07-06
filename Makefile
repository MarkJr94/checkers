all: checkers.cpp checkers.hpp player.cpp player.hpp main.cpp
	 g++ -Wall -g -std=c++0x checkers.cpp player.cpp main.cpp -o draughts

tree: checkers.cpp checkers.hpp player.cpp player.hpp bst.cpp bst.hpp
	g++ -Wall -g -std=c++0x checkers.cpp player.cpp bst.cpp -o trees
