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
				sf::RenderWindow(sf::VideoMode(wide, high, 32), "Game Window",
						sf::Style::Titlebar | sf::Style::Close),
				_game(false, false),
				_ai(),
				_board(64),
				_state(NORMAL),
				circRad(40),
				cellSz(100)
{
	set_has_window(false);

	add_events(Gdk::BUTTON_PRESS_MASK);

	Glib::signal_idle().connect(sigc::mem_fun(*this, &SFMLGame::on_idle));
}

SFMLGame::SFMLGame(const sf::VideoMode& mode) :
				sf::RenderWindow(mode, "Game Window",
						sf::Style::Titlebar | sf::Style::Close),
				_game(false, false),
				_ai(),
				_board(64),
				_state(NORMAL),
				circRad(40),
				cellSz(100)
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
	x -= x % cellSz;
	y -= y % cellSz;
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
	const int hCellSz = cellSz / 2;
	switch (c) {
	case P_B:
		piece = Shape::Circle(x * cellSz + hCellSz, y * cellSz + hCellSz,
				circRad, black);
		Draw(piece);
		break;
	case P_W:
		piece = Shape::Circle(x * cellSz + hCellSz, y * cellSz + hCellSz,
				circRad, white);
		Draw(piece);
		break;
	case K_B:
		piece = Shape::Circle(x * cellSz + hCellSz, y * cellSz + hCellSz,
				circRad, black, -5.f, Color::Cyan);
		Draw(piece);
		break;
	case K_W:
		piece = Shape::Circle(x * cellSz + hCellSz, y * cellSz + hCellSz,
				circRad, white, -5.f, Color::Cyan);
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
	os << m.src << " => " << m.dst;

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

	do {
		if (_state == EVALUATING) {
			MoveCode err;

			err = evalSelections();
			std::cout << _errtable[err] << std::endl;
			if (_game._turn) {
				_state = NORMAL;
				break;
			}
			std::pair<Move, bool> moveinfo = _ai.evaluateGame(_game);
			cout << "AI Move: " << moveinfo.first << endl;
			if (moveinfo.first.dst == 0 && moveinfo.first.src == 0)
				break;
			if (moveinfo.second) {
				cout << _errtable[_game.jump(moveinfo.first)] << endl;
			} else
				cout << _errtable[_game.makeMove(moveinfo.first)] << endl;
//			}
			if (_game._turn)
				_state = NORMAL;
		}
	} while (0);
	Clear();
	int m = min(GetWidth(), GetHeight());
//	SetSize(m,m);
	cellSz = m / 8;
	circRad = cellSz * 2 / 5;

//	static int counter = 0;
//	if (counter++ % 30 == 0)
//	cout << cellSz << ": :" << circRad <<  "\t";
//	cout.flush();

	_board = _game.toArr();

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			drawCell(_board[i + j * 8], i, j);
		}
	}

	Shape cell;
	switch (_state) {
	case EVALUATING:
		cell = Shape::Rectangle(_mDown2.x, _mDown2.y, (_mDown2.x + cellSz),
				(_mDown2.y + cellSz), Color(0, 0, 0, 0), -4.f,
				sf::Color::Yellow);
		Draw(cell);
	case HLIGHT_1:
		cell = Shape::Rectangle(_mDown1.x, _mDown1.y, (_mDown1.x + cellSz),
				(_mDown1.y + cellSz), Color(0, 0, 0, 0), -4.f,
				sf::Color::Yellow);
		Draw(cell);
		break;
	default:
		break;
	}
//	std::cout << _mDown1 << " " << _state << std::endl;
}

MoveCode SFMLGame::evalSelections()
{
	using sf::Vector2i;

	int x = _mDown1.x / cellSz, y = 7 - _mDown1.y / cellSz;
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

	x = _mDown2.x / cellSz;
	y = 7 - _mDown2.y / cellSz;
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

	_board = _game.toArr();
	Move m = { src, dst };
	std::cout << "Player Move: " << m << std::endl;
	if (_board[x + y * 8] == EMPTY) {
		return _game.makeMove(m);
	} else {
		return _game.jump(m);
	}
}

void SFMLGame::loop()
{
	using std::cout;
	using std::endl;

	while (IsOpened() && _game.isLive()) {
		if (_state == EVALUATING) {
			MoveCode err;

			err = evalSelections();
			std::cout << _errtable[err] << std::endl;
			if (_game._turn) {
				_state = NORMAL;
				continue;
			}
			std::pair<Move, bool> moveinfo = _ai.evaluateGame(_game);
			cout << "AI Move: " << moveinfo.first << endl;
			if (moveinfo.first.dst == 0 && moveinfo.first.src == 0)
				break;
			if (moveinfo.second) {
				cout << _errtable[_game.jump(moveinfo.first)] << endl;
			} else
				cout << _errtable[_game.makeMove(moveinfo.first)] << endl;
			if (_game._turn)
				_state = NORMAL;
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
    if(_gdkWindow)
    {
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

    if(_gdkWindow)
    {
//        _gdkWindow->move_resize(allocation.get_x(), allocation.get_y(), allocation.get_width(), allocation.get_height() );
    	_gdkWindow->move(allocation.get_x(), allocation.get_y());
//        _sfRenWin->SetSize(allocation.get_width(), allocation.get_height());
//        _sfRenWin->SetPosition(allocation.get_x(),allocation.gset_y());
    }
}

void SFMLGame::on_map()
{
    Gtk::Widget::on_map();
}

void SFMLGame::on_unmap()
{
    Gtk::Widget::on_unmap();
}

void SFMLGame::on_realize()
{
    Gtk::Widget::on_realize();

    if(!_gdkWindow)
    {
        //Create the GdkWindow:
        GdkWindowAttr attributes;
        memset(&attributes, 0, sizeof(attributes));

        Gtk::Allocation allocation = get_allocation();

        //Set initial position and size of the Gdk::Window:
        attributes.x = allocation.get_x();
        attributes.y = allocation.get_y();
        attributes.width = allocation.get_width();
        attributes.height = allocation.get_height();

        attributes.event_mask = get_events () | Gdk::EXPOSURE_MASK;
        attributes.window_type = GDK_WINDOW_CHILD;
        attributes.wclass = GDK_INPUT_OUTPUT;


        _gdkWindow = Gdk::Window::create(get_window() /* parent */, &attributes,
                GDK_WA_X | GDK_WA_Y);
//        unset_flags(Gtk::NO_WINDOW);
        set_has_window(true);
        set_window(_gdkWindow);

        //set colors

        //make the widget receive expose events
        _gdkWindow->set_user_data(gobj());

        ///Reference: http://www.nabble.com/Win32-HWND-td20494257.html
        ///This is platform specific, compiling on Linux/MacOS will require a different Window Handle
        sf::RenderWindow::Create(GDK_WINDOW_XID(_gdkWindow->gobj()));
    }
}

void SFMLGame::on_unrealize()
{
  _gdkWindow.clear();

  //Call base class:
  Gtk::Widget::on_unrealize();
}

bool SFMLGame::on_expose_event(GdkEventExpose* event)
{
    if(_gdkWindow)
    {
        update();
        Display();
    }
    return true;
}

bool SFMLGame::on_button_press_event(GdkEventButton* event) {
	using std::cout;
	using std::endl;
	using sf::Vector2;

	cout << event->type << endl;
	cout << event->x << endl;
	cout << event->y << endl;

	switch (_state) {
	case SFMLGame::NORMAL:
		_mDown1 = resolveMouse(
				Vector2<int>(event->x,
						event->y));
		_state = SFMLGame::HLIGHT_1;
		break;
	case SFMLGame::HLIGHT_1:
		_mDown2 = resolveMouse(
				Vector2<int>(event->x,
						event->y));
		_state = SFMLGame::EVALUATING;
		break;
	default:
		break;
	}

	return true;
}
