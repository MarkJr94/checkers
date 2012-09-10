/*
 * DrawGame.cpp
 *
 *  Created on: Jul 22, 2012
 *      Author: markjr
 */

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

#include "GameWin.hpp"

GameWin::GameWin(const int wide, const int high) :
		super(sf::VideoMode(wide, high, 32), "Game Window"), _game(NULL), _ai(), _board(
				64), _state(NORMAL) {

}

GameWin::~GameWin() {
	delete _game;
}

void GameWin::sfHandleEvents() {
	using sf::Event;
	using sf::Vector2;

	Event Event;
	while (GetEvent(Event)) {
		// Window closed
		switch (Event.Type) {
		case Event::Closed:
			Close();
			break;

		case Event::KeyPressed:
			if (Event.Key.Code == sf::Key::Escape) {
				Close();
			} else if (Event.Key.Code == sf::Key::Q) {
				_state = NORMAL;
			}
			break;

		case Event::MouseButtonPressed:
			if (Event.MouseButton.Button == sf::Mouse::Left) {
				switch (_state) {
				case NORMAL:
					_mDown1 = resolveMouse(
							Vector2<float>(Event.MouseButton.X,
									Event.MouseButton.Y));
					_state = HLIGHT_1;
					break;
				case HLIGHT_1:
					_mDown2 = resolveMouse(
							Vector2<float>(Event.MouseButton.X,
									Event.MouseButton.Y));
					_state = EVALUATING;
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}
	}
}

sf::Vector2<int> GameWin::resolveMouse(sf::Vector2<float> downVec) const {
	int x = floor(downVec.x);
	int y = floor(downVec.y);
	x -= x % 100;
	y -= y % 100;
	return {x,y};
}

void GameWin::drawCell(Cell c, int i, int j) {
	using sf::Shape;
	using sf::Color;

	Shape cell;
	const Color tan(0xd2, 0xb4, 0x8c);
	const Color darkBrown(0x9c, 0x66, 0x1f);
	const Color white(0xff, 0xff, 0xff);
	const Color black(0, 0, 0);

	int x = i;
	int y = 7 - j;

	int odd = y & 1;
	if (odd) {
		if (x & 1) {
			cell = Shape::Rectangle(x * cellSz, y * cellSz, (x + 1) * cellSz,
					(y + 1) * cellSz, tan);
			Draw(cell);
		} else {
			cell = Shape::Rectangle(x * cellSz, y * cellSz, (x + 1) * cellSz,
					(y + 1) * cellSz, darkBrown);
			Draw(cell);
		}
	} else {
		if (x & 1) {
			cell = Shape::Rectangle(x * cellSz, y * cellSz, (x + 1) * cellSz,
					(y + 1) * cellSz, darkBrown);
			Draw(cell);
		} else {
			cell = Shape::Rectangle(x * cellSz, y * cellSz, (x + 1) * cellSz,
					(y + 1) * cellSz, tan);
			Draw(cell);
		}
	}

	if (c == EMPTY)
		return;

	Shape piece;
	switch (c) {
	case P_B:
		piece = Shape::Circle(x * cellSz + 50, y * cellSz + 50, circRad, black);
		Draw(piece);
		break;
	case P_W:
		piece = Shape::Circle(x * cellSz + 50, y * cellSz + 50, circRad, white);
		Draw(piece);
		break;
	case K_B:
		piece = Shape::Circle(x * cellSz + 50, y * cellSz + 50, circRad, black,
				-5.f, Color::Cyan);
		Draw(piece);
		break;
	case K_W:
		piece = Shape::Circle(x * cellSz + 50, y * cellSz + 50, circRad, white,
				-5.f, Color::Cyan);
		Draw(piece);
		break;
	default:
		break;
	}
}

std::ostream& operator<<(std::ostream& os, const sf::Vector2<float> vec) {
	os << vec.x << " " << vec.y;

	return os;
}

std::ostream& operator<<(std::ostream& os, const Move m) {
	os << m.src << " => " << m.dst;

	return os;
}

void GameWin::drawGame() {
	using namespace std;
	using namespace sf;
	using sf::Shape;

	Clear();

	_board = _game->toArr();

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			drawCell(_board[i + j * 8], i, j);
		}
	}

	Shape cell;
	switch (_state) {
	case EVALUATING:
		cell = Shape::Rectangle(_mDown2.x, _mDown2.y, (_mDown2.x + 100),
				(_mDown2.y + 100), Color(0, 0, 0, 0), -4.f, sf::Color::Yellow);
		Draw(cell);
	case HLIGHT_1:
		cell = Shape::Rectangle(_mDown1.x, _mDown1.y, (_mDown1.x + 100),
				(_mDown1.y + 100), Color(0, 0, 0, 0), -4.f, sf::Color::Yellow);
		Draw(cell);
		break;
	default:
		break;
	}

	Display();
//	std::cout << _mDown1 << " " << _state << std::endl;
}

MoveCode GameWin::evalSelections() {
	using sf::Vector2i;

	int x = _mDown1.x / 100, y = 7 - _mDown1.y / 100;
	unsigned short src;

	int odd = y & 1;
	src = y * 8 + x;
	if (odd) {
		if (!(src & 1))
			return VOID_PIECE;
		src = (src - 1) / 2;
	} else {
		if (src & 1)
			return VOID_PIECE;
		src = src / 2;
	}

	x = _mDown2.x / 100;
	y = 7 - _mDown2.y / 100;
	unsigned short dst;

	odd = y & 1;
	dst = y * 8 + x;
	if (odd) {
		if (!(dst & 1))
			return VOID_PIECE;
		dst = (dst - 1) / 2;
	} else {
		if (dst & 1)
			return VOID_PIECE;
		dst /= 2;
	}

	_board = _game->toArr();
	Move m = { src, dst };
	std::cout << "Player Move: " << m << std::endl;
	if (_board[x + y * 8] == EMPTY) {
		return _game->makeMove(m);
	} else {
		return _game->jump(m);
	}
}

void GameWin::loop() {
	using std::cout;
	using std::endl;

	while (IsOpened() && _game->isLive()) {
		if (_state == EVALUATING) {
			MoveCode err;

			err = evalSelections();
			std::cout << _errtable[err] << std::endl;
			if (_game->_turn) {
				_state = NORMAL;
				continue;
			}
//		} while ((_game->_turn) && err != SUCCESS);
//			if (err == SUCCESS) {
			std::pair<Move, bool> moveinfo = _ai.evaluateGame(*_game);
			cout << "AI Move: " << moveinfo.first << endl;
			if (moveinfo.first.dst == 0 && moveinfo.first.src == 0) break;
			if (moveinfo.second) {
				cout << _errtable[_game->jump(moveinfo.first)] << endl;
			} else
				cout << _errtable[_game->makeMove(moveinfo.first)] << endl;
//			}
			if (_game->_turn)
				_state = NORMAL;
		}
//		std::pair<Move, bool> moveinfo = _ai.evaluateGame(*_game);
//		cout << "Turn: " << _game->_turn << " AI Move: " << moveinfo.first << endl;
//		if (moveinfo.second) {
//			cout << _errtable[_game->jump(moveinfo.first)] << endl;
//		} else
//			cout << _errtable[_game->makeMove(moveinfo.first)] << endl;
		sfHandleEvents();
		drawGame();
	}

	if (IsOpened()) Close();
}
