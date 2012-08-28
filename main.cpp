#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <optparser>

#include "Piece.hpp"
#include "Game.hpp"
#include "AI.hpp"

int main(int argc, char *argv[]) {
	using namespace std;

	OP::OptParser parser ("draughts");
	parser.addOpt<bool>("play",true,'p',false,"Choose whether or not to interact. default yes");
	parser.addOpt<bool>("help",false,'h',false,"Display this help message");
	parser.parse(argc,argv);
	bool play = parser.getOpt<bool>("play");
	bool help = parser.getOpt<bool>("help");
	if (help) {
		parser.help();
		return 0;
	}
	string instr;
	Game *theMatch;
	Save *loadGame, *saveGame;

	if (!play) {
		theMatch = new Game(true, true);
		playAIvsAI(theMatch,true);
		return 0;
	}
	cout << "Would you like to load a game? (y/n): ";
	getline(cin, instr);
	if (instr == "y") {
		cout << "Enter save file path: ";
		getline(cin, instr);
		try {
			loadGame = new Save(false);
			loadGame->read(instr);
		} catch (const ios_base::failure&) {
			cerr << "Error loading savefile \"" << instr << "\"" << endl;
			return 1;
		}
		theMatch = new Game(*loadGame, true, true);
		delete loadGame;
	} else {
		theMatch = new Game(true, true);
	}
	cout << "Please choose an option by entering it's number :\n"
			<< "1.    Play against AI\t\t2.    Play against a friend\n"
			<< "3.    Watch 2 AI's play each other.\n\n";
	getline(cin, instr);

	if (instr == "1")
		playAgainstAI(theMatch, true);
	else if (instr == "2") {
		playPvP(theMatch);
	} else if (instr == "3") {
		playAIvsAI(theMatch, true);
	} else {
		cerr << "I'm sorry you haven't chosen a valid option. Goodbye!\n";
		return 1;
	}

	cout << "Would you like to save your game? (y/n): ";
	getline(cin, instr);
	if (instr == "y") {
		saveGame = new Save(theMatch->getSave());
		cout << "Enter save file path: ";
		getline(cin, instr);
		saveGame->write(instr);
		delete saveGame;
	}
	delete theMatch;
	return 0;
}
