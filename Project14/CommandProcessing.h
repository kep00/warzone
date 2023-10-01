#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
using std::string;
using std::list;
using std::ifstream;
using std::ostream;
using std::ostringstream;

#include "LoggingObserver.h"

enum CommandType {
    UNKNOWN_COMMAND
    ,LOAD_MAP,VALIDATE_MAP
    ,ADD_PLAYER,GAME_START
    ,REPLAY
    ,QUIT
    ,TOURNAMENT
};

string getCommandTypeStr(CommandType type);

enum GameState;
class GameEngine;

class Command:public ILoggable,public Subject{
    CommandType type=UNKNOWN_COMMAND;
    string arg;
    string effect;

    
public:
    //constructor and destructor
    Command(CommandType type);
    Command(CommandType type,const string& arg);
    Command(CommandType type,int,int,int,int);
    Command(const string& arg);
    Command(const Command& other);
    ~Command();
    Command& operator=(const Command& other);

    // the default effect string when command success
    string getSuccessString()const;

    //getters
    CommandType getType()const;
    const string& getArg()const;
    const string& getEffect()const;

    //save the effect of the command 
    void saveEffect(const string& effect);
    void saveEffect(const char* effect);
    void saveEffect();

    void print(ostream& os)const;
    
    bool validate(GameState state)const;
    static bool validate(CommandType type,GameState state);

    // change the state of game,and return new state
    GameState changeGameState(GameEngine* ge)const;
    static GameState getNewState(CommandType type, GameEngine* ge);
};

ostream& operator<<(ostream& os,const Command& command);

class CommandProcessor:public list<Command*>,public ILoggable,public Subject
{
private:
    
protected:
    

public:
    virtual string readNextLine();
    Command* readCommand(const string& line);
    void saveCommand(Command* command);

    //check if the command is valid in the current game state
    bool validate(Command* command, GameEngine* ge)const;

    //constructor and destructors
    CommandProcessor();
    CommandProcessor(const CommandProcessor&other);
    ~CommandProcessor();
    CommandProcessor& operator=(const CommandProcessor& other);

    virtual Command* getCommand();
    virtual void print(ostream& os)const;
};

ostream& operator<<(ostream& os, const CommandProcessor& commandProcessor);

class FileLineReader {
private:
    string filename;
    ifstream ifs;
public:
    //constructor and destructor
    FileLineReader(const string& filename);
    FileLineReader(const char* filename);
    ~FileLineReader();
    FileLineReader(const FileLineReader& other);

    FileLineReader& operator=(const FileLineReader& other);

    //getters
    const string& getFileName()const;
    string readLineFromFile();
};

class FileCommandProcessorAdapter:public CommandProcessor{
private:
    FileLineReader* reader;
public:
    //constructor
    FileCommandProcessorAdapter(const string& filename);
    FileCommandProcessorAdapter(const char* filename);
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other);
    ~FileCommandProcessorAdapter();
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& other);

    virtual string readNextLine();
};


void testCommandProcessor();
void testTournament();