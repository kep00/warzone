#include <cassert>
#include <iostream>
#include <algorithm>
using std::cout;
using std::endl;
using std::random_shuffle;

#include "Orders.h"
#include "GameEngine.h"

void GameEngine::loadMap(const string &filename)
{
    MapLoader loader;
    loader.load(filename);
    map = loader.getMap();
}

bool GameEngine::validatemap() const
{
    bool res = true;
    try
    {
        map->validate();
    }
    catch (MapException &)
    {
        res = false;
    }
    return res;
}

void GameEngine::distribTerrtories()
{
    vector<Territory *> objs = map->getNodes();
    random_shuffle(objs.begin(), objs.end());
    size_t i = 0;
    for (auto x : objs)
    {
        players[i]->add(x);
        i = (i + 1) % players.size();
    }
}

void GameEngine::shufflePlayers()
{

    random_shuffle(players.begin(), players.end());
}

void GameEngine::initArmy()
{
    for (auto x : players)
        initArmy(x);
}

void GameEngine::initArmy(Player *player)
{
    player->setArmyUnit(50);
}

void GameEngine::initDeck(int n)
{
    for (int i = 0; i < n; ++i)
    {
        auto card = randomCard();
        deck->add(card);
    }
}

void GameEngine::initDeck()
{
    initDeck(players.size() * 2);
}

void GameEngine::initHand()
{
    for (auto x : players)
        initHand(x);
}

void GameEngine::initHand(Player *player)
{
    deck->draw(player);
    deck->draw(player);
}

void GameEngine::startGame()
{
    distribTerrtories();
    shufflePlayers();
    initArmy();
    initDeck();
    initHand();
    orderRecords->clear();
}

void GameEngine::reinforcementPhase()
{
    for (auto x : players)
        x->prepareForTurn();
    initPools();
}

void GameEngine::initPools()
{
    for (auto x : players)
    {
        int n = x->calArmyPool();
        x->incArmyUnit(n);
    }
}

Order *GameEngine::issueOrder(Player *player)
{
    return player->issueOrder(deck, players);
}

int GameEngine::issueOrders(Player *player)
{
    int n = 0;
    while (player->issueOrder(deck, players) != nullptr)
    {
        ++n;
    }
    // assert(n == player->getOrders()->size());
    return n;
}

int GameEngine::issueOrdersPhase()
{
    int n = 0;
    for (auto x : players)
    {
        x->rebuildDefendList();
        x->rebuildAttackList();
        n += issueOrders(x);
    }
    return n;
}

void GameEngine::executeOrders(Player *player)
{
    while (true)
    {
        Order *order = player->nextOrder();
        if (order == nullptr)
            break;
        else
        {
            order->execute();
            orderRecords->push_back(order);
        }
    }
}

void GameEngine::executeOrdersPhase()
{
    for (auto x : players)
    {
        executeOrders(x);
    }
}

bool GameEngine::endOfTurn()
{
    removeEmpyPlayers();
    if (players.size() == 1)
    {
        transition(PLAYER_WIN);
        return true;
    }
    sendCard();
    return false;
}

void GameEngine::removeEmpyPlayers()
{
    vector<Player *> objs;
    for (auto x : players)
    {
        if (!x->isEmpty())
            objs.push_back(x);
    }
    this->players = objs;
}

void GameEngine::sendCard()
{
    for (auto x : players)
    {
        if (x->hasConquered())
        {
            deck->draw(x);
        }
    }
}

int GameEngine::mainGameLoop()
{
    int i = 0;
    while (true)
    {
        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();
        if (endOfTurn())
            break;
        else
        {
            ++i;
            if (maxTurn > 0 && i >= maxTurn)
                break;
        }
    }
    return i;
}

GameEngine::GameEngine()
    : state(STARTED), deck(new Deck()), orderRecords(new OrderList())
{
    srand(static_cast<unsigned int>(time(0)));
}

GameEngine::~GameEngine()
{
    delete map;
    for (auto x : players)
        delete x;
    delete deck;
    delete commandProcessor;
}

GameEngine::GameEngine(const GameEngine &other)
    : ILoggable(other), Subject(other), state(other.state), map(other.map), players(other.players), deck(other.deck), commandProcessor(other.commandProcessor)
{
}

GameEngine &GameEngine::operator=(const GameEngine &other)
{
    if (this != &other)
    {
        ILoggable::operator=(other);
        Subject::operator=(other);
        state = other.state;
        map = other.map;
        players = other.players;
        deck = other.deck;
        commandProcessor = other.commandProcessor;
    }
    return *this;
}

GameState GameEngine::getState() const
{
    return state;
}

Map *GameEngine::getMap()
{
    return map;
}

vector<Player *> &GameEngine::getPlayers()
{
    return players;
}

Deck *GameEngine::getDeck()
{
    return deck;
}

CommandProcessor *GameEngine::getCommandProcessor()
{
    return commandProcessor;
}

bool GameEngine::isPlayerExists(const string &name) const
{
    for (auto x : players)
        if (x->getName() == name)
            return true;
    return false;
}

string getStateString(GameState state)
{
    string s;
    switch (state)
    {
    case STARTED:
        s = "start";
        break;
    case MAP_LOADED:
        s = "map loaded";
        break;
    case MAP_VALIDATED:
        s = "map validated";
        break;
    case PLAYER_ADDED:
        s = "player added";
        break;
    case ASSIGN_REINFORCEMENT:
        s = "assgin reinforcement";
        break;
    case ISSUE_ORDERS:
        s = "issue orders";
        break;
    case EXECUTE_ORDERS:
        s = "execute orders";
        break;
    case PLAYER_WIN:
        s = "win";
        break;
    case GAME_END:
        break;
    default:
        break;
    }
    return s;
}

GameState GameEngine::transition(GameState state)
{
    auto old = this->state;
    this->state = state;

    ostringstream oss;
    oss << "transition to state " << getStateString(state);
    logString = oss.str();
    notify(this);

    return old;
}

void GameEngine::setMap(Map *map)
{
    // free old map
    if (this->map != nullptr)
        delete this->map;
    this->map = map;
}

bool GameEngine::addPlayer(const string &name)
{
    if (isPlayerExists(name))
        return false;

    Player *player = new Player(name);
    player->getOrders()->copyObservers(*this);
    player->GE = this;
    players.push_back(player);
    return true;
}

void GameEngine::addPlayer(Player *player)
{
    players.push_back(player);
    player->GE = this;
    player->getOrders()->copyObservers(*this);
}

void GameEngine::setCommandProcessor(CommandProcessor *commandProcessor)
{
    // delete old object
    if (this->commandProcessor != nullptr)
        delete this->commandProcessor;
    this->commandProcessor = commandProcessor;
    commandProcessor->copyObservers(*this);
}

void GameEngine::setDeck(Deck *deck)
{
    // delete old object
    if (this->deck != nullptr)
        delete this->deck;
    this->deck = deck;
}

bool GameEngine::process(Command *command)
{
    bool res = true;
    auto type = command->getType();
    if (type == UNKNOWN_COMMAND)
    {
        command->saveEffect("unknown command");
        res = false;
    }
    else
    {
        if (commandProcessor->validate(command, this))
        {
            res = execute(command);
            if (res)
            {
                command->saveEffect();
                command->changeGameState(this);
            }
        }
        else
        {
            ostringstream oss;
            oss << "invalid command in state " << getStateString(state);
            command->saveEffect(oss.str());
            res = false;
        }
    }
    return res;
}

bool GameEngine::process()
{
    Command *command = commandProcessor->getCommand();
    return process(command);
}
vector<vector<string>> allresult;
void jbs(int a, int b, int c, int d) // 地图a 人数b 轮数c 最大回合d
{
    cout << "exec:" << endl
         << "a:" << a << " b: " << b << " c: " << c << " d: " << d << endl;
    GameEngine ge;
    ge.loadMap(std::to_string(a + 1) + ".map"); // 加载地图
    cout << "round:" << c << endl;
    cout << "Use Map:" << std::to_string(a + 1) + ".map" << endl
         << "Player:" << endl;
    for (int i = 0; i < b; i++)
    {
        int shuiji = 0;
        shuiji = rand() % 4;
        if (shuiji == 0)
        {
            ge.addPlayer(new Player("Aggressive"));
            ge.getPlayers()[i]->ps = (PlayerStrategy *)new AggressivePlayerStrategy();
            cout << "Player" << std::to_string(i + 1) << ":Aggressive" << endl;
        }
        else if (shuiji == 1)
        {
            ge.addPlayer(new Player("Benevolent"));
            ge.getPlayers()[i]->ps = (PlayerStrategy *)new BenevolentPlayerStrategy();
            cout << "Player" << std::to_string(i + 1) << ":Benevolent" << endl;
        }
        else if (shuiji == 2)
        {
            ge.addPlayer(new Player("Neutral"));
            ge.getPlayers()[i]->ps = (PlayerStrategy *)new NeutralPlayerStrategy();
            cout << "Player" << std::to_string(i + 1) << ":Neutral" << endl;
        }
        else if (shuiji == 3)
        {
            ge.addPlayer(new Player("Cheater"));
            ge.getPlayers()[i]->ps = (PlayerStrategy *)new CheaterPlayerStrategy();
            cout << "Player" << std::to_string(i + 1) << ":Cheater" << endl;
        }
        ge.getPlayers()[i]->ps->P = ge.getPlayers()[i];
    }
    cout << "MaxTurn:" << d << endl;
    ge.maxTurn = d;
    ge.startGame();
    if (ge.mainGameLoop() >= ge.maxTurn)
    {
        allresult[a].push_back("Draw\t");
    }
    else
    {
        allresult[a].push_back(ge.getPlayers()[0]->getName());
    }
    cout << "Over!" << endl
         << endl
         << endl;
}
bool GameEngine::execute(Command *command)
{
    auto type = command->getType();
    bool res = true;
    string canshu;

    int a, b, c, d;
    int *add[4] = {&a, &b, &c, &d};
    int i, j, z;

    switch (type)
    {
    case UNKNOWN_COMMAND:
        assert(false);
        break;
    case LOAD_MAP:
        loadMap(command->getArg());
        break;
    case VALIDATE_MAP:
        res = validatemap();
        if (!res)
        {
            ostringstream oss;
            oss << "map " << map->getFileName() << " is invalid";
            command->saveEffect(oss.str());
        }
        break;
    case ADD_PLAYER:
    {
        if (players.size() > 6)
        {
            res = false;
            command->saveEffect("can not add new player because has reached max player number");
        }
        else
        {
            auto &name = command->getArg();
            res = addPlayer(command->getArg());
            if (!res)
            {
                ostringstream oss;
                oss << "can not add exist player " << name;
                command->saveEffect(oss.str());
            }
        }
        break;
    }
    case GAME_START:
        if (players.size() < 2)
        {
            res = false;
            command->saveEffect("can start game because number of player is less than 2");
        }
        else
            startGame();
        break;
    case REPLAY:
    case QUIT:
        break;
    case TOURNAMENT:
        canshu = command->getArg();
        for (i = 0, j = 0, z = 0; i < canshu.size(); i++)
        {
            if (canshu[i] == ' ')
            {
                *(add[z++]) = std::stoi(canshu.substr(j, i - j));
                j = i + 1;
            }
        }

        cout << "Tournament mode: " << endl;
        cout << "M:" << a << endl;
        cout << "P:" << b << endl;
        cout << "G:" << c << endl;
        cout << "D:" << d << endl
             << endl;
        
        srand(time(NULL));
        for (i = 0; i < a; i++)
        {
            allresult.push_back(vector<string>());
            for (j = 0; j < c; j++)
            {
                jbs(i, b, j, d);
            }
        }
        cout << "|\t\t";
        for (j = 0; j < c; j++)
        {
            cout << "|Game " << j + 1 << "\t\t";
        }
        cout << "|" << endl;
        for (i = 0; i < a; i++)
        {
            cout << "|" << std::to_string(i + 1) + ".map" << '\t';
            for (j = 0; j < c; j++)
            {
                cout << "|" << allresult[i][j] << "\t";
            }
            cout << "|" << endl;
        }
        break;
    default:
        assert(false);
        break;
    }
    return res;
}
void GameEngine::process(const string &command)
{
    if (command == "end")
    {
        state = GAME_END;
    }
    else if (command == "loadmap")
    {
        if (state == STARTED)
        {
            state = MAP_LOADED;
        }
        else if (state == MAP_LOADED)
        {
            state = MAP_LOADED;
        }
        else
        {
            cout << "command " << command << " is not valid in current state:" << getStateString(state) << endl;
        }
    }
    else if (command == "validatemap")
    {
        if (state == MAP_LOADED)
        {
            state = MAP_VALIDATED;
        }
        else
        {
            cout << "command " << command << " is not valid in current state:" << getStateString(state) << endl;
        }
    }
    else if (command == "addplayer")
    {
        if (state == MAP_VALIDATED)
        {
            state = PLAYER_ADDED;
        }
        else if (state == PLAYER_ADDED)
        {
            state = PLAYER_ADDED;
        }
        else
        {
            cout << "command " << command << " is not valid in current state:" << getStateString(state) << endl;
        }
    }
    else if (command == "assigncountries")
    {
        if (state == PLAYER_ADDED)
        {
            state = ASSIGN_REINFORCEMENT;
        }
        else
        {
            cout << "command " << command << " is not valid in current state:" << getStateString(state) << endl;
        }
    }
    else if (command == "issueorder")
    {
        if (state == ASSIGN_REINFORCEMENT)
        {
            state = ISSUE_ORDERS;
        }
        else if (state == ISSUE_ORDERS)
        {
            state = ISSUE_ORDERS;
        }
        else
        {
            cout << "command " << command << " is not valid in current state:" << getStateString(state) << endl;
        }
    }
    else if (command == "endissueorders")
    {
        if (state == ISSUE_ORDERS)
        {
            state = EXECUTE_ORDERS;
        }
        else
        {
            cout << "command " << command << " is not valid in current state:" << getStateString(state) << endl;
        }
    }
    else if (command == "exeorder")
    {
        if (state == EXECUTE_ORDERS)
        {
            state = EXECUTE_ORDERS;
        }
        else
        {
            cout << "command " << command << " is not valid in current state:" << getStateString(state) << endl;
        }
    }
    else if (command == "endexeorders")
    {
        if (state == EXECUTE_ORDERS)
        {
            state = ASSIGN_REINFORCEMENT;
        }
        else
        {
            cout << "command " << command << " is not valid in current state:" << getStateString(state) << endl;
        }
    }
    else if (command == "win")
    {
        if (state == EXECUTE_ORDERS)
        {
            state = PLAYER_WIN;
        }
        else
        {
            cout << "command " << command << " is not valid in current state:" << getStateString(state) << endl;
        }
    }
    else if (command == "play")
    {
        if (state == PLAYER_WIN)
        {
            state = STARTED;
        }
        else
        {
            cout << "command " << command << " is not valid in current state:" << getStateString(state) << endl;
        }
    }
    else
    {
        cout << "command " << command << " is invalid!" << endl;
    }
}

void GameEngine::print(ostream &os) const
{
    os << getStateString(state);
}

void GameEngine::startupPhase()
{
    while (state != ASSIGN_REINFORCEMENT)
    {
        process();
    }
}

Player *GameEngine::findPlayer(const string &name) const
{
    for (auto x : players)
        if (x->getName() == name)
            return x;
    return nullptr;
}

ostream &operator<<(ostream &os, const GameEngine &ge)
{
    ge.print(os);
    return os;
}