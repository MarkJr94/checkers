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
		Game(true, true) {
}

DrawGame::DrawGame(const SaveGame& record) :
		Game(record, true, true) {
	_print();
}

void DrawGame::_print() {
	double d = .01;

}

DrawGame::~DrawGame() {
}

