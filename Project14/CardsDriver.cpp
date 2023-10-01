#include <iostream>
#include<string>
using std::cout;
using std::endl;
using std::string;

#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"

#if 0
void testCards() {
    cout << "\nbegin of testCards" << endl;
    MapLoader loader;
    loader.load("1.map");
    auto map = loader.getMap();
    const vector<Territory*>& obj = map->getNodes();

    Player playerA("Tom");
    Player playerB("Jerry");
    Deck* deck = new Deck();

    Card* c1 = new Card(BOMB);
    Card* c2 = new Card(BLOCKADE);
    Card* c3 = new Card(AIRLIFT);
    Card* c4 = new Card(DIPLOMACY);
    deck->add(c1);
    deck->add(c2);
    deck->add(c3);
    deck->add(c4);
    cout << "cards on deck:" << endl;
    cout << (*deck);

    while (!deck->empty())
    {
        Card* c = deck->draw(&playerA);
        cout << "has draw " << (*c) << " from deck to hand" << endl;
    }

    Hand* h = playerA.getCards();
    cout << "after draw deck to hand" << endl;
    cout << "the playerA's hand is" << endl;
    cout << (*h);

    cout << "will play all cards of " << playerA.getName() << endl;
    while (!h->empty()) {
        Card* c = h->pop();
        switch (c->getType())
        {
        case BOMB:
            c->play(&playerA,deck,obj[0]);
            break;
        case BLOCKADE:
            c->play(&playerA, deck, obj[1]);
            break;
        case AIRLIFT:
            c->play(&playerA, deck,obj[2], 20, obj[3]);
            break;
        case DIPLOMACY:
            c->play(&playerB, deck, &playerA);
            break;
        default:
            break;
        }
    }

    cout << "after play all cards of " << playerA.getName() << ","
        << playerA.getName() << "'s orders are:"
        << endl;

    OrderList* order = playerA.getOrders();
    cout << (*order);

    cout << "end of testCards" << endl;
}
#endif
