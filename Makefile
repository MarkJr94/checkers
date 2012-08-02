COMMONFILES:=checkers.cpp checkers.hpp game.hpp game.cpp
COMMONSRC:=checkers.cpp game.cpp
CPPFLAGS:=-Wall -g -std=c++0x
ALLEGROFLAGS:=`allegro-config --libs`
GTKFLAGS:=`pkg-config --cflags --libs gtkmm-3.0`
cxx:=clang++
bstfiles:=bst.cpp bst.hpp
dgfiles:=DrawGame.cpp DrawGame.hpp
gwfiles:=GameWindow.hpp GameWindow.cpp

all: $(COMMONFILES) $(bstfiles) $(dgfiles) main.cpp $(gwfiles)
	 $(cxx) -o draughts $(CPPFLAGS) $(COMMONSRC) bst.cpp main.cpp
	 $(cxx)  -o visual $(CPPFLAGS) $(COMMONSRC) bst.cpp DrawGame.cpp GameWindow.cpp $(GTKFLAGS) $(ALLEGROFLAGS)

draughts: $(COMMONFILES) $(bstfiles) main.cpp
	$(cxx) -o $@ $(CPPFLAGS) $(COMMONSRC) bst.cpp main.cpp
	
visual: $(COMMONFILES) $(bstfiles) $(dgfiles) $(gwfiles)
	$(cxx) -o $@ $(CPPFLAGS) $(COMMONSRC) bst.cpp DrawGame.cpp GameWindow.cpp $(GTKFLAGS) $(ALLEGROFLAGS)

clean:
	rm draughts visual
