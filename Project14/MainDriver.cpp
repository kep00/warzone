#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
using std::ofstream;

#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "GameEngine.h"
#include "CommandProcessing.h"
#include "PlayerStrategies.h"

int main(int argc, char *argv[])
{

    ofstream ofs("proj2_test_log.txt");
    auto oldbuf = cout.rdbuf(ofs.rdbuf());
    cout << "start!" << endl;
    ;
    // testLoadMaps();
    // testPlayers();
    // testOrdersLists();
    // testCards();
    // testGameStates();

    // testCommandProcessor();

    // testStartupPhase();

    // testMainGameLoop();

    // testOrderExecution();

    // testLoggingObserver();

    testTournament();
    cout.rdbuf(oldbuf);
    cout << "the running detail can be seen in file proj2_test_log.txt,and log of testLoggingObserver is saved in gamelog.txt" << endl;

    testPlayerStrategies();
    return 0;
}