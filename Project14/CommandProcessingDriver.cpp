#include <assert.h>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::istringstream;
using std::ofstream;

#include "GameEngine.h"
#include "CommandProcessing.h"


string tab(4,' ');

//test getter of command
void CommandTest1() {
    {
        Command command("test");
        assert(command.getType()==UNKNOWN_COMMAND);
        assert(command.getArg() == "test");

        cout <<tab <<command <<endl;
    }
    {
        Command command(VALIDATE_MAP);
        assert(command.getType() == VALIDATE_MAP);
        assert(command.getArg() == "");
        cout << tab  << "after execute: " <<  command << endl;
        command.saveEffect();
        cout << tab << command.getSuccessString() <<endl;
        
    }
    {
        Command command(ADD_PLAYER,"Tom");
        assert(command.getType() == ADD_PLAYER);
        assert(command.getArg() == "Tom");

        cout << tab << "after execute: " << command << endl;
        command.saveEffect();
        cout << tab << command.getSuccessString() << endl;
    }
	{
		Command command(TOURNAMENT, 4, 2, 4, 20);
		assert(command.getType() == TOURNAMENT);

		cout << tab << "after execute: " << command << endl;
		command.saveEffect();
		cout << tab << command.getSuccessString() << endl;
	}
    cout << tab << "CommandTest1 success" << endl;
}

// test validate function
void CommandTest2() {
    assert(Command::validate(LOAD_MAP,STARTED)==true);
    assert(Command::validate(LOAD_MAP, MAP_LOADED) == true);
    assert(Command::validate(LOAD_MAP, ASSIGN_REINFORCEMENT) == false);

    assert(Command::validate(VALIDATE_MAP,MAP_LOADED)==true);
    assert(Command::validate(VALIDATE_MAP, STARTED) == false);

    assert(Command::validate(ADD_PLAYER, MAP_VALIDATED) == true);
    assert(Command::validate(ADD_PLAYER,PLAYER_ADDED) == true);
    assert(Command::validate(ADD_PLAYER, MAP_LOADED) == false);
    
    assert(Command::validate(GAME_START, PLAYER_ADDED) == true);
    assert(Command::validate(GAME_START, MAP_VALIDATED) == false);

    assert(Command::validate(REPLAY, PLAYER_WIN) == true);
    assert(Command::validate(REPLAY, EXECUTE_ORDERS) == false);

    assert(Command::validate(QUIT, PLAYER_WIN) == true);
    assert(Command::validate(QUIT, ISSUE_ORDERS) == false);

    cout << tab << "CommandTest2 success" << endl;
}

// test change state
void CommandTest3() {
    GameEngine ge;
    assert(ge.getState()==STARTED);

    {
        Command command(LOAD_MAP,"1.map");
        command.changeGameState(&ge);
        assert(ge.getState()==MAP_LOADED);
        command.changeGameState(&ge);
        assert(ge.getState() == MAP_LOADED);
    }

    {
        Command command(VALIDATE_MAP);
        command.changeGameState(&ge);
        assert(ge.getState() == MAP_VALIDATED);
    }

    {
        Command command(ADD_PLAYER,"Tom");
        command.changeGameState(&ge);
        assert(ge.getState() == PLAYER_ADDED);
        command.changeGameState(&ge);
        assert(ge.getState() == PLAYER_ADDED);
    }

    {
        Command command(GAME_START);
        command.changeGameState(&ge);
        assert(ge.getState() == ASSIGN_REINFORCEMENT);
    }

    {
        ge.transition(PLAYER_WIN);
        Command command(REPLAY);
        command.changeGameState(&ge);
        assert(ge.getState() == STARTED);
    }

    {
        ge.transition(PLAYER_WIN);
        Command command(QUIT);
        command.changeGameState(&ge);
        assert(ge.getState() == GAME_END);
    }

    cout << tab << "CommandTest3 success" << endl;
}



// test for read from console
void CommandProcessorTest1() {
    ostringstream oss;
    oss << "loadmap 1.map" <<endl;
    oss << "validatemap" << endl;
    oss << "addplayer Tom" << endl;
    oss << "gamestart" << endl;
    oss << "quit" <<endl;

    istringstream iss(oss.str());
    auto oldBuffer = cin.rdbuf(iss.rdbuf());
    CommandProcessor cp;
    
    while (true) {
        Command* cmd = cp.getCommand();
        cout << "reading " << (*cmd) << " from console" << endl;
        
        if (cmd->getType()==QUIT)
            break;
    }

    cin.rdbuf(oldBuffer);
    cout << tab << "CommandProcessorTest1 success" << endl;
}

// test for read from file
void CommandProcessorTest2() {
    ofstream ofs("command_list1.txt");
    ofs << "loadmap 1.map" << endl;
    ofs << "validatemap" << endl;
    ofs << "addplayer Tom" << endl;
    ofs << "gamestart" << endl;
    ofs << "quit" << endl;

    FileCommandProcessorAdapter fcp("command_list1.txt");
    while (true) {
        Command* cmd = fcp.getCommand();
        cout << "reading " << (*cmd) << " from file command_list1.txt" << endl;

        if (cmd->getType() == QUIT)
            break;
    }
    cout << tab << "CommandProcessorTest2 success" << endl;
}


/*
commands that are invalid in the current game state are rejected, 
and valid commands result in the correct effect and state change.
*/
void CommandProcessorTest3() {
    ofstream ofs("command_list2.txt");

    ofs << "load " << endl;   //unknown command1
    ofs << "loadmap 1.map" << endl;
    ofs << "addplayer Tom" << endl;  //not invalid at current state
    ofs << "validatemap" << endl;
    ofs << "addplayer Tom" << endl;
    ofs << "load " << endl;   //unknown command2
    ofs << "gamestart" << endl;
    ofs << "replay" << endl;
    ofs << "loadmap 2.txt" << endl;
    ofs << "validatemap" << endl;
    ofs << "addplayer Jerry" << endl;
    ofs << "gamestart" << endl;
    ofs << "quit" << endl;

    FileCommandProcessorAdapter fcp("command_list2.txt");
    GameEngine ge;
    while (true) {
        Command* cmd = fcp.getCommand();

        auto type = cmd->getType();
        if (type == UNKNOWN_COMMAND) {
            cmd->saveEffect("unknown command");
        }
        else {
            if (fcp.validate(cmd, &ge)) {
                cmd->saveEffect();
                cmd->changeGameState(&ge);
            }
            else {
                ostringstream oss;
                oss << "invalid command in state " <<getStateString(ge.getState());
                cmd->saveEffect(oss.str());
            }
        }

        if (cmd->getType() == QUIT)
            break;
        if (type == GAME_START)
            ge.transition(PLAYER_WIN);
    }
    for (auto x : fcp) {
        cout << "after execute " <<(*x) << ",its effect is " << x->getEffect() <<endl;
    }

    cout << tab << "CommandProcessorTest3 success" << endl;
}
void testCommandProcessorFF()
{

}

void testCommandProcessor() {
    cout << "begin of testCommandProcessor" << endl;
    CommandTest1();
    CommandTest2();
    CommandTest3();
    CommandProcessorTest1();
    CommandProcessorTest2();
    CommandProcessorTest3();
    testCommandProcessorFF();
    cout << "testCommandProcessor success" << endl;
}
