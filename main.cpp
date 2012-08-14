#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <argparse>

#include "checkers.hpp"
#include "game.hpp"
#include "bst.hpp"
#include "../ogl-tryouts/World.hpp"

int main(int argc, char *argv[]) {
	using namespace std;

	ArgumentParser parser ("draughts");
	parser.addarg<bool>("play",true,'p',false,"Choose whether or not to interact. default yes");
	parser.addarg<bool>("help",false,'h',false,"Display this help message");
	parser.parse(argc,argv);
	bool play = parser.getarg<bool>("play");
	bool help = parser.getarg<bool>("help");
	if (help) {
		parser.help();
		return 0;
	}
	string instr;
	Game *theMatch;
	SaveGame *loadGame, *saveGame;

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
			loadGame = new SaveGame(false);
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
		saveGame = new SaveGame(theMatch->getSave());
		cout << "Enter save file path: ";
		getline(cin, instr);
		saveGame->write(instr);
		delete saveGame;
	}
	delete theMatch;
	return 0;
}
