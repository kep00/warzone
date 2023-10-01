#pragma once

#include <vector>
#include <list>
#include <iostream>
#include <cstdlib>
using std::vector;
using std::ostream;
using std::list;

#include "Map.h"


class Player;
class Order;
class Deck;

enum CardType {
    BOMB, BLOCKADE, AIRLIFT, DIPLOMACY
};

class Card {
    CardType type;
    
public:    
    Card(CardType type);
    Card(const Card& other);
    ~Card();
    Card& operator=(const Card& other);

    Order* play(Player* owner, Deck* deck,const vector<Player*>& players);

    //Order* play(Player* owner, Deck* deck, Territory* dest);
    //Order* play(Player* owner, Deck* deck, Territory* src, int n, Territory* dest);
    //Order* play(Player* owner, Deck* deck, Player* destPlayer);

    //getter
    CardType getType()const;

    void print(ostream&os)const;
};
ostream& operator<<(ostream& os, const Card& card);

class Hand:public list<Card*> {
public:
    Hand();
    ~Hand();
    Hand(const Hand& other);
    Hand& operator=(const Hand& other);

    void add(Card* card);
    Card* pop();
    Card* top();

    void print(ostream& os)const;
};

ostream& operator<<(ostream& os, const Hand& hand);

class Deck:public vector<Card*> {
public:
    Deck();
    ~Deck();
    Deck(const Deck& other);
    Deck& operator=(const Deck& other);

    Card* draw(Player* player);
    void add(Card* card);

    void print(ostream& os)const;
};

ostream& operator<<(ostream& os, const Deck& deck);

void testCards();

//generate a random card
Card* randomCard();

