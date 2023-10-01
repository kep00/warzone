#include <iostream>
#include<string>
using std::cout;
using std::endl;
using std::string;

#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"

#include "Player.h"

void testPlayers() {
    cout << "\nbegin of testPlayers" << endl;
    MapLoader loader;
    loader.load("1.map");
    auto map = loader.getMap();
    const vector<Territory*>& obj = map->getNodes();

    Player playerA("Tom");
    Player playerB("Jerry");

    Deck deck;

    playerA.add(obj[7]);
    playerA.add(obj[8]);

    playerA.issueOrder(3, obj[0]);
#if 1
    
    playerA.issueOrder(obj[1], 10, obj[2]);
    playerA.issueOrder(AIRLIFT,obj[3], 10, obj[4]);
    playerA.issueOrder(BLOCKADE, obj[5]);
    playerA.issueOrder(BOMB,obj[6]);
    playerA.issueOrder(&playerB);

    Hand* h = playerA.getCards();
    Card* c1 = new Card(BOMB);
    Card* c2 = new Card(BLOCKADE);
    Card* c3 = new Card(AIRLIFT);
    Card* c4 = new Card(DIPLOMACY);

    h->add(c1);
    h->add(c2);
    h->add(c3);
    h->add(c4);
    cout << playerA;

    playerA.defend(obj[9]);
    playerA.defend(obj[10]);
    playerA.defend(obj[11]);
    playerA.attack(obj[12]);
    playerA.attack(obj[13]);
    playerA.attack(obj[14]);

    cout << "Defend List of " << playerA.getName() << ":" << endl;
    for (const auto x : playerA.toDefend())
        cout << (x->getName()) << endl;
    cout << "Attack List of " << playerA.getName() << ":" << endl;
    for (const auto x : playerA.toDefend())
        cout << (x->getName()) << endl;
#endif 

    cout << "end of testPlayers" << endl;
}