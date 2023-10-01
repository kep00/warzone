#pragma once

#include <list>
using std::list;
#include "Map.h"
#include "PlayerStrategies.h"
#include "GameEngine.h"
enum CardType;
class Card;
class OrderList;
class Order;
class Hand;
enum CardType;
class Deck;
class PlayerStrategy;
class GameEngine;
class Player
{
protected:
    string name;

    Hand *hand;
    int armyUnit; // army pool

    vector<Player *> negotiatedPlayers;
    bool conquereFlag; // record whether player conquered at least one territory in his turn

public:
    GameEngine *GE;
    OrderList *orders;
    vector<Territory *> territores;
    PlayerStrategy *ps;

    vector<Territory *> defends;
    vector<Territory *> attacks;
    // constructors and destructors
    Player(const string &name);
    Player(const char *name);
    Player(const Player &other);
    ~Player();
    Player &operator=(const Player &other);

    // getters
    OrderList *getOrders();
    Hand *getCards();
    const vector<Territory *> &getTerritories() const;
    const vector<Player *> &getNegotiatedPlayers() const;

    bool isNegotiated(Player *other) const;
    bool hasConquered() const;
    int getArmyUnit() const;
    void setPlayerStrategy(PlayerStrategy *);
    const string &getName() const;
    int getallarmyUnit();
    // print the information of this player
    void print(ostream &os) const;

    /*
        return a list of territories that are
        to be defendedand to be attacked, respectively.
    */
    const vector<Territory *> &toDefend() ;
    const vector<Territory *> &toAttack() ;

    // detect whether a territory or a continent is owned by this player
    bool owns(const Territory *territory) const;
    bool owns(const Continent *continent) const;

    // setters

    void conqure(Territory *t);
    void add(Territory *t);
    void remove(Territory *t); // remove a territory which is not owned by this player
    void addCard(Card *card);

    void defend(Territory *t);
    void attack(Territory *t);

    // detect whether a plaer has zero territories
    bool isEmpty() const;

    // create a DeployOrder
    Order *issueOrder(int n, Territory *dest);

    // create AdvanceOrder
    Order *issueOrder(Territory *src, int n, Territory *dest);

    // create AirLiftOrder
    Order *issueOrder(CardType type, Territory *src, int n, Territory *dest);

    // create a a bomb order or BlockadeOrder
    Order *issueOrder(CardType cardType, Territory *dest);

    // create a negotiate order
    Order *issueOrder(Player *dest_player);

    // setters
    void setArmyUnit(int n);

    // increase the size of army pool
    void incArmyUnit(int n);

    // decrease the size of army pool
    void decArmyUnit(int n);

    // add a negotiated player
    void addNegotiated(Player *dest_player);

    //
    void clearNegotiated();

    //
    virtual bool isNeutral() const;

    // detech whether a territory is adjacent to one of territories owned by this player
    bool isAdjacentTo(Territory *t) const;

    // //calculate the number of army in pool for a player
    int calArmyPool() const;

    void rebuildDefendList();
    void rebuildAttackList();

    void prepareForTurn();

    Order *issueOrder(Deck *deck, const vector<Player *> &players);

    // random select an enemyw which is not equal to this
    Player *selectRandomEnemy(const vector<Player *> &players) const;

    Order *nextOrder();
};

// NeutralPlayer  only be created when executed blockade order
class NeutralPlayer : public Player
{
public:
    NeutralPlayer();
    virtual bool isNeutral() const;
};

ostream &operator<<(ostream &os, const Player &player);
void testPlayers();
