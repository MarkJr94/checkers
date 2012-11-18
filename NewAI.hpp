/*
 * NewAI.hpp
 *
 *  Created on: Nov 18, 2012
 *      Author: mark
 */

#ifndef NEWAI_HPP_
#define NEWAI_HPP_

#include "AI.hpp"

struct Node {
	Save save;
	int val;
	std::vector<std::unique_ptr<Node> > branches;
	Move move;
	unsigned char depth;

	Node()  {}
	Node(const Save& s, const Move& m, unsigned char depth) : save(s), move(m) , depth(depth){}
	void move_and_restore(const Move&);
	void jump_and_restore(const Move&);
	void print();

};

class NewAI: public BaseAI {
public:
	friend struct Node;

	NewAI(const unsigned char difficulty);
	virtual ~NewAI();

//	void print_scene();

	Move get_random_move() const;

	Move evaluate_game(Game&);


	void difficulty(unsigned difficulty)
	{
		_difficulty = difficulty;
	}

	unsigned difficulty() const
	{
		return _difficulty;
	}

private:
	void gen_moves_black(Node&);
	void gen_moves_white(Node&);
	void gen_moves(Node& n) {
		if (s_game.mTurn)
			gen_moves_black(n);
		else
			gen_moves_white(n);
	}


	void gen_jumps_black(Node&);
	void gen_jumps_white(Node&);
	void gen_jumps(Node& n) {
		if (s_game.mTurn)
			gen_jumps_black(n);
		else
			gen_jumps_white(n);
	}

	void gen_outcomes(Node&);

	void initialize(const Save&);

	int alphabeta(Node& node, int alpha, int beta, bool player);

	Node _root;
	unsigned char _difficulty;
	static Game s_game;
};



#endif /* NEWAI_HPP_ */
