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

void testPlayerStrategies()
{
    cout << "begin of testPlayerStrategies" << endl;
    GameEngine ge;
    ge.loadMap("map10.txt");
    if (!ge.validatemap())
    {
        cout << "map error" << endl;
    }

    for (int i = 0; i < 5; i++)
    {
        ge.addPlayer(new Player("player" + std::to_string(i + 1)));
    }

    ge.getPlayers()[0]->ps = (PlayerStrategy *)new AggressivePlayerStrategy();
    cout << "Player1 "
         << ":Aggressive" << endl;

    ge.getPlayers()[1]->ps = (PlayerStrategy *)new BenevolentPlayerStrategy();
    cout << "Player2 "
         << ":Benevolent" << endl;

    ge.getPlayers()[2]->ps = (PlayerStrategy *)new NeutralPlayerStrategy();
    cout << "Player3 "
         << ":Neutral" << endl;

    ge.getPlayers()[3]->ps = (PlayerStrategy *)new CheaterPlayerStrategy();
    cout << "Player4 "
         << ":Cheater" << endl;

    ge.getPlayers()[4]->ps = (PlayerStrategy *)new HumanPlayerStrategy();
    cout << "Player5 "
         << ":Human" << endl;
    for (int i = 0; i < 5; i++)
    {
        ge.getPlayers()[i]->ps->P = ge.getPlayers()[i];
    }
    cout << "MaxTurn:" << 50 << endl;
    ge.maxTurn = 50;
    ge.startGame();
    ge.mainGameLoop();
    cout << "testPlayerStrategies success" << endl;
}