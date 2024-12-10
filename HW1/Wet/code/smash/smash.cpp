#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
// #include <sys/wait.h>
#include <iomanip>
#include "Smash.h"
#include "../util.cpp"
#include "../built_in/chprompt/chprompt_command.h"
#include "../built_in/show_pid/show_pid_command.h"
#include "../built_in/get_current_dir/get_current_dir_command.h"
#include "../built_in/change_dir/change_dir_command.h"
#include "../built_in/jobs/jobs_command.h"

using namespace std;

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command* SmallShell::createCommand(const char *cmd_line) {
	// For example:
	string cmd_s = _trim(string(cmd_line));
	string firstWord = cmd_s.substr(0, cmd_s.find_first_of(" \n"));

	if (firstWord.compare("chprompt") == 0) {
		return new Chprompt(cmd_line);
	}
	else if (firstWord.compare("showpid") == 0) {
		return new ShowPidCommand(cmd_line);
	}
	else if (firstWord.compare("pwd") == 0) {
		return new GetCurrDirCommand(cmd_line);
	}
	// else if (firstWord.compare("cd") == 0) {
	// 	return new ChangeDirCommand(cmd_line); // plastPwd might not needed as input ?
	// }
	else if (firstWord.compare("jobs") == 0) {
		return new JobsCommand(cmd_line);
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
    Command* cmd = createCommand(cmd_line);
	if (cmd != nullptr) {
        cmd->execute();
        delete cmd; // Prevent memory leaks
    }
    // Please note that you must fork smash process for some commands (e.g., external commands....)
}