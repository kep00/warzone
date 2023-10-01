
#pragma once
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
class PlayerStrategy
{
public:
    Player *P;//指向Player的指针
    PlayerStrategy();
    ~PlayerStrategy();
    virtual void issueOrder()=0 ;
    virtual void toDefend() = 0;
    virtual void toAttack() = 0;
};

class HumanPlayerStrategy : public PlayerStrategy
{
public:
    HumanPlayerStrategy();
    ~HumanPlayerStrategy();
    void issueOrder();
    void toDefend();
    void toAttack();
};

class AggressivePlayerStrategy : public PlayerStrategy
{
public:
    AggressivePlayerStrategy();
    ~AggressivePlayerStrategy();
    void issueOrder();
    void toDefend();
    void toAttack();
};

class BenevolentPlayerStrategy : public PlayerStrategy
{
public:
    BenevolentPlayerStrategy();
    ~BenevolentPlayerStrategy();
    void issueOrder();
    void toDefend();
    void toAttack();
};

class NeutralPlayerStrategy : public PlayerStrategy
{
public:
    NeutralPlayerStrategy();
    ~NeutralPlayerStrategy();
    void issueOrder();
    void toDefend();
    void toAttack();
};

class CheaterPlayerStrategy : public PlayerStrategy
{
public:
    CheaterPlayerStrategy();
    ~CheaterPlayerStrategy();
    void issueOrder();
    void toDefend();
    void toAttack();
};
void testPlayerStrategies();