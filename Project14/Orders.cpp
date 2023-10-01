#include <cassert>
#include <iostream>
#include <sstream>
using std::ostringstream;
using std::cout;
using std::endl;

#include "Map.h"
#include "Player.h"
#include "Orders.h"

OrderList::OrderList()
{

}

OrderList::OrderList(const OrderList& other)
    :vector<Order*>(other)
    ,ILoggable(other)
    , Subject(other)
{
    
}

OrderList::~OrderList()
{
    for (auto x : *this) {
        delete x;
    }
}

OrderList& OrderList::operator=(const OrderList& other) {
    if (this != &other) {
        vector<Order*>::operator=(other);
        ILoggable::operator=(other);
        Subject::operator=(other);
    }

    return *this;
}

Order::Order(Player* player)
    :owner(player)
{
    
}

Order::~Order()
{

}

Order::Order(const Order& other)
    :
    ILoggable(other)
    , Subject(other)
    ,owner(other.owner)
    ,effect(other.effect)
{
    
}

Order& Order::operator=(const Order& other) {
    if (this != &other) {
        ILoggable::operator=(other);
        Subject::operator=(other);
        owner = other.owner;
        effect =other.effect;
    }

    return *this;
}

bool Order::validate()const
{
    return true;
}

void Order::enact()
{
    //cout << owner->name;
}

string Order::getordertype()
{
    if (dynamic_cast<DeployOrder*>(this))
    {
        return "DeployOrder";
    }
    if (dynamic_cast<AdvanceOrder*>(this))
    {
        return "AdvanceOrder";
    }
    if (dynamic_cast<BombOrder*>(this))
    {
        return "BombOrder";
    }
    if (dynamic_cast<BlockadeOrder*>(this))
    {
        return "BlockadeOrder";
    }
    if (dynamic_cast<AirliftOrder*>(this))
    {
        return "AirliftOrder";
    }
    if (dynamic_cast<NegotiateOrder*>(this))
    {
        return "NegotiateOrder";
    }
    return "";
}

Player* Order::getOwner() const
{
    return owner;
}

bool Order::execute(){
    bool res = validate();
    if (res) {
        enact();
    }
    
    logString = effect;
    notify(this);

    return res;
}

const string& Order::getEffect() const
{
    return effect;
}

void Order::print(ostream& os) const
{
    os << owner->getName() << " will ";
}

ostream& operator<<(ostream& os, const Order& order) {
    order.print(os);
    return os;
}

void OrderList::move(Order* order, int offset)
{
    if (offset==0)
        return;

    size_t index = 0;
    for (;index < size();++index) {
        auto x = (*this)[index];
        if (x == order)
            break;
    }

    if (offset>0){
        for (int i = 0;i < offset;++i, ++index)
        {
            swapWithRight(index % size());
        }
    }
    else {
        for (int i = 0;i < -offset;++i, --index)
        {
            swapWithLeft(index % size());
        }
    }
}

Order* OrderList::top() const
{
    return front();
}

Order* OrderList::pop()
{
    Order* order = top();
    erase(begin());
    return order;
}

void OrderList::remove(Order* order)
{
    auto it = begin();
    for (;it != end();++it) {
        auto& x = *it;
        if ((x == order))
        {
            break;
        }
    }
    erase(it);
}

void OrderList::addOrder(Order* order)
{
    order->copyObservers(*this);
    push_back(order);
    ostringstream oss;
    oss<<"add an order:"<< (*order);
    logString = oss.str();
    notify(this);
}

void OrderList::swapWithRight(int index)
{
    int next_index = (index + 1) % size();
    Order* A = (*this)[index];
    Order* B = (*this)[next_index];
    (*this)[index] = B;
    (*this)[next_index] = A;
}

void OrderList::swapWithLeft(int index)
{
    int n = size();
    int prev_index = (index - 1) % n;
    Order* A = (*this)[index];
    Order* B = (*this)[prev_index];
    (*this)[index] = B;
    (*this)[prev_index] = A;
}

void OrderList::print(ostream& os) const
{
    for(auto x:*this)
        os << (*x) <<endl;
}

ostream& operator<<(ostream& os, const OrderList& orders) {
    orders.print(os);
    return os;
}

DeployOrder::DeployOrder(Player* owner, int armyUnit, Territory* t)
    :Order(owner)
    , armyUnit(armyUnit)
    , target(t)
{

}

DeployOrder::DeployOrder(const DeployOrder& other)
    :Order(other)
    , armyUnit(other.armyUnit)
    , target(other.target)
{

}

DeployOrder::~DeployOrder()
{

}

DeployOrder& DeployOrder::operator=(const DeployOrder& other) {
    if (this != &other) {
        Order::operator=(other);
        armyUnit = other.armyUnit;
        target = other.target;
    }
    return *this;
}

bool DeployOrder::validate() const
{
    // If the target territory does not belong to the player that issued the order, the order is invalid.
    assert (owner!=nullptr  && target!=nullptr);

    if (target->getPlayer() != owner)
    {
        ostringstream oss;
        oss << "failed because the dest doesnt belongs to player " <<owner->getName();
        effect = oss.str();
        owner->incArmyUnit(armyUnit);
        return false;
    }
    return true;
}

void DeployOrder::enact()
{
    /*
    the selected number of army units is added to the number of army 
    units on that territory.
    */
    target->incArmyUint(armyUnit);

    ostringstream oss;
    oss <<"success  :" << "player " <<owner->getName() << " deploy " <<armyUnit << " armys at territory "
      <<target->getName()
      << ",now " << target->getName() << " has " <<target ->getArmyUnit() << " armys.";
     effect =oss.str();
}

void DeployOrder::print(ostream& os) const
{
    Order::print(os);
    os << "deploy " << armyUnit << " army units on " << target->getName();
}

AdvanceOrder::AdvanceOrder(Player* owner, Territory* src, int n, Territory* dest)
    :Order(owner)
    , source(src)
    , armyUnit(n)
    , target(dest)
{

}

AdvanceOrder::AdvanceOrder(const AdvanceOrder& other)
    :Order(other)
    , source(other.source)
    , armyUnit(other.armyUnit)
    , target(other.target)
{
    
}

AdvanceOrder::~AdvanceOrder()
{
    
}

AdvanceOrder& AdvanceOrder::operator=(const AdvanceOrder& other) {
    if (this != &other) {
        Order::operator=(other);
        this->source = other.source;
        this->armyUnit = other.armyUnit;
        this->target = other.target;
    }
    return *this;
}

void AdvanceOrder::enact()
{
    if (target->getPlayer() == this->owner)
    {
        /*
        If the source and target territory both belong to the player that issued the order, the army units are moved
        from the source to the target territory.
        */
        this->source->decArmyUint(armyUnit);
        this->target->incArmyUint(armyUnit);

        ostringstream oss;
        oss << "success : " << "player " <<owner->getName() << " has advanced " <<armyUnit << " armys from " <<source->getName()
            << " to " <<target->getName();
        oss << ", now army number in " << source->getName() << " is " <<source->getArmyUnit();
        oss << ", now army number in " << target->getName() << " is " << target->getArmyUnit();
        effect = oss.str();
    }
    else {
        doAttack();
    }
}

bool AdvanceOrder::validate() const
{
    // If the source territory does not belong to the player that issued the order, the order is invalid.
    if (source->getPlayer() != owner)
    {
        ostringstream oss;
        oss << "failed because the source " <<  source->getName()<< " doesn't belongs to player " << owner->getName();
        effect = oss.str();
        return false;
    }
    
    // If the target territory is not adjacent to the source territory, the order is invalid.
    if (!owner->isAdjacentTo(target)){
        ostringstream oss;
        oss << "failed because the target is not adjacent to player " << owner->getName();
        effect = oss.str();
        return false;
    }
    if (source->getArmyUnit()<armyUnit){
        ostringstream oss;
        oss << "failed because in the source territory " << source->getName() 
            << ", army number = " << source->getArmyUnit() << " is less than " <<armyUnit;
        effect = oss.str();
        return false;
    }
    Player* targetOwner=target->getPlayer();
    if (targetOwner != nullptr && targetOwner != owner && (owner->isNegotiated(targetOwner) || targetOwner->isNegotiated(owner)))
    {
        ostringstream oss;
        oss << "failed because target " << target->getName()<< " is owned by another negotiated player " << targetOwner->getName();
        effect = oss.str();
        return false;
    }
        
    return true;
}

void AdvanceOrder::doAttack()
{
    ostringstream oss;
    int attackerNum = armyUnit;  // 
    source->decArmyUint(armyUnit);

    int defenderNum = this->target->getArmyUnit();

    int deadDefender = 0;
    int deadAttacker = 0;

    for (int i = 0;i < attackerNum && deadDefender < defenderNum;++i) {
        //Each attacking army unit involved has 60% chances of killing one defending army. 
        int value = rand() % 10;
        if (value < 6) {
            ++deadDefender;
        }
    }

    for (int i = 0;i < defenderNum && deadAttacker < attackerNum;++i) {
        // each defending army unit has 70 % chances of killing one attacking army unit.
        int value = rand() % 10;
        if (value < 7) {
            ++deadAttacker;
        }
    }

    attackerNum -= deadAttacker;
    defenderNum -= deadDefender;

    oss << deadDefender << "defenders was killed by attacker " << owner->getName();
    oss << deadAttacker << ",attackers was killed by defender " <<target->getPlayer()->getName();

    oss << "the remain defender is " <<defenderNum ;
    /*
    If all the defender's army units are eliminated, the attacker captures the territory. The attacking army
    units that survived the battle then occupy the conquered territory.
    */
    if (defenderNum==0) {
        owner->conqure(target);
        target->setArmyUnit(attackerNum);

        oss << "defend success : " << ",the territory " << target->getName() << " is conqured by player " <<owner->getName()
            <<",now it has " <<attackerNum << "armies";
    }
    else {
        target->setArmyUnit(defenderNum);
        source->incArmyUint(attackerNum);

        oss << "attack success : " << attackerNum << "armys move back to source " <<source->getName();
        oss << ", now army number in " << source->getName() << "is " << source->getArmyUnit();
        oss << ", now army number in " << target->getName() << "is " << target->getArmyUnit();
    }
	if (target->getPlayer())
	{


		if (dynamic_cast<NeutralPlayerStrategy*>(target->getPlayer()->ps))
		{
			delete target->getPlayer()->ps;
			target->getPlayer()->setPlayerStrategy(new AggressivePlayerStrategy());
			target->getPlayer()->ps->P = target->getPlayer();
			cout << target->getPlayer()->getName() << "'s PlayerStrategy changed" << endl;
		}
	}
    
    effect =oss.str();
}

void AdvanceOrder::print(ostream& os) const
{
    Order::print(os);
    os << "advance " << armyUnit << " army units from " << source->getName() << " to " << target->getName();
}

BombOrder::BombOrder(Player* owner, Territory* dest)
    :Order(owner)
    , target(dest)
{
    
}

BombOrder::BombOrder(const BombOrder& other)
    :Order(other)
    , target(other.target)
{

}

BombOrder::~BombOrder()
{
}

BombOrder& BombOrder::operator=(const BombOrder& other) {
    if (this != &other) {
        Order::operator=(other);
        this->target = other.target;
    }
    return *this;
}

Territory* BombOrder::getTarget() const{
    return target;
}

bool BombOrder::validate() const
{
    assert(owner != nullptr && target != nullptr);
    auto targetOwner =target->getPlayer();
    if (targetOwner == owner) {
        ostringstream oss;
        oss << "failed because dest territory " <<target->getName() << " has owned by " << owner->getName();
        effect = oss.str();
        return false;
    }
    if (!owner->isAdjacentTo(target)) {
        ostringstream oss;
        oss << "failed because dest territory " << target->getName() << " is not adjacent to player" << owner->getName();
        effect = oss.str();
        return false;
    }
    if (targetOwner != nullptr && targetOwner != owner && (owner->isNegotiated(targetOwner) || targetOwner->isNegotiated(owner))){
        ostringstream oss;
        oss << "failed because player " << owner->getName() << " is negotiated with " << targetOwner->getName();
        effect = oss.str();
        return false;
    }
    return true;
}

void BombOrder::enact()
{
    target->decHalf();
    ostringstream oss;
    if (target->isEmpty()){
        owner->conqure(target);
        
        oss << "all army at  " << target->getName() 
            << " is killed, now it is conquaed by new owner" 
            <<owner->getName();
    }
    else
    {
        oss << " the army units located on " << target->getName() << " is half decreased, the remain is "
           <<target->getArmyUnit();
    }
    effect = oss.str();
}

void BombOrder::print(ostream& os) const
{
    Order::print(os);
    os << "destroy half of the army units located on " << target->getName();
}

BlockadeOrder::BlockadeOrder(Player* owner, Territory* dest)
    :Order(owner)
    , target(dest)
{
    
}

BlockadeOrder::BlockadeOrder(const BlockadeOrder& other)
    :Order(other)
    , target(other.target)
{

}

BlockadeOrder::~BlockadeOrder()
{
}

BlockadeOrder& BlockadeOrder::operator=(const BlockadeOrder& other) {
    if (this != &other) {
        Order::operator=(other);
        this->target = other.target;
    }
    return *this;
}

bool BlockadeOrder::validate() const
{
    assert(owner != nullptr && target != nullptr);
    if (target->getPlayer() != owner)
    {
        ostringstream oss;
        oss << "failed becuase owner the target " <<target->getName() << " is " << target->getPlayer()->getName();
        effect = oss.str();
        return false;
    }
    return true;
}

void BlockadeOrder::enact()
{
    target->doubled();
    auto p = new NeutralPlayer();
    p->add(target);
    
    ostringstream oss;
    oss <<"success : " << target->getName() <<" has been blockaded,now it has " <<target->getArmyUnit() << " armys";
    effect =oss.str();
}

void BlockadeOrder::print(ostream& os) const
{
    Order::print(os);
    os << "blockade " << target->getName();
}

AirliftOrder::AirliftOrder(Player* owner, Territory* src, int n, Territory* dest)
    :Order(owner)
    , source(src)
    , armyUnit(n)
    , target(dest)
{
    
}

AirliftOrder::AirliftOrder(const AirliftOrder& other)
    :Order(other)
    , source(other.source)
    , armyUnit(other.armyUnit)
    , target(other.target)
{

}

AirliftOrder::~AirliftOrder()
{

}

AirliftOrder& AirliftOrder::operator=(const AirliftOrder& other) {
    if (this != &other) {
        Order::operator=(other);
        this->source = other.source;
        this->armyUnit = other.armyUnit;
        this->target = other.target;
    }
    return *this;
}

bool AirliftOrder::validate() const
{
    assert(owner!=nullptr && this->source!= nullptr && target!=nullptr);
    if (source->getPlayer() != owner)
    {
        ostringstream oss;
        oss << "failed because the source "
        <<source->getName() <<"belongs to other player " << source->getPlayer()->getName();
        effect = oss.str();
        return false;
    }
    if (target->getPlayer() != owner) {
        ostringstream oss;
        oss << "failed because the target "
            << target->getName() << "belongs to other player " << target->getPlayer()->getName();
        effect = oss.str();
        return false;
    }
    if (source->getArmyUnit() < armyUnit) {
        ostringstream oss;
        oss << "failed because the number of army at "
            << source->getName() << " is " << source->getArmyUnit() << ", less than"
            <<armyUnit;
        effect = oss.str();

        return false;
    }
    return true;
}

void AirliftOrder::enact()
{
    /*
    If both the source and target territories belong to the player that issue the airlift order, then the selected
    number of army units is moved from the source to the target territory.
    */
    this->source->decArmyUint(armyUnit);
    this->target->incArmyUint(armyUnit);

    ostringstream oss;
    oss << "success : "
        << "player" << "has airlifted " << armyUnit << " armys from " << source->getName()
        << " to " << target->getName();
    oss << ", now army number in " << source->getName() << "is " << source->getArmyUnit();
    oss << ", now army number in " << target->getName() << "is " << target->getArmyUnit();
    effect = oss.str();
}

void AirliftOrder::print(ostream& os) const
{
    Order::print(os);
    os << "move " << armyUnit<< " army units from " << source->getName() << " to " << target->getName();
}

NegotiateOrder::NegotiateOrder(Player* owner, Player* other)
    :Order(owner)
    , destPlayer(other)
{

}

NegotiateOrder::NegotiateOrder(const NegotiateOrder& other)
    :Order(other)
    , destPlayer(other.destPlayer)
{

}

NegotiateOrder::~NegotiateOrder()
{

}

NegotiateOrder& NegotiateOrder::operator=(const NegotiateOrder& other) {
    if (this != &other) {
        Order::operator=(other);
        destPlayer = other.destPlayer;
    }

    return *this;
}

bool NegotiateOrder::validate() const
{
    assert(destPlayer!=nullptr);

    if (destPlayer == owner) {
        ostringstream oss;
        oss << "failed because the dest player is same as owner";
        effect =oss.str();
        return false;
    }
    if (destPlayer->isNeutral()) {
        ostringstream oss;
        oss << "failed because the dest player is neutral";
        effect = oss.str();
        return false;
    }
    return true;
}

void NegotiateOrder::enact()
{
    this->owner->addNegotiated(this->destPlayer);
    ostringstream oss;
    oss << "success : " << owner->getName() <<" has negotiate with " << destPlayer->getName();
    effect = oss.str();
}

void NegotiateOrder::print(ostream& os) const
{
    Order::print(os);
    os << "negotiate with " << destPlayer->getName();
}


