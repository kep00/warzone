
#pragma once
#include <iostream>
#include <string>
#include <time.h>
using std::string;
using std::ostream;

#include "Map.h"
#include "Player.h"
#include "Cards.h"
#include "CommandProcessing.h"
#include "LoggingObserver.h"

enum GameState {
    STARTED,
    MAP_LOADED,
    MAP_VALIDATED,
    PLAYER_ADDED,
    ASSIGN_REINFORCEMENT,
    ISSUE_ORDERS,
    EXECUTE_ORDERS,
    PLAYER_WIN,
    GAME_END,
    TOURNAMENT_STATE
};
class OrderList;
class GameEngine:public ILoggable,public Subject
{
    GameState state=STARTED;
    Map* map=nullptr;
    vector<Player*> players;
    Deck* deck;
    CommandProcessor* commandProcessor;
    

public:
    OrderList* orderRecords;  //only for debug
    int maxTurn; // only for debug

    //constructors
    GameEngine();
    ~GameEngine();
    GameEngine(const GameEngine& other);
    GameEngine& operator=(const GameEngine& other);

    // getters
    GameState getState()const;
    Map* getMap();
    vector<Player*>& getPlayers();
    Deck* getDeck();
    CommandProcessor* getCommandProcessor();

    //detect whether a player has the given name exists
    bool isPlayerExists(const string&name)const;

    //setters
    GameState transition(GameState state);
    void setMap(Map* map);
    bool addPlayer(const string& name);
    void addPlayer(Player* player);
    void setCommandProcessor(CommandProcessor* commandProcessor);
    void setDeck(Deck* deck);
    
    //whether show detail at each step
    void setDetailFlag(bool value);

    //execute the command
    bool process();
    bool process(Command* command);
    bool execute(Command* command);

    // process the command
    void process(const string& command);

    void print(ostream& os)const;

    void startupPhase();

    //show current detail of game
    void showStatus(ostream&os)const;

    
    Player* findPlayer(const string&name)const;

    /*
     select a map from a list of map files as stored in a directory,
    which results in the map being loaded in the game.
    */
    void loadMap(const string& filename);

    bool validatemap()const;

    // fairly distribute all the territories to the players
    void  distribTerrtories();
    
    // determine randomly the order of play of the players in the game
    void shufflePlayers();

    // give 50 initial army units to the players, which are placed in their respective reinforcement pool
    void initArmy();
    void initArmy(Player* player);

    // build initial deck,randomly add cards
    void initDeck(int n);
    void initDeck();

    // let each player draw 2 initial cards from the deck using the deck's draw() method
    void initHand();
    void initHand(Player* player);

    void startGame();

    void reinforcementPhase();

    // initialize the pool for all players at start of each turn
    void initPools();

    Order* issueOrder(Player *player);

    // issue all orders of a player,return the number of orders
    int issueOrders(Player* player);

    //return the total order num of all players
    int issueOrdersPhase();

    void executeOrders(Player* player);

    //execute all of players orders,and save them in vector(for debug)
    void executeOrdersPhase();

    // return true if a player win
    bool endOfTurn();

    void removeEmpyPlayers();

    //send a card to the player who conqures at least territory.
    void sendCard();

    int mainGameLoop();
};

string getStateString(GameState state);

ostream& operator<<(ostream& os, const GameEngine& ge);
void testGameStates();
void testStartupPhase();
void testMainGameLoop();



