#include <random>
#include <algorithm>
#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"

using std::endl;

Card::Card(CardType type)
    : type(type)
{
}

Card::Card(const Card &other)
    : type(other.type)
{
}

Card::~Card()
{
}

Card &Card::operator=(const Card &other)
{
    if (this != &other)
    {
        type = other.type;
    }
    return *this;
}

Order *Card::play(Player *owner, Deck *deck, const vector<Player *> &players)
{
    Order *order = nullptr;
    switch (type)
    {
    case BOMB:
    {
        const auto &attacks = owner->toAttack();
        if (attacks.size() > 0)
        {
            int index = rand() % attacks.size();
            auto t = attacks[index];
            order = owner->issueOrder(type, t);
        }

        break;
    }
    case BLOCKADE:
    {
        const auto &defends = owner->toDefend();
        int index = rand() % defends.size();
        auto t = defends[index];
        order = owner->issueOrder(type, t);
        break;
    }
    case AIRLIFT:
    {
        const auto &defends = owner->toDefend();

        int srcIndex = rand() % defends.size();
        auto t = defends[srcIndex];
        const auto &neighbours = t->getNeighbours();
        int dstIndex = rand() % neighbours.size();
        auto y = neighbours[dstIndex];
        int n = t->selectRandArmy();
        order = owner->issueOrder(type, t, n, y);

        break;
    }
    case DIPLOMACY:
    {
        Player *enemy = owner->selectRandomEnemy(players);
        order = owner->issueOrder(enemy);
        break;
    }
    default:
        break;
    }
    deck->add(this);
    return order;
}

CardType Card::getType() const
{
    return type;
}

void Card::print(ostream &os) const
{
    switch (type)
    {
    case BOMB:
        os << "bomb card";
        break;
    case BLOCKADE:
        os << "blockade card";
        break;
    case AIRLIFT:
        os << "airlift card";
        break;
    case DIPLOMACY:
        os << "diplomacy card";
        break;
    default:
        break;
    }
}

#if 0
Order* Card::play(Player* owner, Deck* deck, Territory* dest)
{
    auto p = owner->issueOrder(type,dest);
    deck->add(this);
    return p;
}

Order* Card::play(Player* owner, Deck* deck, Territory* src, int n, Territory* dest)
{
    auto p = owner->issueOrder(type,src,n,dest);
    deck->add(this);
    return p;
}

Order* Card::play(Player* owner, Deck* deck, Player* destPlayer)
{
    auto p = owner->issueOrder(destPlayer);
    deck->add(this);
    return p;
}
#endif

ostream &operator<<(ostream &os, const Card &card)
{
    card.print(os);
    return os;
}

Hand::Hand()
{
}

Hand::~Hand()
{
    for (const auto x : *this)
        delete x;
}

Hand::Hand(const Hand &other)
    : list<Card *>(other)
{
}

Hand &Hand::operator=(const Hand &other)
{
    if (this != &other)
    {
        list<Card *>::operator=(other);
    }
    return *this;
}

void Hand::add(Card *card)
{
    push_back(card);
}

Card *Hand::pop()
{
    Card *x = front();
    pop_front();
    return x;
}

Card *Hand::top()
{
    return front();
}

void Hand::print(ostream &os) const
{
    for (const auto x : *this)
        os << (*x) << endl;
}

ostream &operator<<(ostream &os, const Hand &hand)
{
    hand.print(os);
    return os;
}

Deck::Deck()
{
}

Deck::~Deck()
{
}

Deck::Deck(const Deck &other)
    : vector<Card *>(other)
{
}

Deck &Deck::operator=(const Deck &other)
{
    if (this != &other)
    {
        vector<Card *>::operator=(other);
    }
    return *this;
}

Card *Deck::draw(Player *player)
{
    std::random_shuffle(begin(), end());
    Card *card = (*this)[0];
    erase(begin());
    player->addCard(card);
    return card;
}

void Deck::add(Card *card)
{
    push_back(card);
}

void Deck::print(ostream &os) const
{
    for (const auto x : *this)
        os << (*x) << endl;
}

ostream &operator<<(ostream &os, const Deck &deck)
{
    deck.print(os);
    return os;
}

Card *randomCard()
{
    static CardType types[] = {
        BOMB, BLOCKADE, AIRLIFT, DIPLOMACY};
    int index = rand() % 4;
    return new Card(types[index]);
}
