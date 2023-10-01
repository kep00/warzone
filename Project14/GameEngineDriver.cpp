#include <cassert>
#include <iostream>
#include <sstream>
#include<string>
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::istringstream;

#include "Cards.h"
#include "Orders.h"
#include "GameEngine.h"
void testGameStates() {
	cout << "\nbegin of testGameStates" << endl;
	GameEngine ge;
	cout << "current state: ";
	cout << ge << endl;
	while (ge.getState() != GAME_END)
	{
		cout << "please input command:";
		string line;
		getline(cin, line);
		ge.process(line);
		cout << "current state: ";
		cout << ge << endl;
	}
	cout << "end of testGameStates" << endl;
}


static string tab(4,' ');

// test for map
void startupSimpleTest1() {
	cout << tab << "start of startupSimpleTest1" <<endl;
	GameEngine ge;

	assert(ge.getMap()==nullptr);
	assert(ge.getPlayers().empty());
	assert(ge.getDeck()->empty());
	assert(ge.getCommandProcessor()==nullptr);

	{
		auto c =new Command(LOAD_MAP,"3.map");
		ge.process(c);
		assert(ge.getMap() != nullptr);
		assert(ge.getState()==MAP_LOADED);
	}
	{
		auto c = new Command(VALIDATE_MAP);
		ge.process(c);
		assert(ge.getState() == MAP_LOADED);
	}
	
	{
		auto c = new Command(LOAD_MAP, "2.map");
		ge.process(c);
		assert(ge.getMap() != nullptr);
		assert(ge.getState() == MAP_LOADED);
	}
	{
		auto c = new Command(VALIDATE_MAP);
		ge.process(c);
		assert(ge.getState() == MAP_VALIDATED);
	}

	cout << tab << "startupSimpleTest1 success" << endl;
}


// test for player
void startupSimpleTest2() {
	cout << tab << "start of startupSimpleTest2" << endl;
	GameEngine ge;

	ge.transition(MAP_VALIDATED);
	ge.addPlayer("A");
	ge.addPlayer("B");
	ge.addPlayer("C");
	ge.addPlayer("D");

	assert(ge.getPlayers().size()==4);

	const auto& players = ge.getPlayers();
	cout << "before shuffle players, player list is:" << endl;
	for (auto x : players) {
		cout << x->getName();
	}
	cout << endl;
	ge.shufflePlayers();
	
	cout << "after shuffle players, player list is:" <<endl;
	for (auto x : players) {
		cout << x->getName();
	}
	cout <<endl;

	ge.initArmy();
	for (auto x : players) {
		assert(x->getArmyUnit()==50);
	}

	ge.initDeck();
	ge.initHand();
	for (auto x : players) {
		const auto& hand = *(x->getCards());
		cout << "player " <<x->getName() << "'s hand has "<<hand.size() << " cards" <<endl;
		cout << hand <<endl;
	}

	cout << tab << "startupSimpleTest2 success" << endl;
}

void startupFullTest1() {
	cout << tab << "start of startupFullTest1" << endl;
	

	ostringstream oss;
	oss << "loadmap 3.map" << endl;
	oss << "validatemap" << endl;
	oss << "loadmap 1.map" << endl;
	oss << "validatemap" << endl;
	oss << "addplayer A" << endl;
	oss << "addplayer B" << endl;
	oss << "addplayer C" << endl;
	oss << "addplayer D" << endl;
	oss << "addplayer E" << endl;
	oss << "gamestart" << endl;
	

	istringstream iss(oss.str());
	auto oldBuffer = cin.rdbuf(iss.rdbuf());
	
	GameEngine ge;
	CommandProcessor *cp = new CommandProcessor();
	ge.setCommandProcessor(cp);
	assert(ge.getCommandProcessor()==cp);
	ge.startupPhase();
	auto map =ge.getMap();
	auto players = ge.getPlayers();
	auto deck = ge.getDeck();
	assert(map!=nullptr);
	assert(players.size()==5);
	assert(ge.getState()==ASSIGN_REINFORCEMENT);

	for (auto x : players) {
		assert(x->getArmyUnit() == 50);
	}

	cout << "after shuffle players, player list is:" << endl;
	for (auto x : players) {
		cout << x->getName();
	}
	cout << endl;

	for (char c = 'A';c <= 'E';++c) {
		string name(1,c);
		auto x =ge.findPlayer(name);
		cout << "player " << x->getName() << " has " << x->getArmyUnit() << " army units in its pool" << endl;
		
		auto territories = x->getTerritories();
		cout << "player " << x->getName() << " has " << territories.size() << " territories" << endl;
		for (auto y : territories) {
			cout << y->getName() << " ";
		}
		cout << endl;
		auto hand = x->getCards();
		cout << "player " << x->getName() << "'s hand has " << hand->size() << " cards" << endl;
		cout << (*hand) << endl;
	}

	cin.rdbuf(oldBuffer);
	cout << tab << "startupFullTest1 success" << endl;
}

void testStartupPhase()
{
	cout << "begin of testStartupPhase" << endl;
	startupSimpleTest1();
	startupSimpleTest2();
	startupFullTest1();
	cout << "testStartupPhase success" << endl;
}

void testCalArmyPool1() {
	Territory* t1 = new Territory("t1");
	t1->setArmyUnit(2);
	Territory* t2 = new Territory("t2");
	t2->setArmyUnit(2);
	Territory* t3 = new Territory("t3");
	t3->setArmyUnit(3);
	
	Continent* c1 = new Continent("c1", 5);
	
	c1->add(t1);
	c1->add(t2);
	c1->add(t3);

	Player* p1 = new Player("p1");
	p1->add(t1);
	p1->add(t2);

	assert(p1->calArmyPool()==3);

	cout << tab << "testCalArmyPool1 success" << endl;
}

void testCalArmyPool2() {
	Player* p1 = new Player("p1");
	Continent* c1 = new Continent("c1", 5);
	for(int i=0;i<15;++i){
		auto x= new Territory("t");
		c1->add(x);
		if (i<12)
			p1->add(x);
	}
	assert(p1->calArmyPool() == 4);

	cout << tab << "testCalArmyPool2 success" << endl;
}

void testCalArmyPool3() {
	Player* p1 = new Player("p1");
	Continent* c1 = new Continent("c1", 5);
	Continent* c2 = new Continent("c2", 8);
	for (int i = 0;i < 15;++i) {
		auto x = new Territory("t");
		c1->add(x);
		p1->add(x);
	}
	for (int i = 0;i < 2;++i) {
		auto x = new Territory("t");
		c2->add(x);
		p1->add(x);
	}

	assert(p1->calArmyPool() == 18);

	cout << tab << "testCalArmyPool3 success" << endl;
}



void testCalArmyPool() {
	testCalArmyPool1();
	testCalArmyPool2();
	testCalArmyPool3();
	cout <<tab << "testCalArmyPool success" <<endl;
}

void testReinforcementPhase1() {
	GameEngine ge;
	ge.loadMap("map10.txt");

	auto map = ge.getMap();
	assert(map->getContinents().size()==3);
	assert(map->getNodes().size()==10);
	assert(map->findTerritory("1")!=nullptr);

	Player* p1=new Player("p1");
	Player* p2 = new Player("p2");
	

	p1->add(map->findTerritory("1"));
	p1->add(map->findTerritory("2"));
	p1->add(map->findTerritory("3"));
	p1->add(map->findTerritory("4"));
	p1->add(map->findTerritory("5"));
	p1->add(map->findTerritory("6"));

	p2->add(map->findTerritory("7"));
	p2->add(map->findTerritory("8"));
	p2->add(map->findTerritory("9"));
	p2->add(map->findTerritory("10"));

	ge.addPlayer(p1);
	ge.addPlayer(p2);

	ge.initArmy();

	p1->addNegotiated(p2);
	p2->addNegotiated(p1);

	assert(p1->isNegotiated(p2));
	assert(p2->isNegotiated(p1));


	ge.reinforcementPhase();
	assert(p1->getArmyUnit()==50+10+12+2);
	assert(p2->getArmyUnit() == 53);

	assert(!p1->isNegotiated(p2));
	assert(!p2->isNegotiated(p1));

	cout << tab << "testReinforcementPhase1 success" << endl;
}

void testReinforcementPhase() {
	testReinforcementPhase1();
	cout << tab << "testReinforcementPhase success" << endl;
}

void testDefendList() {
	GameEngine ge;
	ge.loadMap("map10.txt");

	auto map = ge.getMap();
	assert(map->getContinents().size() == 3);
	assert(map->getNodes().size() == 10);
	assert(map->findTerritory("1") != nullptr);

	Player* p1 = new Player("p1");
	Player* p2 = new Player("p2");


	p1->add(map->findTerritory("1"));
	p1->add(map->findTerritory("2"));
	p1->add(map->findTerritory("3"));
	p1->add(map->findTerritory("4"));
	p1->add(map->findTerritory("5"));
	p1->add(map->findTerritory("6"));

	p2->add(map->findTerritory("7"));
	p2->add(map->findTerritory("8"));
	p2->add(map->findTerritory("9"));
	p2->add(map->findTerritory("10"));

	ge.addPlayer(p1);
	ge.addPlayer(p2);


	p1->rebuildDefendList();
	p2->rebuildDefendList();

	cout << "p1's defend list" <<endl;
	for (auto x : p1->toDefend()) {
		cout << x->getName() <<" ";
	}
	cout <<endl;

	cout << "p2's defend list" << endl;
	for (auto x : p2->toDefend()) {
		cout << x->getName() << " ";
	}
	cout << endl;

	cout <<tab << "testDefendList success" <<endl;
}

void testAttackList() {
	GameEngine ge;
	ge.loadMap("map10.txt");

	auto map = ge.getMap();
	assert(map->getContinents().size() == 3);
	assert(map->getNodes().size() == 10);
	assert(map->findTerritory("1") != nullptr);

	Player* p1 = new Player("p1");
	Player* p2 = new Player("p2");


	p1->add(map->findTerritory("1"));
	p1->add(map->findTerritory("2"));
	p1->add(map->findTerritory("3"));
	p1->add(map->findTerritory("4"));
	p1->add(map->findTerritory("5"));
	p1->add(map->findTerritory("6"));

	p2->add(map->findTerritory("7"));
	p2->add(map->findTerritory("8"));
	p2->add(map->findTerritory("9"));
	p2->add(map->findTerritory("10"));

	ge.addPlayer(p1);
	ge.addPlayer(p2);


	p1->rebuildAttackList();
	p2->rebuildAttackList();

	cout << "p1's attack list" << endl;
	for (auto x : p1->toAttack()) {
		cout << x->getName() << " ";
	}
	cout << endl;

	cout << "p2's attack list" << endl;
	for (auto x : p2->toAttack()) {
		cout << x->getName() << " ";
	}
	cout << endl;

	cout << tab << "testAttackList success" << endl;
}

//test for issue deploy order
void testIssueOrder1() {
	GameEngine ge;
	ge.loadMap("map10.txt");

	auto map = ge.getMap();
	assert(map->getContinents().size() == 3);
	assert(map->getNodes().size() == 10);
	assert(map->findTerritory("1") != nullptr);

	Player* p1 = new Player("p1");
	Player* p2 = new Player("p2");


	p1->defend(map->findTerritory("1"));
	p1->defend(map->findTerritory("2"));
	p1->defend(map->findTerritory("3"));
	p1->defend(map->findTerritory("4"));
	p1->defend(map->findTerritory("5"));
	p1->defend(map->findTerritory("6"));
	p1->attack(map->findTerritory("7"));

	p2->defend(map->findTerritory("7"));
	p2->defend(map->findTerritory("8"));
	p2->defend(map->findTerritory("9"));
	p2->defend(map->findTerritory("10"));
	p2->attack(map->findTerritory("6"));

	ge.addPlayer(p1);
	ge.addPlayer(p2);

	ge.initArmy();

	{
		auto order = ge.issueOrder(p1);
		cout <<*order <<endl;
		assert(dynamic_cast<DeployOrder*>(order) != nullptr);
	}
	{
		auto order = ge.issueOrder(p2);
		cout << *order << endl;
		assert(dynamic_cast<DeployOrder*>(order) != nullptr);
	}

	cout <<tab << "testIssueOrder1 success"  <<endl;
}

//test for issue advance order
void testIssueOrder2() {
	GameEngine ge;
	ge.loadMap("map10.txt");

	auto map = ge.getMap();
	assert(map->getContinents().size() == 3);
	assert(map->getNodes().size() == 10);
	assert(map->findTerritory("1") != nullptr);

	Player* p1 = new Player("p1");
	Player* p2 = new Player("p2");


	p1->defend(map->findTerritory("1"));
	p1->defend(map->findTerritory("2"));
	p1->defend(map->findTerritory("3"));
	p1->defend(map->findTerritory("4"));
	p1->defend(map->findTerritory("5"));
	p1->defend(map->findTerritory("6"));
	p1->attack(map->findTerritory("7"));

	p2->defend(map->findTerritory("7"));
	p2->defend(map->findTerritory("8"));
	p2->defend(map->findTerritory("9"));
	p2->defend(map->findTerritory("10"));
	p2->attack(map->findTerritory("6"));

	ge.addPlayer(p1);
	ge.addPlayer(p2);

	for (int i=0;i<6;++i)
	{
		auto order = ge.issueOrder(p1);
		cout << *order << endl;
		assert(dynamic_cast<AdvanceOrder*>(order) != nullptr);
	}

	for (int i = 0;i < 4;++i)
	{
		auto order = ge.issueOrder(p2);
		cout << *order << endl;
		assert(dynamic_cast<AdvanceOrder*>(order) != nullptr);
	}

	cout << tab << "testIssueOrder2 success" << endl;
}

//test for issue order need card
void testIssueOrder3() {
	GameEngine ge;
	ge.loadMap("map10.txt");

	auto map = ge.getMap();
	assert(map->getContinents().size() == 3);
	assert(map->getNodes().size() == 10);
	assert(map->findTerritory("1") != nullptr);

	Player* p1 = new Player("p1");
	Player* p2 = new Player("p2");

	auto t1 = map->findTerritory("1");
	auto t2 = map->findTerritory("2");
	auto t3 = map->findTerritory("3");
	auto t4 = map->findTerritory("4");
	auto t5 = map->findTerritory("5");
	auto t6 = map->findTerritory("6");
	auto t7 = map->findTerritory("7");
	auto t8 = map->findTerritory("8");
	auto t9 = map->findTerritory("9");
	auto t10 = map->findTerritory("10");

	p1->defend(map->findTerritory("1"));
	p1->defend(map->findTerritory("2"));
	p1->defend(map->findTerritory("3"));
	p1->defend(map->findTerritory("4"));
	p1->defend(map->findTerritory("5"));
	p1->defend(map->findTerritory("6"));
	p1->attack(map->findTerritory("7"));

	p2->defend(map->findTerritory("7"));
	p2->defend(map->findTerritory("8"));
	p2->defend(map->findTerritory("9"));
	p2->defend(map->findTerritory("10"));
	p2->attack(map->findTerritory("6"));

	ge.addPlayer(p1);
	ge.addPlayer(p2);

	p1->getCards()->add(new Card(AIRLIFT));
	p2->getCards()->add(new Card(BOMB));
	p1->getCards()->add(new Card(BLOCKADE));
	p2->getCards()->add(new Card(DIPLOMACY));

	{
		auto order = ge.issueOrder(p1);
		assert(order!=nullptr);
		cout << *order << endl;
		assert(dynamic_cast<AirliftOrder*>(order) != nullptr);
	}
	{
		auto order = ge.issueOrder(p2);
		assert(order != nullptr);
		cout << *order << endl;
		auto bomb = dynamic_cast<BombOrder*>(order);
		assert(bomb!=nullptr);
	}

	{
		auto order = ge.issueOrder(p1);
		assert(order != nullptr);
		cout << *order << endl;
		assert(dynamic_cast<BlockadeOrder*>(order) != nullptr);
	}
	{
		auto order = ge.issueOrder(p2);
		assert(order != nullptr);
		cout << *order << endl;
		auto bomb = dynamic_cast<NegotiateOrder*>(order);
		assert(bomb != nullptr);
	}

	cout << tab << "testIssueOrder3 success" << endl;
}

void testIssueOrder4() {
	GameEngine ge;
	ge.loadMap("map10.txt");

	auto map = ge.getMap();
	assert(map->getContinents().size() == 3);
	assert(map->getNodes().size() == 10);
	assert(map->findTerritory("1") != nullptr);

	Player* p1 = new Player("p1");
	Player* p2 = new Player("p2");

	auto t1 = map->findTerritory("1");
	auto t2 = map->findTerritory("2");
	auto t3 = map->findTerritory("3");
	auto t4 = map->findTerritory("4");
	auto t5 = map->findTerritory("5");
	auto t6 = map->findTerritory("6");
	auto t7 = map->findTerritory("7");
	auto t8 = map->findTerritory("8");
	auto t9 = map->findTerritory("9");
	auto t10 = map->findTerritory("10");

	p1->defend(map->findTerritory("1"));
	p1->defend(map->findTerritory("2"));
	p1->defend(map->findTerritory("3"));
	p1->defend(map->findTerritory("4"));
	p1->defend(map->findTerritory("5"));
	p1->defend(map->findTerritory("6"));
	p1->attack(map->findTerritory("7"));

	p2->defend(map->findTerritory("7"));
	p2->defend(map->findTerritory("8"));
	p2->defend(map->findTerritory("9"));
	p2->defend(map->findTerritory("10"));
	p2->attack(map->findTerritory("6"));

	ge.addPlayer(p1);
	ge.addPlayer(p2);

	{
	int i=0;
	while (true) {
		auto order =ge.issueOrder(p2);
		if (order==nullptr)
			break;
		assert(dynamic_cast<AdvanceOrder*>(order)!=nullptr);
		++i;
	}
	assert(i==4);
	}

	{
	int i=0;
	while (true) {
		auto order = ge.issueOrder(p1);
		if (order == nullptr)
			break;
		assert(dynamic_cast<AdvanceOrder*>(order) != nullptr);
		++i;
	}
	assert(i == 6);
	}

	cout << tab << "testIssueOrder4 success" << endl;
}

void testIssueOrdersPhase1() {
	GameEngine ge;
	ge.loadMap("map10.txt");
	ge.validatemap();
	ge.addPlayer("p1");
	ge.addPlayer("p2");
	ge.startGame();
	ge.reinforcementPhase();
	int n =ge.issueOrdersPhase();
	auto p1 = ge.findPlayer("p1");
	auto p2 = ge.findPlayer("p2");

	cout << tab << "issue total " << n << " orders" << endl;
	cout << "p1 has " <<p1->getOrders()->size() << " orders" <<endl;
	cout << (*p1->getOrders()) <<endl;
	cout << "p2 has " << p2->getOrders()->size() << " orders" << endl;
	cout << (*p2->getOrders()) << endl;

	cout << "testIssueOrdersPhase1 success" <<endl;
}

void testIssueOrdersPhase() {
	testDefendList();
	testAttackList();
	testIssueOrder1();
	testIssueOrder2();
	testIssueOrder3();
	testIssueOrder4();
	testIssueOrdersPhase1();
	cout << tab << "testIssueOrdersPhase success" << endl;
}

void testEndOfTurn1() {
	GameEngine ge;
	ge.loadMap("map10.txt");
	ge.validatemap();

	Player* p1 = new Player("p1");
	Player* p2 = new Player("p2");

	ge.addPlayer(p1);
	ge.addPlayer(p2);
	ge.getDeck()->add(new Card(BOMB));
	ge.getDeck()->add(new Card(BLOCKADE));

	//ge.startGame();

	auto map = ge.getMap();
	auto t1 = map->findTerritory("1");
	auto t2 = map->findTerritory("2");
	auto t3 = map->findTerritory("3");
	auto t4 = map->findTerritory("4");
	auto t5 = map->findTerritory("5");
	auto t6 = map->findTerritory("6");
	auto t7 = map->findTerritory("7");
	auto t8 = map->findTerritory("8");
	auto t9 = map->findTerritory("9");
	auto t10 = map->findTerritory("10");

	p1->add(t1);
	p1->add(t2);
	p1->add(t3);
	p1->add(t4);
	p1->add(t5);
	p1->add(t6);
	p2->add(t7);
	p2->add(t8);
	p2->add(t9);
	p2->add(t10);

	assert(ge.endOfTurn()==false);
	assert(p1->getCards()->empty());
	assert(p2->getCards()->empty());

	p1->conqure(t7);
	assert(p1->getTerritories().size()==7);
	assert(p2->getTerritories().size() == 3);
	assert(p1->getCards()->empty());
	assert(ge.endOfTurn() == false);
	assert(p1->getCards()->size()==1);
	assert(ge.getDeck()->size()==1);
	assert(p2->getCards()->empty());
	
	p1->prepareForTurn();
	p2->conqure(t7);
	assert(p1->getTerritories().size() == 6);
	assert(p2->getTerritories().size() == 4);
	assert(ge.endOfTurn() == false);
	assert(p1->getCards()->size()==1);
	assert(ge.getDeck()->empty());
	assert(p2->getCards()->size()==1);

	cout << tab << "testEndOfTurn1 success" << endl;

	p1->prepareForTurn();
	p2->prepareForTurn();
	p1->conqure(t7);
	p1->conqure(t8);
	p1->conqure(t9);
	p1->conqure(t10);

	assert(ge.endOfTurn());
	assert(p2->isEmpty());
	assert(ge.getPlayers().size()==1);
	assert(ge.findPlayer("p2")==nullptr); // p2 is auto removed

	//p1 owns all the territories
	for (auto x : map->getNodes())
		assert(p1->owns(x));

	assert(ge.getState()==PLAYER_WIN);

	
}

void testEndOfTurn() {
	testEndOfTurn1();
	cout << tab << "testEndOfTurn success" << endl;
}

void testMainGameLoop1() {
	cout << tab << "begin of testMainGameLoop1" <<endl;
	GameEngine ge;
	ge.loadMap("map10.txt");
	ge.validatemap();

	ge.addPlayer("p1");
	ge.addPlayer("p2");
	ge.addPlayer("p3");
	ge.addPlayer("p4");

	ge.startGame();
	ge.maxTurn =100;
	int n =ge.mainGameLoop();
	cout << "game stops after " << n << " turns." <<endl;
	if (ge.getState() == PLAYER_WIN) {
		cout << "winner is " <<ge.getPlayers()[0]->getName() <<endl;
	}
	else {
		cout << "no player win,final status is " << endl;
		for (auto x :ge.getPlayers())
			cout <<(*x) <<endl;
	}
	cout << "order records is:" <<endl;
	for (auto x : *(ge.orderRecords)) {
		cout << (*x) << endl;
		cout << "effect: " << x->getEffect() << endl;
	}

	cout << tab << "testMainGameLoop1 success" << endl;
}

void testMainGameLoop() {
	cout << "begin of testMainGameLoop" <<endl;
	testCalArmyPool();
	cout <<endl;
	testReinforcementPhase();
	cout <<endl;
	testIssueOrdersPhase();
	cout <<endl;
	testEndOfTurn();
	cout << endl;
	testMainGameLoop1();
	cout << "end of testMainGameLoop" << endl;
}
