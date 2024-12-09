#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
// #include <sys/wait.h>
#include <iomanip>
#include "Smash.h"
#include "../util.cpp"
#include "code/commands/built_in/change_dir/change_dir_command.h"
// #include "code/commands/built_in/show_current_dir/show_current_dir_command.h"

using namespace std;

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command* SmallShell::createCommand(const char *cmd_line) {
	// For example:
	string cmd_s = _trim(string(cmd_line));
	string firstWord = cmd_s.substr(0, cmd_s.find_first_of(" \n"));

	if (firstWord.compare("chprompt") == 0) {
		return (Command*)(new Chprompt(cmd_line));
	}
	else if (firstWord.compare("showpid") == 0) {
		return (Command*)(new ShowPidCommand(cmd_line));
	}
	else if (firstWord.compare("pwd") == 0) {
		return (Command*)(new GetCurrDirCommand(cmd_line));
	}
	else if (firstWord.compare("cd") == 0) {
		return (Command*)(new ChangeDirCommand(cmd_line));
	}
	/*
	else if ...
	.....
	else {
	return new ExternalCommand(cmd_line);
	}
	*/
	return nullptr;
}

void SmallShell::executeCommand(const char *cmd_line) {
    // TODO: Add your implementation here
    // for example:
    Command* cmd = createCommand(cmd_line);
    cmd->execute();
    // Please note that you must fork smash process for some commands (e.g., external commands....)
}