#include <iostream>
#include <string>
#include <assert.h>
using std::cout;
using std::endl;
using std::string;

#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "GameEngine.h"

static string tab(4, ' ');

void testOrdersLists() {
    cout << "\nbegin of testOrdersLists" << endl;
    MapLoader loader;
    loader.load("1.map");
    auto map = loader.getMap();
    const vector<Territory*>& obj = map->getNodes();

    Player playerA("Tom");



    Player playerB("Jerry");
    playerA.issueOrder(3, obj[0]);
    playerA.issueOrder(obj[1], 10, obj[2]);
    playerA.issueOrder(AIRLIFT,obj[3], 10, obj[4]);
    playerA.issueOrder(BOMB,obj[5]);
    playerA.issueOrder(BLOCKADE,obj[6]);
    playerA.issueOrder(&playerB);

    cout << playerA.getName() << "'s order:" << endl;
    OrderList* orders = playerA.getOrders();

    cout << "will execute all of " << playerA.getName() << "'s orders " << endl;
    for (const auto x : *orders) {
        x->enact();
        cout << endl;
    }

    Order* t0 = playerB.issueOrder(3, obj[7]);
    Order* t1 = playerB.issueOrder(obj[8], 10, obj[9]);
    playerB.issueOrder(AIRLIFT,obj[10], 10, obj[11]);
    playerB.issueOrder(BOMB,obj[12]);
    playerB.issueOrder(BLOCKADE,obj[13]);
    playerB.issueOrder(&playerA);
    cout << playerB.getName() << "'s order:" << endl;
    OrderList* ordersB = playerB.getOrders();
    for (const auto x : *ordersB) {
        cout << (*x) << endl;
    }
    ordersB->move(t1, 2);
    cout << "after move second order two positions," << playerB.getName() << "'s order:" << endl;
    for (const auto x : *ordersB) {
        cout << (*x) << endl;
    }

    ordersB->remove(t0);
    cout << "after remove first order," << playerB.getName() << "'s order:" << endl;
    for (const auto x : *ordersB) {
        cout << (*x) << endl;
    }

    cout << tab << "end of testOrdersLists" << endl;
}



void testDeployOrderValidate1() {
    Territory* t = new Territory("t1");
    assert(t->getArmyUnit() == 0);

    Player* p1 = new Player("Tom");
    p1->incArmyUnit(100);
    assert(p1->getArmyUnit() == 100);

    Player* p2 = new Player("Tom");
    p2->add(t);


    Order* o = new DeployOrder(p1, 10, t);
    assert(o->getOwner() == p1);
    assert(o->validate()==false);

    cout << tab << "testDeployOrderValidate1 success" <<endl;
}

#if 0
void testDeployOrderValidate2() {
    Territory* t = new Territory("t1");
    Player* p = new Player("Tom");
    p->incArmyUnit(5);
    p->add(t);
    Order* o = new DeployOrder(p, 10, t);
    assert(o->validate()==false);

    cout << tab << "testDeployOrderValidate2 success" << endl;
}
#endif

#if 0
void testDeployOrderExecute1() {
    Territory* t = new Territory("t1");

    Player* p1 = new Player("Tom");
    p1->incArmyUnit(100);

    Player* p2 = new Player("Tom");
    p2->add(t);

    Order* o = new DeployOrder(p1, 10, t);
    assert(o->execute()==false);

    assert(t->getArmyUnit() == 0);
    assert(p1->getTerritories().empty());
    assert(p1->getArmyUnit() == 100);

    cout << tab << "testDeployOrderExecute1 success" << endl;
}
#endif

#if 0
void testDeployOrderExecute2() {
    Territory* t = new Territory("t1");
    assert(t->getArmyUnit() == 0);

    Player* p = new Player("Tom");
    p->incArmyUnit(5);
    p->add(t);
    Order* o = new DeployOrder(p, 10, t);
    assert(o->execute() == false);

    assert(t->getArmyUnit() == 0);
    assert(p->getArmyUnit()==5);

    cout << tab << "testDeployOrderExecute2 success" << endl;
}
#endif

void testDeployOrderValidate3() {
    Territory* t = new Territory("t1");
    Player* p = new Player("Tom");
    p->incArmyUnit(100);
    p->add(t);
    Order* o = new DeployOrder(p, 10, t);
    assert(o->validate());

    cout <<tab << "testDeployOrderValidate3 success" << endl;
}


void testDeployOrderExecute3() {
    
    Territory* t = new Territory("t1");
    Player* p = new Player("Tom");
    
    Order* o =new DeployOrder(p,10,t);
    p->add(t);
    assert(o->execute());

    assert(t->getPlayer()==p);
    assert(t->getArmyUnit()==10);
    assert(p->getTerritories().size()==1);

    cout << tab << "testDeployOrderExecute3 success" <<endl;
}


void testDeployOrder() {
    testDeployOrderValidate1();
    //testDeployOrderValidate2();
    testDeployOrderValidate3();
    //testDeployOrderExecute1();
    //testDeployOrderExecute2();
    testDeployOrderExecute3();
    cout << tab << "testDeployOrderExecute success" << endl;
}


void testAirliftOrder1() {
    Territory* t1 = new Territory("t1");
    Territory* t2 = new Territory("t2");
    Player* p1 = new Player("p1");
    Player* p2 = new Player("p2");
    p1->add(t1);
    p2->add(t2);

    Order *order = new AirliftOrder(p1,t1,5,t2);
    assert(order->execute()==false);   // fail becuase targer not belongs to p1

    cout << tab << "testAirliftOrder1 success" << endl;
}

void testAirliftOrder2() {
    Territory* t1 = new Territory("t1");
    Territory* t2 = new Territory("t2");
    Player* p1 = new Player("p1");
    Player* p2 = new Player("p2");
    p1->add(t1);
    p2->add(t2);

    Order* order = new AirliftOrder(p2, t1, 5, t2);
    assert(order->execute() == false);   // fail becuase source not belongs to p1

    cout << tab << "testAirliftOrder2 success" << endl;
}

void testAirliftOrder3() {
    Territory* t1 = new Territory("t1");
    Territory* t2 = new Territory("t2");
    Player* p1 = new Player("p1");
    Player* p2 = new Player("p2");
    p1->add(t1);
    p1->add(t2);

    t1->setArmyUnit(5);
    Order* order = new AirliftOrder(p1, t1, 10, t2);
    assert(order->execute() == false);   // fail becuase the number of army in source is less than required

    cout << tab << "testAirliftOrder3 success" << endl;
}

void testAirliftOrder4() {
    Territory* t1 = new Territory("t1");
    Territory* t2 = new Territory("t2");
    Player* p1 = new Player("p1");
    p1->add(t1);
    p1->add(t2);

    t1->setArmyUnit(10);
    Order* order = new AirliftOrder(p1, t1, 10, t2);
    assert(order->execute() == true);   

    assert(t1->getArmyUnit()==0);
    assert(t2->getArmyUnit() == 10);

    cout << tab << "testAirliftOrder4 success" << endl;
}


void testBombOrder1() {
    Territory* t1 = new Territory("t1");
    Territory* t2 = new Territory("t2");
    assert(t2->getPlayer()==nullptr);
    Player* p1 = new Player("p1");
    assert(p1->hasConquered() == false);

    t2->setArmyUnit(10);
    t1->addNeighbour(t2);
    t2->addNeighbour(t1);

    p1->add(t1);
    
    Order* order = new BombOrder(p1, t2);
    assert(order->execute());
    assert(t2->getArmyUnit()==5);
    assert(order->execute());
    assert(t2->getArmyUnit() == 2);
    assert(order->execute());
    assert(t2->getArmyUnit() == 1);
    assert(order->execute());
    assert(t2->getArmyUnit() == 0);
    assert(t2->getPlayer()==p1);
    assert(p1->hasConquered()==true);

    cout << tab << "testBombOrder1 success" << endl;
}


void testBombOrder2() {
    Territory* t1 = new Territory("t1");
    Territory* t2 = new Territory("t2");
    Player* p1 = new Player("p1");

    t2->setArmyUnit(10);
    
    p1->add(t1);
    p1->add(t2);

    Order* order = new BombOrder(p1, t2);
    assert(order->execute()==false);  //t2 belongs to p1
    
    cout << tab << "testBombOrder2 success" << endl; 
}

void testBombOrder3() {
    Territory* t1 = new Territory("t1");
    Territory* t2 = new Territory("t2");
    Player* p1 = new Player("p1");

    t2->setArmyUnit(10);

    p1->add(t1);
    t1->addNeighbour(t2);
    t2->addNeighbour(t2);
    p1->add(t2);

    Order* order = new BombOrder(p1, t2);
    assert(order->execute() == false);   //t2 belongs to p1

    cout << tab << "testBombOrder3 success" << endl;
}

void testBombOrder4() {
    Territory* t1 = new Territory("t1");
    Territory* t2 = new Territory("t2");
    Player* p1 = new Player("p1");

    t2->setArmyUnit(10);

    p1->add(t1);

    Order* order = new BombOrder(p1, t2);
    assert(order->execute() == false);  // t2 not adjacent to p1

    cout << tab << "testBombOrder4 success" << endl;
}


void testBlockadeOrder1() {
    Territory* t1 = new Territory("t1");
    t1->setArmyUnit(5);

    Player* p1 = new Player("p1");
    p1->add(t1);


    Order* order = new BlockadeOrder(p1, t1);
    assert(order->execute());  

    assert(t1->getPlayer() != p1);
    assert(!p1->owns(t1));
    assert(p1->getTerritories().size()==0);
    assert(t1->getPlayer()->isNeutral());
    assert(t1->getArmyUnit()==10);

    cout << tab << "testBlockadeOrder1 success" << endl;

}

void testBlockadeOrder2() {
    Territory* t1 = new Territory("t1");
    t1->setArmyUnit(5);

    Player* p1 = new Player("p1");
    Player* p2 = new Player("p2");
    p2->add(t1);

    Order* order = new BlockadeOrder(p1, t1);
    assert(order->execute()==false);  //t1 not belongs to p1

    cout << tab << "testBlockadeOrder2 success" << endl;
}

void testAdvanceOrder1() {
    Territory* t1 = new Territory("t1");
    Territory* t2 = new Territory("t1");
    Player* p1 = new Player("p1");
    Order* order = new AdvanceOrder(p1, t1,10,t2);
    assert(order->execute() == false);  //t1 not belongs to p1
    cout << tab << "testAdvanceOrder1 success" << endl;
}

void testAdvanceOrder2() {
    Territory* t1 = new Territory("t1");
    Territory* t2 = new Territory("t2");
    Player* p1 = new Player("p1");
    p1->add(t1);
    p1->add(t2);
    Order* order = new AdvanceOrder(p1, t1, 10, t2);
    assert(order->execute() == false);  //t2 not adjacent to t1
    cout << tab << "testAdvanceOrder2 success" << endl;
}

void testAdvanceOrder3() {
    Territory* t1 = new Territory("t1");
    Territory* t2 = new Territory("t2");
    Player* p1 = new Player("p1");
    p1->add(t1);
    p1->add(t2);
    t1->addNeighbour(t2);
    t2->addNeighbour(t1);
    assert(t1->isAdjacentTo(t2));
    assert(t2->isAdjacentTo(t1));

    Order* order = new AdvanceOrder(p1, t1, 10, t2);
    assert(order->execute() == false);  //t1 not has enough armyunit
    cout << tab << "testAdvanceOrder3 success" << endl;
}

void testAdvanceOrder4() {
    Territory* t1 = new Territory("t1");
    Territory* t2 = new Territory("t2");
    Player* p1 = new Player("p1");
    p1->add(t1);
    p1->add(t2);
    t1->addNeighbour(t2);
    t2->addNeighbour(t1);
    assert(t1->isAdjacentTo(t2));
    assert(t2->isAdjacentTo(t1));
    t1->setArmyUnit(10);
    t2->setArmyUnit(5);

    Order* order = new AdvanceOrder(p1, t1, 10, t2);
    assert(order->execute());
    assert(t1->isEmpty());
    assert(t2->getArmyUnit()==15);

    cout << tab << "testAdvanceOrder4 success" << endl;
}

void testAttack(int n, int m) {
    Territory* t1 = new Territory("t1");
    Territory* t2 = new Territory("t2");
    Player* p1 = new Player("p1");
    Player* p2 = new Player("p2");
    p1->add(t1);
    p2->add(t2);
    t1->addNeighbour(t2);
    t2->addNeighbour(t1);
    assert(t1->isAdjacentTo(t2));
    assert(t2->isAdjacentTo(t1));
    t1->setArmyUnit(n);
    t2->setArmyUnit(m);

    Order* order = new AdvanceOrder(p1, t1, n, t2);

    cout << "before attack,t2 is owned by " << t2->getPlayer()->getName() << endl;
    cout << "before attack,t2 has " << t2->getArmyUnit() << " armyunit" << endl;
    cout << "before attack,t1 has " << t1->getArmyUnit() << " armyunit" << endl;
    cout << "now begin advance " <<n  << " armyunit from t1 to t2" <<endl;

    assert(order->execute());


    if (t2->getPlayer() == p1) {
        assert(t1->isEmpty());
        assert(p1->hasConquered());
    }

    cout << "after attack,t2 is owned by " << t2->getPlayer()->getName() << endl;
    cout << "after attack,t2 has " << t2->getArmyUnit() << " armyunit" << endl;
    cout << "after attack,t1 has " << t1->getArmyUnit() << " armyunit" << endl;
}
void testAdvanceOrder5() {
    testAttack(10,1);
    cout << tab << "testAdvanceOrder5 success" << endl;
}
void testAdvanceOrder6() {
    testAttack(10, 5);
    cout << tab << "testAdvanceOrder6 success" << endl;
}
void testAdvanceOrder7() {
    testAttack(10, 10);
    cout << tab << "testAdvanceOrder7 success" << endl;
}

void testNegotiateOrder1() {
    Player* p1 = new Player("p1");
    Order* order = new NegotiateOrder(p1, p1);
    assert(order->execute()==false);

    cout << tab << "testNegotiateOrder1 success" << endl;
}

void testNegotiateOrder2() {
    Player* p1 = new Player("p1");
    Player* p2 = new NeutralPlayer();
    Order* order = new NegotiateOrder(p1, p2);
    assert(order->execute() == false);

    cout << tab << "testNegotiateOrder2 success" << endl;
}

void testNegotiateOrder3() {
    Territory* t1 = new Territory("t1");
    Territory* t2 = new Territory("t2");
    Player* p1 = new Player("p1");
    Player* p2 = new Player("p2");
    p1->add(t1);
    p2->add(t2);
    t1->addNeighbour(t2);
    t2->addNeighbour(t1);
    t1->setArmyUnit(10);
    t2->setArmyUnit(10);

    Order *order2 = new AdvanceOrder(p1,t1,5,t2);
    assert(order2->validate());
    Order* order3 = new AdvanceOrder(p2, t2, 5, t1);
    assert(order3->validate());

    Order* order1 = new NegotiateOrder(p1, p2);
    assert(order1->execute());

    assert(order2->execute()==false);
    assert(order3->execute() == false);

    cout << tab << "testNegotiateOrder3 success" << endl;
}

void testNegotiateOrder4() {
    Territory* t1 = new Territory("t1");
    Territory* t2 = new Territory("t2");
    Player* p1 = new Player("p1");
    Player* p2 = new Player("p2");
    p1->add(t1);
    p2->add(t2);
    t1->addNeighbour(t2);
    t2->addNeighbour(t1);
    t1->setArmyUnit(10);
    t2->setArmyUnit(10);

    Order* order2 = new BombOrder(p1, t2);
    assert(order2->validate());
    Order* order3 = new BombOrder(p2, t1);
    assert(order3->validate());

    Order* order1 = new NegotiateOrder(p1, p2);
    assert(order1->execute());

    assert(order2->execute() == false);
    assert(order3->execute() == false);
    cout << tab << "testNegotiateOrder4 success" << endl;
}


void testNegotiateOrder() {
    testNegotiateOrder1();
    testNegotiateOrder2();
    testNegotiateOrder3();
    testNegotiateOrder4();
    cout << tab << "testNegotiateOrder success" << endl;
}

void testAdvanceOrder() {
    testAdvanceOrder1();
    testAdvanceOrder2();
    testAdvanceOrder3();
    testAdvanceOrder4();
    testAdvanceOrder5();
    testAdvanceOrder6();
    testAdvanceOrder7();
    cout << tab << "testAdvanceOrder success" << endl;
}

void testBlockadeOrder() {
    testBlockadeOrder1();
    testBlockadeOrder2();

    cout << tab << "testBlockadeOrder success" << endl;
}

void testBombOrder() {
    testBombOrder1();
    testBombOrder2();
    testBombOrder3();
    testBombOrder4();
    cout << tab << "testBombOrder success" << endl;
}

void testAirliftOrder() {
    testAirliftOrder1();
    testAirliftOrder2();
    testAirliftOrder3();
    testAirliftOrder4();
    cout <<tab << "testAirliftOrder success" << endl;
}

void testExecuteOrderParse1() {
    GameEngine ge;
    ge.loadMap("map10.txt");
    ge.validatemap();

    ge.addPlayer("p1");
    ge.addPlayer("p2");
    ge.startGame();

    ge.reinforcementPhase();

    cout << "after reinforcementPhase"<<endl;
     for (auto x : ge.getPlayers()) {
        cout <<(*x) <<endl;
    }

    ge.issueOrdersPhase();

    cout << "after issurOrderPhase" << endl;
    for (auto x : ge.getPlayers()) {
        cout << (*x) << endl;
    }

    ge.executeOrdersPhase();

    cout << "after executeOrderPhase" << endl;
    for (auto x : *(ge.orderRecords)) {
        cout << (*x) <<endl;
        cout <<"effect " <<x->getEffect() <<endl;
    }

    cout << tab << "testExecuteOrderParse1 success" <<endl;
}

void testOrderExecution() {
    cout << "\nbegin of testOrderExecution" <<endl;
    testDeployOrder();
    cout <<endl;
    testAirliftOrder();
    cout << endl;
    testBombOrder();
    cout << endl;
    testBlockadeOrder();
    cout << endl;
    testAdvanceOrder();
    cout << endl;
    testNegotiateOrder();
    cout << endl;
    testExecuteOrderParse1();
    cout << "testOrderExecution success" << endl;
}

