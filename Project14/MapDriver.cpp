#include <cassert>

#include "Map.h"

using std::endl;

// test for correct map file
void basicMapTest1() {
    MapLoader loader;
    loader.load("1.map");
    Map* map = loader.getMap();

    cout <<"read mapfile 1.map success,the detail of map is" <<endl;
    cout << (*map) <<endl;

    assert(map->getContinents().size()==14);
    auto c1 = map->findContinent("New Brunswick");
    assert(c1!=nullptr);
    assert(c1->getTerritories().size()==2);
    assert(c1->getValue()==2);

    auto t1=map->findTerritory("79");
    assert(t1!=nullptr);
    auto c2 = t1->getContinent();
    assert(c2->getName()=="Newfoundland");
    assert(c2->getTerritories().size()==5);

    //cout <<(*t1)<<endl;
    auto const& list1 = t1->getNeighbours();
    assert(list1.size()==3);
    auto t2 = list1[1];
    assert(t2 != nullptr);
    assert(t2->getName()=="80");
    
    auto c3 = map->findContinent("Not exist");
    assert(c3 == nullptr);

    cout << "basicTest1 test success" <<endl;
}


void connectTest1() {
    MapLoader loader;
    loader.load("2.map");
    Map* map = loader.getMap();

    try {
        map->validate();
    }
    catch (MapException& e) {
        cout <<e<<endl;
        return;
    }

    cout << "connectTest1 success,mapfile 2.map is connected" <<endl;
}

void connectTest2() {
    MapLoader loader;
    loader.load("3.map");
    Map* map = loader.getMap();

    try {
        map->validate();
    }
    catch (MapException& e) {
        cout << e ;
        cout << "connectTest2 success" << endl;
        return;
    }

    cout << "connectTest2 failed" << endl;
}

void connectTest3() {
    MapLoader loader;
    loader.load("4.map");
    Map* map = loader.getMap();

    try {
        map->validate();
    }
    catch (MapException& e) {
        cout << e ;
        cout << ", connectTest3 success" << endl;
        return;
    }

    cout << "connectTest3 failed" << endl;
}



void testLoadMaps() {
      basicMapTest1();
      connectTest1();
      connectTest2();
      connectTest3();

      cout << "\ntestLoadMaps success" <<endl;
}
