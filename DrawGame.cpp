/*
 * DrawGame.cpp
 *
 *  Created on: Jul 22, 2012
 *      Author: markjr
 */

#include <iostream>

#include "bst.hpp"
#include "checkers.hpp"
#include "game.hpp"
#include "DrawGame.hpp"
#include "../ogl-tryouts/World.hpp"
#include "../ogl-tryouts/Shape.hpp"
#include "../ogl-tryouts/RectangularPrism.hpp"
#include "../ogl-tryouts/Box.hpp"
#include "../ogl-tryouts/Cylinder.hpp"

DrawGame::DrawGame() :
		Game(true, true), world() {
	setup_opengl(1366, 768);
//	const Color TAN = makeCol(0xff,0xda,0xb9);
	const Color BLACK = makeCol(0x0, 0xff, 0x0);
	const Color RED = makeCol(0xff, 0x0, 0x0);
	const Color SADDLEBROWN = makeCol(0x8b, 0x45, 0x13);
	Color col;
	unsigned counter = 0;
	world.add(new Box(0, -32, 0, 66, SADDLEBROWN, true));
	for (int i = -28; i <= 28; i += 8) {
		for (int j = -28; j <= 28; j += 8) {
			if (counter++ % 2)
				col = RED;
			else
				col = BLACK;
			world.add(new Box(i, -2, j, 8, col, true));
		}
	}
	world.prepare();
	_print();
}

DrawGame::DrawGame(const SaveGame& record) :
		Game(record, true, true) {
	_print();
}

void DrawGame::_print() {
	double d = .01;
	world.draw(d);

}

DrawGame::~DrawGame() {
	World::quit_opengl(0);
}

