/*
 * DrawGame.cpp
 *
 *  Created on: Jul 22, 2012
 *      Author: markjr
 */

#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

#include "SFMLGame.hpp"

SFMLGame::SFMLGame(const int wide, const int high) :
				supersf(sf::VideoMode(wide, high, 32), "Game Window",
						sf::Style::Titlebar | sf::Style::Close),
				mGame(false, false),
				mAi(14),
				mBoard(64),
				mState(NORMAL),
				CIRCRAD(40),
				CELLSZ(100)
{
	set_has_window(false);

	add_events(Gdk::BUTTON_PRESS_MASK);
	add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK);

	Glib::signal_idle().connect(sigc::mem_fun(*this, &SFMLGame::on_idle));
}

SFMLGame::SFMLGame(const sf::VideoMode& mode) :
				supersf(mode, "Game Window",
						sf::Style::Titlebar | sf::Style::Close),
				mGame(false, false),
				mAi(14),
				mBoard(64),
				mState(NORMAL),
				CIRCRAD(40),
				CELLSZ(100)
{
	set_has_window(false);

	add_events(Gdk::BUTTON_PRESS_MASK);

	Glib::signal_idle().connect(sigc::mem_fun(*this, &SFMLGame::on_idle));
}

SFMLGame::~SFMLGame()
{
	Close();
}

/* ======================================================================= */
/* ======================================================================= */
/* ======================================================================= */
/* ===================== SFML Code Section =============================== */
/* ======================================================================= */
/* ======================================================================= */
/* ======================================================================= */

sf::Vector2<int> SFMLGame::resolveMouse(sf::Vector2<int> downVec) const
{
	int x = downVec.x;
	int y = downVec.y;
	x -= x % CELLSZ;
	y -= y % CELLSZ;
	return {x,y};
}

void SFMLGame::drawCell(Cell c, int i, int j)
{
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
			cell = Shape::Rectangle(x * CELLSZ, y * CELLSZ, (x + 1) * CELLSZ,
					(y + 1) * CELLSZ, tan);
			Draw(cell);
		} else {
			cell = Shape::Rectangle(x * CELLSZ, y * CELLSZ, (x + 1) * CELLSZ,
					(y + 1) * CELLSZ, darkBrown);
			Draw(cell);
		}
	} else {
		if (x & 1) {
			cell = Shape::Rectangle(x * CELLSZ, y * CELLSZ, (x + 1) * CELLSZ,
					(y + 1) * CELLSZ, darkBrown);
			Draw(cell);
		} else {
			cell = Shape::Rectangle(x * CELLSZ, y * CELLSZ, (x + 1) * CELLSZ,
					(y + 1) * CELLSZ, tan);
			Draw(cell);
		}
	}

	if (c == EMPTY)
		return;

	Shape piece;
	const int hCellSz = CELLSZ / 2;
	switch (c) {
	case P_B:
		piece = Shape::Circle(x * CELLSZ + hCellSz, y * CELLSZ + hCellSz,
				CIRCRAD, black);
		Draw(piece);
		break;
	case P_W:
		piece = Shape::Circle(x * CELLSZ + hCellSz, y * CELLSZ + hCellSz,
				CIRCRAD, white);
		Draw(piece);
		break;
	case K_B:
		piece = Shape::Circle(x * CELLSZ + hCellSz, y * CELLSZ + hCellSz,
				CIRCRAD, black, -5.f, Color::Cyan);
		Draw(piece);
		break;
	case K_W:
		piece = Shape::Circle(x * CELLSZ + hCellSz, y * CELLSZ + hCellSz,
				CIRCRAD, white, -5.f, Color::Cyan);
		Draw(piece);
		break;
	default:
		break;
	}
}

std::ostream& operator<<(std::ostream& os, const sf::Vector2<float> vec)
{
	os << vec.x << " " << vec.y;

	return os;
}

std::ostream& operator<<(std::ostream& os, const Move m)
{
	os << m.src << " => " << m.dst << "[" << (m.jump ? "jump" : "move") << "]";

	return os;
}

void SFMLGame::update()
{
	using sf::Shape;
	using sf::Color;
	using std::min;
	using std::max;
	using std::cout;
	using std::endl;

	if (mGame.isLive()) {
		do {
			if (mState == PLAYER_MOVE) {
				MoveCode err;

				err = evalSelections();
				std::cout << errorTable[err] << std::endl;
				if (mGame.mTurn) {
					mState = NORMAL;
					break;
				} else {
					mState = AI_MOVE;
					break;
				}

			} else if (mState == AI_MOVE) {
				Move move = mAi.evaluate_game(mGame);
				cout << "AI Move: " << move << endl;

				if (move.dst == 0 && move.src == 0)
					break;

				cout << errorTable[mGame.move(move)] << endl;

				if (mGame.mTurn)
					mState = NORMAL;
			}
		} while (0);
	}

	Clear();

	mBoard = mGame.toArr();

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			drawCell(mBoard[i + j * 8], i, j);
		}
	}

	Shape cell;
	switch (mState) {
	case PLAYER_MOVE:
		cell = Shape::Rectangle(mDown2.x, mDown2.y, (mDown2.x + CELLSZ),
				(mDown2.y + CELLSZ), Color(0, 0, 0, 0), -4.f,
				sf::Color::Yellow);
		Draw(cell);
	case HLIGHT_1:
		cell = Shape::Rectangle(mDown1.x, mDown1.y, (mDown1.x + CELLSZ),
				(mDown1.y + CELLSZ), Color(0, 0, 0, 0), -4.f,
				sf::Color::Yellow);
		Draw(cell);
		break;
	default:
		break;
	}
}

MoveCode SFMLGame::evalSelections()
{
	using sf::Vector2i;

	int x = mDown1.x / CELLSZ, y = 7 - mDown1.y / CELLSZ;
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

	x = mDown2.x / CELLSZ;
	y = 7 - mDown2.y / CELLSZ;
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

	mBoard = mGame.toArr();
	Move m = { src, dst, false };
	if (mBoard[x + y * 8] != EMPTY) m.jump = true;
	std::cout << "Player Move: " << m << std::endl;
	return mGame.move(m);
//	if (mBoard[x + y * 8] == EMPTY) {
//		return mGame.makeMove(m);
//	} else {
//		return mGame.jump(m);
//	}
}

void SFMLGame::loop()
{
	using std::cout;
	using std::endl;

	while (IsOpened() && mGame.isLive()) {
		if (mState == PLAYER_MOVE) {
			MoveCode err;

			err = evalSelections();
			std::cout << errorTable[err] << std::endl;
			if (mGame.mTurn) {
				mState = NORMAL;
				continue;
			}
			Move move = mAi.evaluate_game(mGame);
			cout << "AI Move: " << move << endl;

			if (move.dst == 0 && move.src == 0)
				break;

			cout << errorTable[mGame.move(move)] << endl;

			if (mGame.mTurn)
				mState = NORMAL;
		}
		update();
		Display();
	}

	if (IsOpened())
		Close();
}

/* ======================================================================= */
/* ======================================================================= */
/* ======================================================================= */
/* ======================= Gtk Code Section ============================== */
/* ======================================================================= */
/* ======================================================================= */
/* ======================================================================= */

bool SFMLGame::on_idle()
{
	if (mGdkWindow) {
		update();
		Display();
	}

	return true;
}

void SFMLGame::on_size_request(Gtk::Requisition* requisition)
{
	*requisition = Gtk::Requisition();

	requisition->width = GetWidth();
	requisition->height = GetHeight();
}

void SFMLGame::on_size_allocate(Gtk::Allocation& allocation)
{
	//Do something with the space that we have actually been given:
	//(We will not be given heights or widths less than we have requested, though
	//we might get more)

	this->set_allocation(allocation);

	if (mGdkWindow) {
//        _gdkWindow->move_resize(allocation.get_x(), allocation.get_y(), allocation.get_width(), allocation.get_height() );
		mGdkWindow->move(allocation.get_x(), allocation.get_y());
//        _sfRenWin->SetSize(allocation.get_width(), allocation.get_height());
//        _sfRenWin->SetPosition(allocation.get_x(),allocation.gset_y());
	}
}

void SFMLGame::on_map()
{
	supergtk::on_map();
}

void SFMLGame::on_unmap()
{
	supergtk::on_unmap();
}

void SFMLGame::on_realize()
{
	supergtk::on_realize();

	if (!mGdkWindow) {
		//Create the GdkWindow:
		GdkWindowAttr attributes;
		memset(&attributes, 0, sizeof(attributes));

		Gtk::Allocation allocation = get_allocation();

		//Set initial position and size of the Gdk::Window:
		attributes.x = allocation.get_x();
		attributes.y = allocation.get_y();
		attributes.width = allocation.get_width();
		attributes.height = allocation.get_height();

		attributes.event_mask = get_events() | Gdk::EXPOSURE_MASK;
		attributes.window_type = GDK_WINDOW_CHILD;
		attributes.wclass = GDK_INPUT_OUTPUT;

		mGdkWindow = Gdk::Window::create(get_window() /* parent */, &attributes,
				GDK_WA_X | GDK_WA_Y);
//        unset_flags(Gtk::NO_WINDOW);
		set_has_window(true);
		set_window(mGdkWindow);

		//set colors

		//make the widget receive expose events
		mGdkWindow->set_user_data(gobj());

		///Reference: http://www.nabble.com/Win32-HWND-td20494257.html
		///This is platform specific, compiling on Linux/MacOS will require a different Window Handle
		sf::RenderWindow::Create(GDK_WINDOW_XID(mGdkWindow->gobj()));

		set_can_focus(true);
//		grab_focus();
	}
}

void SFMLGame::on_unrealize()
{
	mGdkWindow.clear();

	//Call base class:
	Gtk::Widget::on_unrealize();
}

bool SFMLGame::on_expose_event(GdkEventExpose* event)
{
	if (mGdkWindow) {
		update();
		Display();
	}
	return true;
}

bool SFMLGame::on_button_press_event(GdkEventButton* event)
{
	using std::cout;
	using std::endl;
	using sf::Vector2;

	grab_focus();

	cout << event->type << endl;
	cout << event->x << endl;
	cout << event->y << endl;

	switch (mState) {
	case SFMLGame::NORMAL:
		mDown1 = resolveMouse(Vector2<int>(event->x, event->y));
		mState = SFMLGame::HLIGHT_1;
		break;
	case SFMLGame::HLIGHT_1:
		mDown2 = resolveMouse(Vector2<int>(event->x, event->y));
		mState = SFMLGame::PLAYER_MOVE;
		break;
	default:
		break;
	}

	return true;
}

bool SFMLGame::on_key_press_event(GdkEventKey* event)
{
	using std::cout;
	using std::endl;

	switch (event->keyval) {
	case GDK_KEY_q:
		cout << "Returning to normal state" << endl;
		mState = NORMAL;
		break;
	default:
		break;
	}
	return true;
}
