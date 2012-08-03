#include <iostream>
#include <string>
#include <sstream>
#include <exception>

#include "checkers.hpp"
#include "game.hpp"
#include "bst.hpp"

int main() {
	using namespace std;

	string instr;
	Game *theMatch;
	SaveGame *loadGame, *saveGame;
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
