COMMONFILES:=checkers.cpp checkers.hpp player.cpp player.hpp game.hpp game.cpp
COMMONSRC:=checkers.cpp player.cpp game.cpp
CPPFLAGS:=-Wall -g -std=c++0x
ALLEGROFLAGS:=`pkg-config --cflags --libs gtkmm-3.0` `allegro-config --libs`

all: $(COMMONFILES) bst.cpp bst.hpp DrawGame.hpp DrawGame.cpp main.cpp
	 g++ -o draughts $(CPPFLAGS) $(COMMONSRC) bst.cpp main.cpp
	 g++  -o visual $(CPPFLAGS) $(COMMONSRC) bst.cpp DrawGame.cpp $(ALLEGROFLAGS)

draughts: $(COMMONFILES) bst.cpp bst.hpp main.cpp
	g++ -o $@ $(CPPFLAGS) $(COMMONSRC) bst.cpp main.cpp
	
visual: $(COMMONFILES) bst.cpp bst.hpp DrawGame.hpp DrawGame.cpp
	g++ -o $@ $(CPPFLAGS) $(COMMONSRC) bst.cpp DrawGame.cpp $(ALLEGROFLAGS)

clean:
	rm draughts visual