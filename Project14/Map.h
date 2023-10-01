#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <set>
#include <map>
#include <utility>
#include <exception>
#include <fstream>

using std::vector;
using std::string;
using std::ostream;
using std::istream;
using std::shared_ptr;
using std::set;
using std::pair;
using std::map;
using std::cout;
using std::exception;
using std::ifstream;

class Player;
class Continent;

class Territory {
    string name;
    int x;
    int y;

    Continent* continent;

    int armyUnit = 0;

    vector<Territory*> neighbours;

public:
    Player* player;
    /*constructor and destructor*/
    Territory(string name);
    Territory(const Territory& other);
    ~Territory();
    Territory& operator=(const Territory& other);

    void set(int x, int y, Continent* continent);

    /*getter and setters*/
    const string& getName()const;

    Player* getPlayer()const;
    void setPlayer(Player* player);

    Continent* getContinent()const;
    void setContinent(Continent* c);

    int getArmyUnit()const;
    void setArmyUnit(int newValue);
    void incArmyUint(int n);
    void decArmyUint(int n);
    //half of the army units are removed from this territory.
    void decHalf();

    // the number of army units on the territory is doubled
    void doubled();
    // detect whether all the army is removed from this territory.
    bool isEmpty()const;

    

    const vector<Territory*> getNeighbours()const;
    void addNeighbour(Territory* t);

    //detect whether two Territories is adjacent to each other
    bool isAdjacentTo(Territory* t)const;

    // outprint function
    void print(ostream& os)const;

    //select random army from this territory
    int selectRandArmy()const;
};

// stream inserter for Territory
ostream& operator<<(ostream& os, const Territory& territory);

class Continent {
    string name;

    /*
     continent control value, which represents the number of bonus armies
     that you get during the reinforcement phase if you control all the territories
     that belong to this continent
    */
    int controlValue;
    vector<Territory*> territories;

public:
    /*constructor and destructor*/


    Continent(string name, int controlValue);
    ~Continent();
    Continent(const Continent& other);

    Continent& operator=(const Continent& other);

    /* getter and setters */
    const string& getName()const;
    const vector<Territory*>& getTerritories()const;

    int getValue()const;

    //add a territroy to this continent
    void add(Territory* t);

    // print the information of this continent
    void print(ostream& os)const;
};

ostream& operator<<(ostream& os, const Continent& continent);

class MapException :public exception {
public:
    virtual void print(ostream& os)const;
};
ostream& operator<<(ostream& os, const MapException& e);

class NoFatherException :public MapException {
    const Territory* territory;
public:
    NoFatherException(const Territory* t);
    virtual void print(ostream& os)const;
};

class Map;
class NotConnectedException :public MapException {

};

class MapNotConnectedException :public NotConnectedException {
    const Map* map;
public:
    MapNotConnectedException(const Map* map);
    virtual void print(ostream& os)const;
};

class ContinentNotConnectedException :public NotConnectedException {
    const Continent* continent;
public:
    ContinentNotConnectedException(const Continent* c);
    virtual void print(ostream& os)const;
};

class Map
{
    string filename;
    map<string, string> configs;
    vector<Continent*> continents;
    vector<Territory*> nodes;

    Territory* addTerritory(const string& name);

    Territory* territorieNoFather()const;
    Continent* unconnectedContinent()const;
public:
    /*constructor and destructor*/
    Map();
    Map(const Map& other);
    Map& operator=(const Map& other);
    ~Map();

    // getters
    const string& getFileName()const;

    const vector<Continent*>& getContinents()const;
    const vector<Territory*>& getNodes()const;

    Territory* findTerritory(const string& name)const;
    Continent* findContinent(const string& name)const;

    //setters
    void setFileName(const string& name);
    void addEdge(const string& src, const string& dest);
    void addConfig(const string& key, const string& value);
    Continent* addContinent(const string& name, int value);
    Territory* addTerritoryInfo(const string& name, int x, int y, const string& continentName);

    void print(ostream& os)const;
    void validate()const;
};

ostream& operator<<(ostream& os, const Map& map);

class MapLoader {
    ifstream ifs;
    Map* map;

    //help functions for read data from file
    void readConfigs();
    void readContinents();
    void readTerritories();
public:
    void load(const string& filename);
    Map* getMap()const;

};


// find a territory  in a vector
Territory* find(const vector<Territory*>& territories,const string& name);
Continent* find(const vector<Continent*>& continents, const string& name);
void testLoadMaps();



