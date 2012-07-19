all: checkers.cpp checkers.hpp player.cpp player.hpp game.hpp game.cpp bst.cpp bst.hpp main.cpp
	 g++ -Wall -g -std=c++0x -fopenmp checkers.cpp player.cpp game.cpp bst.cpp main.cpp -o draughts

draughts: checkers.cpp checkers.hpp player.cpp player.hpp game.hpp game.cpp bst.cpp bst.hpp main.cpp
	g++ -Wall -g -std=c++0x checkers.cpp player.cpp game.cpp bst.cpp main.cpp -o draughts

clean:
	rm draughts