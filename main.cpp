#include <iostream>
#include <string>
#include <sstream>
#include <exception>

#include "checkers.hpp"
#include "player.hpp"
#include "game.hpp"


int main()
{
	using namespace std;
	
	string instr;
	Match *theMatch;
	SaveGame *loadGame, *saveGame;
	cout << "Would you like to load a game? (y/n): ";
	getline(cin,instr);
	if (instr == "y") {
		cout << "Enter save file path: ";
		getline(cin, instr);
		try {
			loadGame = new SaveGame (false);
			loadGame->read(instr);
		} catch (const ios_base::failure&) {
			cerr << "Error loading savefile \"" << instr << "\"" << endl;
			return 1;
		}
		theMatch = new Match(*loadGame,true);
	} else {
		theMatch = new Match(true);
	}
	theMatch->play();
	cout << "Would you like to save your game? (y/n): ";
	getline(cin,instr);
	if (instr == "y") {
		saveGame = new SaveGame(theMatch->getSave());
		cout << "Enter save file path: ";
		getline(cin, instr);
		saveGame->write(instr);
	}
	return 0;
}
