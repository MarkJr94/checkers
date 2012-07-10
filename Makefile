main: checkers.cpp checkers.hpp player.cpp player.hpp game.hpp game.cpp main.cpp
	 g++ -Wall -g -std=c++0x checkers.cpp player.cpp game.cpp main.cpp -o draughts

tree: checkers.cpp checkers.hpp player.cpp player.hpp game.hpp game.cpp bst.cpp bst.hpp
	g++ -Wall -g -std=c++0x checkers.cpp player.cpp bst.cpp -o trees


