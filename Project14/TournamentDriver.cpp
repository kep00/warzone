#pragma once
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"

#include "Player.h"
#include "PlayerStrategies.h"
#include "CommandProcessing.h"
void testTournament()
{
    cout << "begin of testTournament" << endl;
    FileCommandProcessorAdapter fcp("PlayerStrategiesDrivertest.txt");
    Command *cmd = fcp.getCommand();
    GameEngine ge;
    ge.process(cmd);
    cout  << "testTournament success" << endl;
}