#include <cassert>
#include <iostream>
#include <algorithm>
using std::endl;
using std::random_shuffle;

#include "Orders.h"
#include "Cards.h"
#include "Player.h"

void Player::conqure(Territory *t)
{
    add(t);
    conquereFlag = true;
}

void Player::add(Territory *t)
{
    auto oldOwner = t->getPlayer();
    if (oldOwner != nullptr)
    {
        oldOwner->remove(t);

        if (oldOwner->isNeutral())
            delete oldOwner;
    }
    territores.push_back(t);
    t->setPlayer(this);
}

void Player::remove(Territory *t)
{
    auto it = find(territores.begin(), territores.end(), t);
    if (it != territores.end())
    {
        territores.erase(it);
    }
}

void Player::addCard(Card *card)
{
    hand->add(card);
}

void Player::defend(Territory *t)
{
    defends.push_back(t);
}

void Player::attack(Territory *t)
{
    attacks.push_back(t);
}

bool Player::isEmpty() const
{
    return territores.empty();
}

Order *Player::issueOrder(int n, Territory *dest)
{
    auto p = new DeployOrder(this, n, dest);
    orders->addOrder(p);
    return p;
}

Order *Player::issueOrder(Territory *src, int n, Territory *dest)
{
    auto p = new AdvanceOrder(this, src, n, dest);
    orders->addOrder(p);
    return p;
}

Order *Player::issueOrder(CardType type, Territory *src, int n, Territory *dest)
{
    assert(type == AIRLIFT);
    auto p = new AirliftOrder(this, src, n, dest);
    orders->addOrder(p);
    return p;
}

Order *Player::issueOrder(CardType cardType, Territory *dest)
{
    Order *p = nullptr;
    if (cardType == BOMB)
        p = new BombOrder(this, dest);
    else if (cardType == BLOCKADE)
        p = new BlockadeOrder(this, dest);
    else
        assert(false);
    orders->addOrder(p);
    return p;
}

Order *Player::issueOrder(Player *destPlayer)
{
    auto p = new NegotiateOrder(this, destPlayer);
    orders->addOrder(p);
    return p;
}

void Player::setArmyUnit(int n)
{
    armyUnit = n;
}

void Player::incArmyUnit(int n)
{
    armyUnit += n;
}

void Player::decArmyUnit(int n)
{
    armyUnit -= n;
}

void Player::addNegotiated(Player *player)
{
    negotiatedPlayers.push_back(player);
}

void Player::clearNegotiated()
{
    negotiatedPlayers.clear();
}

bool Player::isNeutral() const
{
    return false;
}

bool Player::isAdjacentTo(Territory *t) const
{
    for (auto x : territores)
        if (x->isAdjacentTo(t))
            return true;
    return false;
}

/*
Players are given a number of army units that depends on the number of
territories they own, (# of territories owned divided by 3, rounded down). If the player owns all the
territories of an entire continent, the player is given a number of army units corresponding to the
continent��s control bonus value. In any case, the minimal number of reinforcement army units per turn for
any player is 3.
*/
int Player::calArmyPool() const
{
    int n = territores.size() / 3;

    map<string, Continent *> continents;

    // get different continents
    for (auto x : territores)
    {
        auto c = x->getContinent();
        auto name = c->getName();
        continents[name] = c;
    }

    for (auto x : continents)
    {
        auto c = x.second;
        if (owns(c))
            n += c->getValue();
    }

    if (n < 3)
        n = 3;
    return n;
}

void Player::rebuildDefendList()
{
    defends = territores;
    random_shuffle(defends.begin(), defends.end());
}

void Player::rebuildAttackList()
{
    map<string, Territory *> nodes;

    // get all neighbour nodes owned by other player
    for (auto x : territores)
    {
        for (auto y : x->getNeighbours())
            if (y->getPlayer() != this)
            {
                nodes[y->getName()] = y;
            }
    }

    attacks.clear();
    for (auto x : nodes)
    {
        attacks.push_back(x.second);
    }
}

void Player::prepareForTurn()
{
    // clearNegotiated();
    // conquereFlag = false;
    // rebuildDefendList();
    // rebuildAttackList();

    ps->issueOrder();
    ps->toAttack();
    ps->toDefend();
}

Order *Player::issueOrder(Deck *deck, const vector<Player *> &players)
{
    Order *order = nullptr;
    /*
    As long as the player has army units in their reinforcement pool, it
    can only issue a deploy order.
    */
    if (armyUnit > 0)
    {
        int destIndex = rand() % defends.size();
        auto t = defends[destIndex];
        int n = (rand() % armyUnit + 1);
        order = issueOrder(n, t);
        armyUnit -= n;
    }
    else if (!hand->empty())
    {
        // try to issue an order using card
        Card *c = hand->pop();
        order = c->play(this, deck, players);
    }
    else if (!defends.empty())
    {
        // last issue advance order
        auto t = defends.back();
        defends.pop_back();

        // random select a dest node
        int destIndex = rand() % (t->getNeighbours().size());
        auto y = t->getNeighbours()[destIndex];
        int n = t->selectRandArmy();
        order = issueOrder(t, n, y);
    }
    return order;
}

OrderList *Player::getOrders()
{
    return orders;
}

Hand *Player::getCards()
{
    return hand;
}

const vector<Territory *> &Player::getTerritories() const
{
    return territores;
}

const vector<Player *> &Player::getNegotiatedPlayers() const
{
    return negotiatedPlayers;
}

bool Player::isNegotiated(Player *other) const
{
    return find(negotiatedPlayers.begin(), negotiatedPlayers.end(), other) != negotiatedPlayers.end();
}

bool Player::hasConquered() const
{
    return conquereFlag;
}

int Player::getArmyUnit() const
{
    return armyUnit;
}
void Player::setPlayerStrategy(PlayerStrategy *a)
{
    ps = a;
    ps->P = this;
}
const string &Player::getName() const
{
    return name;
}
int Player::getallarmyUnit() // 获得所有军队的和
{
    int all = 0;
    for (int i = 0; i < territores.size(); i++)
    {
        all += territores[i]->getArmyUnit();
    }
    return all;
}
void Player::print(ostream &os) const
{
    os << "Player name:" << name << endl;

    os << " owns " << territores.size() << " territories:  " << endl;
    for (auto const x : territores)
        os << (*x) << endl;
    os << " has " << armyUnit << " armys in pool " << endl;
    os << " issue " << orders->size() << "orders:" << endl;
    os << (*orders);
    os << hand->size() << " cards in hand "
       << ":" << endl;
    os << (*hand);
}

Player::Player(const string &name)
    : name(name)
{
    orders = new OrderList();
    hand = new Hand();
}

Player::Player(const char *name)
    : name(name)
{
    orders = new OrderList();
    hand = new Hand();
}

Player::Player(const Player &other)
    : name(other.name), territores(other.territores), orders(other.orders), hand(other.hand), armyUnit(other.armyUnit)
{
}

Player::~Player()
{
    delete orders;
    delete hand;
    delete ps;
}

Player &Player::operator=(const Player &other)
{
    if (this != &other)
    {
        this->name = other.name;
        this->territores = other.territores;
        this->orders = other.orders;
        this->hand = other.hand;
        this->armyUnit = other.armyUnit;
    }

    return *this;
}

bool isNeutral(Player *player)
{
    return false;
}

ostream &operator<<(ostream &os, const Player &player)
{
    player.print(os);
    return os;
}

const vector<Territory *> &Player::toDefend()
{
    rebuildAttackList();
    rebuildDefendList();
    if (ps)
        ps->toDefend();
    rebuildAttackList();
    rebuildDefendList();
    return defends;
}

const vector<Territory *> &Player::toAttack()
{
    rebuildAttackList();
    rebuildDefendList();
    if (ps)
        ps->toAttack();
    rebuildAttackList();
    rebuildDefendList();
    return attacks;
}

bool Player::owns(const Territory *territory) const
{
    return territory->getPlayer() == this;
}

bool Player::owns(const Continent *continent) const
{
    for (auto x : continent->getTerritories())
        if (!owns(x))
            return false;
    return true;
}

NeutralPlayer::NeutralPlayer()
    : Player("neutral player")
{
}

bool NeutralPlayer::isNeutral() const
{
    return true;
}

// select a random enemy of the given player
Player *Player::selectRandomEnemy(const vector<Player *> &players) const
{
    Player *p = const_cast<Player *>(this);
    while (p == this)
    {
        int index = rand() % players.size();
        p = players[index];
    }
    return p;
}

Order *Player::nextOrder()
{
    if (orders->empty())
        return nullptr;
    else
        return orders->pop();
}
