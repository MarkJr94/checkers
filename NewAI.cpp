/*
 * NewAI.cpp
 *
 *  Created on: Nov 18, 2012
 *      Author: mark
 */

#include "NewAI.hpp"

#include <algorithm>
#include <iostream>
#include <limits>

Game NewAI::s_game(false, false);

NewAI::NewAI(const unsigned char difficulty) :
				_difficulty(difficulty)
{
	_root.depth = 0;
	// TODO Auto-generated constructor stub

}

NewAI::~NewAI()
{
	// TODO Auto-generated destructor stub
}

void NewAI::initialize(const Save& s)
{
	s_game.restoreToSave(s);
	_root.branches.clear();
	_root.save = s;
}

Move NewAI::evaluate_game(Game& g)
{
	const Save old_save = g.getSave();
	initialize(old_save);

	int alpha = std::numeric_limits<int>::min();
	int beta = std::numeric_limits<int>::max();

	alphabeta(_root, alpha, beta, old_save.turn);

	if (_root.branches.empty()) return {0,0,false};

	Move best_move;
	int idx = 0;
	if (g.mTurn) {
		int max = alpha;
		for (int i = 0; i < _root.branches.size(); i++) {
			if (_root.branches[i]->val > max) {
				max = _root.branches[i]->val;
				idx = i;
			}
//			_root.branches[i]->print();
		}
	} else {
		int min = beta;
		for (int i = 0; i < _root.branches.size(); i++) {
			if (_root.branches[i]->val < min) {
				min = _root.branches[i]->val;
				idx = i;
			}
//			_root.branches[i]->print();
		}
	}

	return _root.branches[idx]->move;

}

int NewAI::alphabeta(Node& node, int alpha, int beta, bool player)
{
	using std::max;
	using std::min;

	if (!s_game.getJumpers() && !s_game.getMovers()) {
		if (node.depth == 0) std::cerr << "NO MOVES: FIRST CHECK.\n";
		return node.save.grade();
	}

//	if (node.depth == 0)
//		return node.save.grade();
	if (node.depth >= _difficulty) {
//		std::cerr << "TOO DEEP!!.!!" << (int)node.depth << " " << (int)_difficulty << std::endl;
		return node.save.grade();
	}

	gen_outcomes(node);
	if (player) {
		for (auto & child : node.branches) {
			alpha = max(alpha, alphabeta(*(child), alpha, beta, !player));
//			Node nd = *child;
			if (beta <= alpha)
				break;
		}
		node.val = alpha;
		return alpha;
	} else {
		for (auto & child : node.branches) {
			beta = min(beta, alphabeta(*(child), alpha, beta, !player));
			//			Node nd = *child;
			if (beta <= alpha)
				break;
		}
		node.val = beta;
		return beta;
	}

}

void NewAI::gen_outcomes(Node& n)
{
	gen_jumps(n);
	if (n.branches.empty())
		gen_moves(n);
}

void NewAI::gen_moves_black(Node& n)
{
//	using namespace std;
	using namespace Bit::Masks;
	using Bit::rol;
	using Bit::ror;
	using Bit::highBit;

	BB Movers = s_game.getMovers();
//	std::cout << "++++++++++++" << hex << Movers << dec <<std::endl;

	BB empty = s_game.getEmpty();
	BB target;
	while (Movers) {
		const BB mover = highBit(Movers);
//		std::cout << "++++++++++++mover = :" << hex << mover << dec <<std::endl;
		Movers ^= mover;
		if ((target = ((rol(mover & CAN_UPLEFT, 7) & empty))))
			n.move_and_restore( { bbUMap[mover], bbUMap[target], false });
		if ((target = ((rol(mover & CAN_UPRIGHT, 1) & empty))))
//			mMoves.push_back( { bbUMap[mover], bbUMap[target], false });
			n.move_and_restore( { bbUMap[mover], bbUMap[target], false });

		if (mover & s_game.mK) {
			if ((target = ((ror(mover & CAN_DOWNRIGHT, 7) & empty))))
				n.move_and_restore( { bbUMap[mover], bbUMap[target], false });
			if ((target = ((ror(mover & CAN_DOWNLEFT, 1) & empty))))
				n.move_and_restore( { bbUMap[mover], bbUMap[target], false });
		}
	}
}

void NewAI::gen_moves_white(Node& n)
{
	using namespace std;
	using namespace Bit::Masks;
	using Bit::ror;
	using Bit::rol;
	using Bit::highBit;

	BB Movers = s_game.getMovers();
//	std::cout << "++++++++++++" << hex << Movers << dec <<std::endl;

	BB empty = s_game.getEmpty();
	BB target;
	while (Movers) {
		BB mover = highBit(Movers);
//		std::cout << "++++++++++++mover = :" << hex << mover << dec <<std::endl;
		Movers ^= mover;

		if ((target = ((ror(mover & CAN_DOWNRIGHT, 7) & empty))))
//			mMoves.push_back( { bbUMap[mover], bbUMap[target], false });
			n.move_and_restore( { bbUMap[mover], bbUMap[target], false });

		if ((target = ((ror(mover & CAN_DOWNLEFT, 1) & empty))))
			n.move_and_restore( { bbUMap[mover], bbUMap[target], false });

		if (mover & s_game.mK) {
			if ((target = ((rol(mover & CAN_UPLEFT, 7) & empty))))
				n.move_and_restore( { bbUMap[mover], bbUMap[target], false });
			if ((target = ((rol(mover & CAN_UPRIGHT, 1) & empty))))
				n.move_and_restore( { bbUMap[mover], bbUMap[target], false });
		}
	}
}

void NewAI::gen_jumps_black(Node& n)
{
	using namespace std;
	using Bit::Masks::bbUMap;
	using Bit::rol;
	using Bit::ror;
	using Bit::highBit;

	BB jumpers = s_game.getJumpers();
//	_game->print();
//	std::cout << "++++++++++++" << hex << jumpers << dec << std::endl;

	while (jumpers) {
		BB j = highBit(jumpers);
//		std::cout << "++++++++++++jumper = :" << hex << j << dec <<std::endl;
		jumpers ^= j;
		BB victims = s_game.mWP;
		BB vict;
		vict = rol(j, 7) & victims;
		if (s_game.canJump(j, vict)) {
//			mMoves.push_back( { bbUMap[j], bbUMap[vict], true });
			n.move_and_restore( { bbUMap[j], bbUMap[vict], true });
		}
		vict = rol(j, 1) & victims;
		if (s_game.canJump(j, vict)) {
//			mMoves.push_back( { bbUMap[j], bbUMap[vict], true });
			n.move_and_restore( { bbUMap[j], bbUMap[vict], true });
		}

		if (j & s_game.mK) {
			vict = ror(j, 7) & victims;
			if (s_game.canJump(j, vict)) {
				n.move_and_restore( { bbUMap[j], bbUMap[vict], true });
			}
			vict = ror(j, 1) & victims;
			if (s_game.canJump(j, vict)) {
				n.move_and_restore( { bbUMap[j], bbUMap[vict], true });
			}
		}
	}
}

void NewAI::gen_jumps_white(Node& n)
{
	using namespace std;
	using Bit::Masks::bbUMap;
	using Bit::rol;
	using Bit::ror;
	using Bit::highBit;

	BB jumpers = s_game.getJumpers();

	while (jumpers) {
		BB j = highBit(jumpers);
		jumpers ^= j;
		BB victims = s_game.mBP;

		BB vict = ror(j, 7) & victims;
		if (s_game.canJump(j, vict)) {
//			mMoves.push_back( { bbUMap[j], bbUMap[vict], true });
			n.move_and_restore( { bbUMap[j], bbUMap[vict], true });
		}
		vict = ror(j, 1) & victims;
		if (s_game.canJump(j, vict)) {
			n.move_and_restore( { bbUMap[j], bbUMap[vict], true });
		}

		if (j & s_game.mK) {
			vict = rol(j, 7) & victims;
			if (s_game.canJump(j, vict)) {
				n.move_and_restore( { bbUMap[j], bbUMap[vict], true });
			}
			vict = rol(j, 1) & victims;
			if (s_game.canJump(j, vict)) {
				n.move_and_restore( { bbUMap[j], bbUMap[vict], true });
			}
		}
	}
}

Move NewAI::get_random_move() const
{
	unsigned index = rand() % _root.branches.size();

	return _root.branches[index]->move;
}

void Node::move_and_restore(const Move& m)
{
	NewAI::s_game.move(m);

	branches.push_back(
			std::unique_ptr<Node>(
					new Node(NewAI::s_game.getSave(), m, depth + 1)));
	NewAI::s_game.restoreToSave(save);
}

void Node::print()
{
	using namespace std;

	cout << "This is a degree " << (int)depth << " Node with Value: " << val << "."
			<< endl;
}
