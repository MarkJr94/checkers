chk_f:=checkers.cpp checkers.hpp
chk_o:=objs/checkers.o

game_f:=game.hpp game.cpp
game_o:=objs/game.o

bst_f:=bst.cpp bst.hpp
bst_o:=objs/bst.o

main_f:=main.cpp
main_o:=objs/main.o

dg_f:=DrawGame.cpp DrawGame.hpp
dg_o:=objs/DrawGame.o

gw_f:=GameWindow.hpp GameWindow.cpp
gw_o:=objs/GameWindow.o
gw_i:=`pkg-config --cflags gtkmm-3.0`

cmdobjs:=$(chk_o) $(game_o) $(bst_o) $(main_o)
guiobjs:=$(chk_o) $(game_o) $(bst_o) $(dg_o) $(gw_o)

cxx:=clang++
cppflags:=-Wall -g -std=c++0x
links:=`allegro-config --libs` `pkg-config --libs gtkmm-3.0`

all: draughts visual

$(chk_o): $(chk_f)
	$(cxx) -o $@ $(cppflags) -c checkers.cpp
	
$(game_o): $(game_f)
	$(cxx) -o $@ $(cppflags) -c game.cpp
	
$(bst_o): $(bst_f)
	$(cxx) -o $@ $(cppflags) -c bst.cpp
	
$(main_o): $(main_f)
	$(cxx) -o $@ $(cppflags) -c main.cpp
	
$(dg_o): $(dg_f)
	$(cxx) -o $@ $(cppflags) -c DrawGame.cpp
	
$(gw_o): $(gw_f)
	$(cxx) -o $@ $(cppflags) -c GameWindow.cpp $(gw_i)

draughts: $(cmdobjs)
	$(cxx) -o $@ $(cppflags) $(cmdobjs)
	
visual: $(guiobjs)
	$(cxx) -o $@ $(cppflags) $(guiobjs) $(links)

clean:
	rm draughts visual objs/*.o
