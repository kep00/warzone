#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>
using std::istringstream;
using std::cin;
using std::endl;

#include "LoggingObserver.h"
#include "GameEngine.h"


static string tab(4, ' ');
void testLoggingObserver() {
	cout << "start of testLoggingObserver" << endl;

	ostringstream oss;
	oss << "loadmap 3.map" << endl;
	oss << "validatemap" << endl;
	oss << "loadmap 1.map" << endl;
	oss << "validatemap" << endl;
	oss << "addplayer A" << endl;
	oss << "addplayer B" << endl;
	oss << "addplayer C" << endl;
	oss << "addplayer D" << endl;
	oss << "addplayer E" << endl;
	oss << "gamestart" << endl;

	istringstream iss(oss.str());
	auto oldBuffer = cin.rdbuf(iss.rdbuf());

	GameEngine ge;
	LoggingObserver observer("gamelog.txt");
	ge.attach(&observer);
	CommandProcessor* cp = new CommandProcessor();
	ge.setCommandProcessor(cp);
	ge.startupPhase();
	ge.maxTurn =1000;
	int n =ge.mainGameLoop();

	cout << "game stops after " << n << " turns." << endl;
	if (ge.getState() == PLAYER_WIN) {
		cout << "winner is " << ge.getPlayers()[0]->getName() << endl;
	}
	else {
		cout << "no player win,final player list is " << endl;
		for (auto x : ge.getPlayers())
			cout << (*x) << endl;
	}
	
	cin.rdbuf(oldBuffer);
	cout << "testLoggingObserver success" << endl;
}