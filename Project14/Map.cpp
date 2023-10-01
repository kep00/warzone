#include<cassert>
#include <iostream>
#include <queue>
#include <sstream>
using std::endl;
using std::istringstream;
using std::queue;

#include "Map.h"
#include "Player.h"

bool isConnected(const vector<Territory*>& nodes);

Territory::Territory(string name)
    :name(name)
{   
        
}

Territory::Territory(const Territory& other)
    :name(other.name)
    , x(other.x)
    , y(other.y)
    , continent(other.continent)
    , player(other.player)
    , armyUnit(other.armyUnit)
    ,neighbours(other.neighbours)
{
    
}

Territory::~Territory()
{
    
}

Territory& Territory::operator=(const Territory& other) {
    if (this != &other) {
        name = other.name;
        x = other.x;
        y = other.y;
        continent = other.continent;
        player = other.player;
        armyUnit= other.armyUnit;
        neighbours =other.neighbours;
    }
    return *this;
}

void Territory::set(int x, int y, Continent* continent)
{
    this->x=x;
    this->y=y;
    this->continent =continent;
}

const string& Territory::getName() const
{
    return name;
}

Player* Territory::getPlayer() const
{
    return player;
}

void Territory::setPlayer(Player* player)
{
    assert(player!=nullptr);
    this->player = player;
}

Continent* Territory::getContinent() const
{
    return continent;
}

void Territory::setContinent(Continent* c)
{
    continent =c;
}

int Territory::getArmyUnit() const
{
    return armyUnit;
}

void Territory::setArmyUnit(int n)
{
    armyUnit = n;
}

void Territory::incArmyUint(int n)
{
    armyUnit += n;
}

void Territory::decArmyUint(int n)
{
    armyUnit -= n;
}

void Territory::decHalf()
{
    armyUnit /=2 ;
}

void Territory::doubled()
{
    armyUnit *= 2;
}

bool Territory::isEmpty() const
{
    return armyUnit==0;
}

const vector<Territory*> Territory::getNeighbours() const
{
    return neighbours;
}

void Territory::addNeighbour(Territory* t)
{
    neighbours.push_back(t);
}

bool Territory::isAdjacentTo(Territory* t) const
{
    assert (t!=nullptr);
    for(auto x :neighbours)
        if (x==t)
            return true;
    return false;
}

void Territory::print(ostream& os) const
{
    os << name <<","<<x<<","<<y<<","<<continent->getName();
    for(const auto x:neighbours)
        os <<"," <<x->getName();
}

int Territory::selectRandArmy() const
{
    if (armyUnit>0)
        return rand()%armyUnit+1;
    else
        return rand()%10+1;
}

ostream& operator<<(ostream& os, const Territory& territory) {
    territory.print(os);
    return os;
}

Continent::Continent(string name, int controlValue)
    :name(name),controlValue(controlValue)
{
    
}

Continent::~Continent()
{
    
}

Continent::Continent(const Continent& other)
    :name(other.name)
    ,controlValue(other.controlValue)
    ,territories(other.territories)
{
    
}

Continent& Continent::operator=(const Continent& other) {
    if (this != &other) {
        this->name =other.name;
        this->controlValue =other.controlValue;
        this->territories= other.territories;
    }
    return *this;
}

const string& Continent::getName() const
{
    return name;
}

const vector<Territory*>& Continent::getTerritories() const
{
    return territories;
}

int Continent::getValue() const
{
    return controlValue;
}

void Continent::add(Territory* t){
    territories.push_back(t);
    t->setContinent(this);
}

void Continent::print(ostream& os) const
{
    os << name<<"=" <<controlValue ;
}

ostream& operator<<(ostream& os, const Continent& continent) {
    continent.print(os);
    return os;
}


Territory* Map::addTerritory(const string& name)
{
    Territory* t = findTerritory(name);
    if (t == nullptr){
        t = new Territory(name);
        nodes.push_back(t);
    }
    return t;
}

Territory* Map::territorieNoFather() const
{
    for (const auto x : nodes) {
        if (x->getContinent()==nullptr)
           return x;
    }

    return nullptr;
}

Continent* Map::unconnectedContinent() const
{
    for (const auto x : continents) {
        if (!isConnected(x->getTerritories()))
            return x;
    }
    return nullptr;
}

Map::Map()
{
}

Map::Map(const Map& other)
    :configs(other.configs)
    ,continents(other.continents)
    ,nodes(other.nodes)
{
    
}

Map::~Map()
{
    
}

Map& Map::operator=(const Map& other) {
    if (this != &other) {
        this->configs = other.configs;
        this->continents = other.continents;
        this->nodes = other.nodes;
    }
    return *this;
}

const string& Map::getFileName() const
{
    return filename;
}

const vector<Continent*>& Map::getContinents() const
{
    return continents;
}

const vector<Territory*>& Map::getNodes() const
{
    return nodes;
}

Territory* Map::findTerritory(const string& name) const
{
    return find(this->nodes,name);
}

Continent* Map::findContinent(const string& name) const
{
    return find(this->continents, name);
}

void Map::setFileName(const string& name)
{
    filename = name;
}

void Map::addEdge(const string& src, const string& dest)
{
    Territory* start = addTerritory(src);
    Territory* end = addTerritory(dest);
    start->addNeighbour(end);
}

void Map::addConfig(const string& key, const string& value)
{
    configs[key] = value;
}

Continent* Map::addContinent(const string& name, int value)
{
    auto c = new Continent(name, value);
    continents.push_back(c);
    return c;
}

Territory* Map::addTerritoryInfo(const string& name, int x, int y, const string& continentName)
{
    Territory* t = addTerritory(name);
    Continent* c=  findContinent(continentName);
    assert(c!=nullptr);
    t->set(x,y,c);
    c->add(t);
    return t;
}

void Map::print(ostream& os) const
{
    os << "[Map]" << endl;
    for (const auto& obj : configs) {
        os << obj.first << "=" << obj.second << endl;
    }
    os << endl;

    os << "[Continents]" << endl;
    for (const auto& p : continents) {
        os << (*p) << endl;
    }
    os << endl;

    os << "[Territories]" << endl;
    for (const auto& p : nodes) {
        os << (*p) << endl;
    }
}

void Map::validate() const
{
    auto x =territorieNoFather();
    if (x != nullptr) {
        throw NoFatherException(x);
    }
    auto c = unconnectedContinent();
    if (c != nullptr) {
        throw ContinentNotConnectedException(c);
    }

    if (!isConnected(nodes))
        throw MapNotConnectedException(this);
}

bool isConnected(const vector<Territory*>& nodes) {
    set<string> visited;
    set<string> names;
    for(const auto& x:nodes)
        names.insert(x->getName());
        
    queue<Territory*> q;
    q.push(nodes[0]);
    names.erase(nodes[0]->getName());
    visited.insert(nodes[0]->getName());

    while (!q.empty()&& !names.empty()) {
        Territory* t =q.front();
        q.pop();

        //cout << "father is " <<t->getName() <<endl;

        for (const auto x : t->getNeighbours()) {
            const auto& name =x->getName();
            if (visited.find(name) == visited.end()) {
                //cout << "will insert " << name << endl;
                q.push(x);
                visited.insert(name);
                names.erase(name);
            }
        }
    }

    return names.empty();
}

ostream& operator<<(ostream& os, const Map& map) {
    map.print(os);
    return os;
}

void MapLoader::readConfigs()
{
    while (true){
        string s;
        getline(ifs, s);
        if (s == "[Map]")
            break;
    }

    while (true)
    {
        string s;
        getline(ifs, s);
        if (s.empty())
            break;
        else {
            istringstream iss(s);
            string key;
            string value;
            getline(iss, key, '=');
            iss >> value;
            map->addConfig(key, value);
        }
    }
}

void MapLoader::readContinents()
{
    while (true)
    {
        string s;
        getline(ifs, s);
        if (s == "[Continents]")
            break;
    }

    while (true)
    {
        string s;
        getline(ifs, s);
        if (s.empty())
            break;
        else {
            istringstream iss(s);
            string name;
            int value;
            getline(iss, name, '=');
            iss >> value;
            map->addContinent(name, value);
        }
    }
}

void MapLoader::readTerritories()
{
    while (true)
    {
        string s;
        getline(ifs, s);
        if (s == "[Territories]")
            break;
    }

    while (ifs)
    {
        string s;
        getline(ifs, s);
        if (s.empty())
            continue;
        else {
            istringstream iss(s);
            string name;
            int x;
            int y;
            string continent_name;
            getline(iss, name, ',');
            iss >> x;
            iss.ignore(1, ',');
            iss >> y;
            iss.ignore(1, ',');
            getline(iss, continent_name, ',');
            Territory* t = map->addTerritoryInfo(name, x, y, continent_name);
            while (iss)
            {
                string s;
                std::getline(iss, s, ',');
                if (!s.empty())
                    map->addEdge(name, s);
            }
        }
    }
}

void MapLoader::load(const string& filename)
{
    ifs.open(filename);
    if (map!=nullptr)
        delete map;
    map =new Map();
    map->setFileName(filename);
    readConfigs();
    readContinents();
    readTerritories();
    ifs.close();
}

Map* MapLoader::getMap() const{
    return map;
}

NoFatherException::NoFatherException(const Territory* t)
    :territory(t)
{
}

void NoFatherException::print(ostream& os) const
{
    os << " the territory " << (territory->getName()) << "doesnot  belongs to any continents.";
}

ContinentNotConnectedException::ContinentNotConnectedException(const Continent* c)
    :continent(c)
{
    
}

void ContinentNotConnectedException::print(ostream& os) const
{
    os << "continent " << continent->getName() << " is not a connected subgraph" ;
}

MapNotConnectedException::MapNotConnectedException(const Map* map)
    :map(map)
{
    
}

void MapNotConnectedException::print(ostream& os) const
{
    os << "map in file " << (this->map->getFileName()) << " is not connected";
}


void MapException::print(ostream& os) const
{
    
}

ostream& operator<<(ostream& os, const MapException& e) {
    e.print(os);
    return os;
}

Continent* find(const vector<Continent*>& continents, const string& name) {
    for (auto x : continents) {
        if (x->getName() == name)
            return x;
    }
    return nullptr;
}

Territory* find(const vector<Territory*>& territories, const string& name)
{
    for (auto x : territories) {
        if (x->getName() == name)
            return x;
    }
    return nullptr;
}

