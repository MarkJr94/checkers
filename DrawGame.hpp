/*
 * DrawGame.h
 *
 *  Created on: Jul 22, 2012
 *      Author: markjr
 */

#ifndef DRAWGAME_H_
#define DRAWGAME_H_

#include "game.hpp"

const unsigned CELL_SIZE = 30;

class DrawGame: public Game
{
	void _print() const;

	/* Typedef for super class  */
	typedef Game super;
public:
	DrawGame();
	DrawGame(const SaveGame& record);
	virtual ~DrawGame();
};

#endif /* DRAWGAME_H_ */
