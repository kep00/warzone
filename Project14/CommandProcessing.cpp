#include <cassert>
#include <iostream>
using std::cin;
using std::endl;
using std::istringstream;

#include "GameEngine.h"
#include "CommandProcessing.h"

string Command::getSuccessString() const
{
    ostringstream oss;
    switch (type)
    {
    case LOAD_MAP:
        oss << "map " << arg << " has been loaded";
        break;
    case VALIDATE_MAP:
        oss << "map validated";
        break;
    case ADD_PLAYER:
        oss << "player " << arg << " has been added";
        break;
    case GAME_START:
        oss << "game started ";
        break;
    case REPLAY:
        oss << "game replayed ";
        break;
    case QUIT:
        oss << "game is end ";
        break;
    case TOURNAMENT:
        oss << "Tournament mode: ";
        break;
    default:
        assert(false);
        break;
    }
    return oss.str();
}

Command::Command(CommandType type)
    : type(type)
{
}

Command::Command(const Command &other)
    : ILoggable(other), Subject(other), type(other.type), arg(other.arg), effect(other.effect)
{
}

Command::Command(CommandType type, const string &arg)
    : type(type), arg(arg)
{
}

Command::Command(const string &arg)
    : arg(arg)
{
}

Command::~Command()
{
}

Command &Command::operator=(const Command &other)
{
    if (this != &other)
    {
        ILoggable::operator=(other);
        Subject::operator=(other);
        this->type = other.type;
        this->arg = other.arg;
        this->effect = other.effect;
    }
    return *this;
}

CommandType Command::getType() const
{
    return type;
}

const string &Command::getArg() const
{
    return arg;
}

const string &Command::getEffect() const
{
    return effect;
}

void Command::saveEffect(const string &effect)
{
    this->effect = effect;
    logString = effect;
    notify(this);
}

void Command::saveEffect(const char *effect)
{
    this->effect = effect;
    logString = effect;
    notify(this);
}

void Command::saveEffect()
{
    saveEffect(getSuccessString());
}

void Command::print(ostream &os) const
{
    if (type != UNKNOWN_COMMAND)
    {
        os << getCommandTypeStr(type);
        if (!arg.empty())
            os << " " << arg;
    }
    else
        os << arg;
}

GameState Command::changeGameState(GameEngine *ge) const
{
    auto state = Command::getNewState(this->type, ge);
    ge->transition(state);
    return state;
}

GameState Command::getNewState(CommandType type, GameEngine *ge)
{
    GameState state = STARTED;
    switch (type)
    {
    case LOAD_MAP:
        state = MAP_LOADED;
        break;
    case VALIDATE_MAP:
        state = MAP_VALIDATED;
        break;
    case ADD_PLAYER:
        state = PLAYER_ADDED;
        break;
    case GAME_START:
        state = ASSIGN_REINFORCEMENT;
        break;
    case REPLAY:
        state = STARTED;
        break;
    case QUIT:
        state = GAME_END;
        break;
    case TOURNAMENT:
        state = TOURNAMENT_STATE;
        break;
    default:
        assert(false);
        break;
    }
    return state;
}

bool Command::validate(GameState state) const
{
    return Command::validate(this->type, state);
}

bool Command::validate(CommandType type, GameState state)
{
    bool res = false;
    switch (type)
    {
    case LOAD_MAP:
        res = (state == STARTED || state == MAP_LOADED);
        break;
    case VALIDATE_MAP:
        res = (state == MAP_LOADED);
        break;
    case ADD_PLAYER:
        res = (state == MAP_VALIDATED || state == PLAYER_ADDED);
        break;
    case GAME_START:
        res = (state == PLAYER_ADDED);
        break;
    case REPLAY:
        res = (state == PLAYER_WIN);
        break;
    case QUIT:
        res = (state == PLAYER_WIN);
        break;
    case TOURNAMENT:
        res = (state == STARTED || state == MAP_LOADED);
        break;
    default:
        assert(false);
        break;
    }
    return res;
}

ostream &operator<<(ostream &os, const Command &command)
{
    command.print(os);
    return os;
}

// get the string of command
string getCommandTypeStr(CommandType type)
{
    string s;
    switch (type)
    {
    case LOAD_MAP:
        s = "loadmap";
        break;
    case VALIDATE_MAP:
        s = "validatemap";
        break;
    case ADD_PLAYER:
        s = "addplayer";
        break;
    case GAME_START:
        s = "gamestart";
        break;
    case REPLAY:
        s = "replay";
        break;
    case QUIT:
        s = "quit";
        break;
    case TOURNAMENT:
        s = "TOURNAMENT";
        break;
    default:
        assert(false);
        break;
    }
    return s;
}

// remove whitespaces at the begin and end of a string
void trim(string &s)
{
    s.erase(0, s.find_first_not_of(" \r\n\t\v\f"));
    s.erase(s.find_last_not_of(" \r\n\t\v\f") + 1);
}

string CommandProcessor::readNextLine()
{
    string s;
    while (!cin.eof() && s.empty())
    {
        getline(cin, s);
        trim(s);
    }
    return s;
}

Command *CommandProcessor::readCommand(const string &line)
{
    Command *cmd = nullptr;
    istringstream iss(line);

    if (line == "validatemap")
    {
        cmd = new Command(VALIDATE_MAP);
    }
    else if (line == "gamestart")
    {
        cmd = new Command(GAME_START);
    }
    else if (line == "replay")
    {
        cmd = new Command(REPLAY);
    }
    else if (line == "quit")
    {
        cmd = new Command(QUIT);
    }
    else
    {
        string name;
        iss >> name;
        string arg;
        iss >> arg;
        if (!arg.empty())
            if (name == "loadmap")
            {
                cmd = new Command(LOAD_MAP, arg);
            }
            else if (name == "addplayer")
            {
                cmd = new Command(ADD_PLAYER, arg);
            }
            else if (name == "tournament")//读取tournament的参数
            {
                int a, b, c, d;
                iss >> a;
                iss >> arg;
                iss >> b;
                iss >> arg;
                iss >> c;
                iss >> arg;
                iss >> d;
                cmd = new Command(TOURNAMENT, a, b, c, d);
            }
    }

    if (cmd == nullptr)
        cmd = new Command(line);
    return cmd;
}
Command::Command(CommandType type, int a, int b, int c, int d) : type(type)
{
    arg = std::to_string(a) + " " + std::to_string(b) + " " + std::to_string(c) + " " + std::to_string(d)+" ";
}

void CommandProcessor::saveCommand(Command *command)
{
    push_back(command);
    ostringstream oss;
    oss << *command;
    logString = oss.str();
    notify(this);
}

bool CommandProcessor::validate(Command *command, GameEngine *ge) const
{
    return command->validate(ge->getState());
}

CommandProcessor::CommandProcessor()
{
}

CommandProcessor::CommandProcessor(const CommandProcessor &other)
    : list<Command *>(other), ILoggable(other), Subject(other)
{
}

CommandProcessor::~CommandProcessor()
{
}

CommandProcessor &CommandProcessor::operator=(const CommandProcessor &other)
{
    if (this != &other)
    {
        list<Command *>::operator=(other);
        ILoggable::operator=(other);
        Subject::operator=(other);
    }
    return *this;
}

Command *CommandProcessor::getCommand()
{
    string s = readNextLine();
    Command *command = readCommand(s);
    command->copyObservers(*this);
    saveCommand(command);
    return command;
}

void CommandProcessor::print(ostream &os) const
{
    for (auto x : *this)
        os << (*x) << endl;
}

ostream &operator<<(ostream &os, const CommandProcessor &commandProcessor)
{
    commandProcessor.print(os);
    return os;
}

string FileCommandProcessorAdapter::readNextLine()
{
    return reader->readLineFromFile();
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const string &filename)
    : reader(new FileLineReader(filename))
{
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const char *filename)
    : reader(new FileLineReader(filename))
{
}

const string &FileLineReader::getFileName() const
{
    return filename;
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter &other)
    : CommandProcessor(other), reader(other.reader)
{
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter()
{
    delete reader;
}

FileCommandProcessorAdapter &FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter &other)
{
    if (this != &other)
    {
        CommandProcessor::operator=(other);
        this->reader = other.reader;
    }

    return *this;
}

string FileLineReader::readLineFromFile()
{
    string s;
    while (!ifs.eof() && s.empty())
    {
        getline(ifs, s);
        trim(s);
    }
    return s;
}

FileLineReader::FileLineReader(const string &filename)
    : filename(filename), ifs(filename)
{
}
FileLineReader::FileLineReader(const char *filename)
    : filename(filename), ifs(filename)
{
}
FileLineReader::~FileLineReader()
{
}
FileLineReader::FileLineReader(const FileLineReader &other)
    : filename(other.filename)
{
}