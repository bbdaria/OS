#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include "Smash.h"
#include "../util.cpp"
#include "../built_in/chprompt/chprompt_command.h"
#include "../built_in/show_pid/show_pid_command.h"
#include "../built_in/get_current_dir/get_current_dir_command.h"
#include "../built_in/change_dir/change_dir_command.h"
#include "../built_in/jobs/jobs_command.h"
#include "../built_in/foreground/foreground_command.h"
#include "../built_in/quit/quit_command.h"

using namespace std;

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command* SmallShell::createCommand(const char *cmd_line) {
	string cmd_s = _trim(string(cmd_line));
	char** args;
	int words = _parseCommandLine(cmd_line, args);
	Command* result = nullptr;
	if (words > 0) {
		result = _createBuiltInCommand(args, words);
		result = (result!=nullptr ? result : _createExternalCommand(args, words));
	}
	_freeArgs(args, words);
	return result;
}

BuiltInCommand* _createBuiltInCommand(char** args, int words) {
	string firstWord = strcpy(args[0]);
	_removeBackgroundSign(firstWord);

	if (firstWord.compare("chprompt") == 0) {
		return new Chprompt(args, words);
	}
	else if (firstWord.compare("showpid") == 0) {
		return new ShowPidCommand();
	}
	else if (firstWord.compare("pwd") == 0) {
		return new GetCurrDirCommand();
	}
	else if (firstWord.compare("cd") == 0) {
		return new ChangeDirCommand(args, words);
	}
	else if (firstWord.compare("jobs") == 0) {
		return new JobsCommand();
	}
	else if (firstWord.compare("fg") == 0) {
		return new ForegroundCommand(args, words);
	}
	else if (firstWord.compare("quit") == 0) {
		return new QuitCommand(args, words);
	}
	return nullptr;
}

ExternalCommand* _createExternalCommand(char** args, int words) {
	string firstWord = args[0];
	
	// if (firstWord.compare("chprompt") == 0) {
	// 	return new Chprompt(args, words);
	// }
	return nullptr;
}

void SmallShell::executeCommand(const char *cmd_line) {
    Command* cmd = createCommand(cmd_line);
	m_jobsList.removeFinishedJobs(); // removing finished jobs before executing cmd
	if (cmd != nullptr) {
        cmd->execute();
        delete cmd; // Prevent memory leaks
    }
	else {
		std::cout << std::endl;
	}
}