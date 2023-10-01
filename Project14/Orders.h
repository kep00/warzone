#pragma once
#include <vector>
#include <iostream>
using std::vector;
using std::ostream;

#include "LoggingObserver.h"
#include "Map.h"

class Player;

class Order:public ILoggable,public Subject
{
protected:
    Player* owner;
    mutable string effect;
public:
    //constructors
    Order(Player* player);
    virtual ~Order();
    Order(const Order& other);
    Order& operator=(const Order& other);

    //check whether the order is valid
    virtual bool validate()const;
    virtual void enact();

    string getordertype();
    //getter
    Player* getOwner()const;

    bool execute();

    const string& getEffect()const;

    virtual void print(ostream& os)const;
};

ostream& operator<<(ostream&os, const Order& order);

class DeployOrder :public Order {
    int armyUnit;
    Territory* target;
public:
    DeployOrder(Player* owner, int army_num, Territory* t);
    DeployOrder(const DeployOrder& other);
    ~DeployOrder();

    DeployOrder& operator=(const DeployOrder& other);

    virtual bool validate()const;
    virtual void enact();
    virtual void print(ostream& os)const;
};

class AdvanceOrder :public Order {
    int armyUnit;
    Territory* source;
    Territory* target;
public:
    AdvanceOrder(Player* owner, Territory* src, int n, Territory* dest);
    AdvanceOrder(const AdvanceOrder& other);
    ~AdvanceOrder();
    AdvanceOrder& operator=(const AdvanceOrder& other);

    virtual void enact();
    
    //simulate the attack
    virtual bool validate()const;
    void doAttack();
    virtual void print(ostream& os)const;
};

class BombOrder :public Order {
    Territory* target;
public:
    BombOrder(Player* owner, Territory* dest);
    BombOrder(const BombOrder& other);
    ~BombOrder();
    BombOrder& operator=(const BombOrder& other);


    //getters
    Territory* getTarget()const;

    virtual bool validate()const;
    virtual void enact();
    virtual void print(ostream& os)const;
};

class BlockadeOrder :public Order {
    Territory* target;

public:
    BlockadeOrder(Player* owner, Territory* dest);
    BlockadeOrder(const BlockadeOrder& other);
    ~BlockadeOrder();
    BlockadeOrder& operator=(const BlockadeOrder& other);

    virtual bool validate()const;
    virtual void enact();
    virtual void print(ostream& os)const;
};

class AirliftOrder :public Order {
    int armyUnit;
    Territory* source;
    Territory* target;

public:
    AirliftOrder(Player* owner, Territory* src, int n, Territory* dest);
    AirliftOrder(const AirliftOrder& other);
    ~AirliftOrder();
    AirliftOrder& operator=(const AirliftOrder& other);

    virtual bool validate()const;
    virtual void enact();
    virtual void print(ostream& os)const;
};

class NegotiateOrder :public Order {
    Player* destPlayer;

public:
    NegotiateOrder(Player* owner, Player* other);
    NegotiateOrder(const NegotiateOrder& other);
    ~NegotiateOrder();
    NegotiateOrder& operator=(const NegotiateOrder& other);

    virtual bool validate()const;
    virtual void enact();
    virtual void print(ostream& os)const;
};







class OrderList:public vector<Order*>,public ILoggable,public Subject{
public:
    OrderList();
    OrderList(const OrderList& other);
    ~OrderList();
    OrderList& operator=(const OrderList& other);
    
    // move an order in the list of orders.
    void move(Order* order, int offset);

    Order* top()const;
    Order* pop();

    //  deletes an order from the list.
    void remove(Order* order);

    
    void addOrder(Order* order);

    // swap the position of two adjacent orders
    void swapWithRight(int index);

    void swapWithLeft(int index);

    void print(ostream& os)const;
};

ostream& operator<<(ostream& os, const OrderList& orders);
void testOrdersLists();
void testOrderExecution();
