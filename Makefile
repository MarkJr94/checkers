COMMONFILES:=checkers.cpp checkers.hpp player.cpp player.hpp game.hpp game.cpp
COMMONSRC:=checkers.cpp player.cpp game.cpp
CPPFLAGS:=-Wall -g -std=c++0x
ALLEGROFLAGS:=`pkg-config --cflags --libs gtkmm-3.0` `allegro-config --libs`
GTKFLAGS:=`pkg-config --cflags --libs gtkmm-3.0`
cxx:=clang++

all: $(COMMONFILES) bst.cpp bst.hpp DrawGame.hpp DrawGame.cpp main.cpp GameWindow.hpp GameWindow.cpp
	 $(cxx) -o draughts $(CPPFLAGS) $(COMMONSRC) bst.cpp main.cpp
	 $(cxx)  -o visual $(CPPFLAGS) $(COMMONSRC) bst.cpp DrawGame.cpp GameWindow.cpp $(GTKFLAGS) $(ALLEGROFLAGS)

draughts: $(COMMONFILES) bst.cpp bst.hpp main.cpp
	$(cxx) -o $@ $(CPPFLAGS) $(COMMONSRC) bst.cpp main.cpp
	
visual: $(COMMONFILES) bst.cpp bst.hpp DrawGame.hpp DrawGame.cpp GameWindow.hpp GameWindow.cpp
	$(cxx) -o $@ $(CPPFLAGS) $(COMMONSRC) bst.cpp DrawGame.cpp GameWindow.cpp $(GTKFLAGS) $(ALLEGROFLAGS)

clean:
	rm draughts visual