#ifndef GAME_HPP_
#define GAME_HPP_

#include "checkers.hpp"
#include "player.hpp"
#include <vector>
#include <fstream>
#include <string>

struct cellRecord {
	bool alive;
	Piece::Color color;
	unsigned id;
};

class SaveGame {
	std::vector<std::vector<cellRecord>> data;
	bool turn;
	
public:
	SaveGame(bool turn)
			: data (8, std::vector<cellRecord> (8)), turn(turn)
	{;}
	
	/* Setter and getter for turn */
	void setTurn (bool newval) {turn = newval;}
	bool getTurn () {return turn;}
	
	SaveGame & operator=(const SaveGame other)
	{
		for (unsigned i = 0; i < BOARD_SIZE; i++) {
			for (unsigned j = 0; j < BOARD_SIZE; j++) {
				data[i][j] = other.data[i][j];
			}
		}
		this->turn = other.turn;
		return *this;
	}

	inline cellRecord & operator()(int row, int col)
	{
		return data[row][col];
	}
	
	inline std::vector<cellRecord> & operator()(int row)
	{
		return data[row];
	}
	
	void write(std::string fname)
	{
		using namespace std;
		
		fstream savefile;
		savefile.open(fname.c_str(),fstream::trunc | fstream::out);
		
		savefile << turn << endl;
		for (unsigned i = 0; i < BOARD_SIZE; i++) {
			for (unsigned j = 0; j < BOARD_SIZE; j++) {
				savefile << data[i][j].id << " " << data[i][j].alive 
					<< " " << data[i][j].color << " ";
			}
			savefile << endl;
		}
		savefile.close();
	}
	
	void read(std::string fname)
	{
		using namespace std;
		
		fstream savefile;
		savefile.open(fname.c_str(),fstream::in);
		bool turn,alive;
		unsigned col;
		unsigned id;
		
		
		savefile >> turn;
		this->turn = turn;
		for (unsigned i = 0; i < BOARD_SIZE; i++) {
			for (unsigned j = 0; j < BOARD_SIZE; j++) {
				savefile >> id >> alive >> col; 
				data[i][j].color = (Piece::Color)col;
				data[i][j].alive = alive;
				data[i][j].id = id;
			}
		}
		savefile.close();
	}
};

class Match {
	Player p1;
	Player p2;
	std::vector<std::vector<Piece> > board;
	/* Tracks if it's P1's turn or not */
	bool turn;
	bool debug;
	SaveGame save;
	
public:
	/* Enumerations for movement and jump directions */
	enum Direction {LEFT,RIGHT};
	/* Constructor */
	Match(bool db = true);
	/* Constructor from memory */
	Match(SaveGame record, bool db);
	/* Update save game */
	inline void updateSave();
	/* return save game */
	SaveGame getSave();
	/* Print game */
	void print() const;
	/* Piece Movement */
	bool movePiece(unsigned piece, Direction d);
	/* Jumping */
	bool jumpPiece(unsigned jumper, unsigned prey);
	/* Play */
	void play();
	/* restore game to save */
	void restoreToSave(SaveGame& record);
	/* Receive input for CLI */
	int receiveInput(unsigned piece, Direction d);
	/* Get p1 score */
	inline unsigned getP1score() {	return p1.getnPieces();}
	/* Get p2 score */
	inline unsigned getP2Score() {	return p2.getnPieces();}
};

#endif /* GAME_HPP_ */
