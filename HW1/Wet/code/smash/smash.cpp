#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include "smash/smash.h"

#include "util.h"
#include "commands/built_in/chprompt/chprompt_command.h"
#include "commands/built_in/show_pid/show_pid_command.h"
#include "commands/built_in/get_current_dir/get_current_dir_command.h"
#include "commands/built_in/change_dir/change_dir_command.h"
#include "commands/built_in/jobs/jobs_command.h"
#include "commands/built_in/foreground/foreground_command.h"
#include "commands/built_in/quit/quit_command.h"
#include "commands/built_in/kill/kill_command.h"
#include "commands/built_in/alias/alias_command.h"
#include "commands/built_in/alias/unalias_command.h"
#include "commands/built_in/special/listdir/listdir_command.h"
#include "commands/built_in/special/net_info/net_info_command.h"
#include "commands/built_in/special/whoami/who_am_i_command.h"

#include "commands/external/external_command.h"
#include "commands/external/simple/simple_command.h"
#include "commands/external/complex/complex_command.h"
#include "commands/external/special/pipe/pipe_command.h"

BuiltInCommand* _createBuiltInCommand(char** args, int words, const char* original_command) {
	std::string firstWord = _trim(args[0]);
	if (firstWord.at(firstWord.size()-1) == '&') {
		firstWord = firstWord.substr(0, firstWord.size()-1);
	}

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
	else if (firstWord.compare("kill") == 0) {
		return new KillCommand(args, words);
	}
	else if (firstWord.compare("alias") == 0) {
		return new AliasCommand(original_command);
	}
	else if (firstWord.compare("unalias") == 0) {
		return new UnaliasCommand(args, words);
	}
	else if (firstWord.compare("listdir") == 0) {
		return new ListDirCommand(args, words);
	}
	else if (firstWord.compare("netinfo") == 0) {
		return new NetinfoCommand(args, words);
	}
	else if (firstWord.compare("whoami") == 0) {
		return new WhoamiCommand();
	}
	return nullptr;
}

ExternalCommand* _createExternalCommand(const char *original_cmd_line, std::string& cmd_s, char** args, int words) {
	if (ExternalCommand::isComplexCommand(cmd_s)) {
		return new ComplexExternalCommand(original_cmd_line, cmd_s);
	} else {
		return new SimpleExternalCommand(original_cmd_line, cmd_s);
	}
	return nullptr;
}

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command* SmallShell::createCommand(const char *original_cmd_line, std::string& cmd_s) {
	if (cmd_s.find('|') != std::string::npos || cmd_s.find("|&") != std::string::npos) {
        // If it contains a pipe, create a PipeCommand
        return new PipeCommand(original_cmd_line, cmd_s);
    }
	
    char** args = (char**) malloc(sizeof(char*) * 20);  // Assuming a maximum of 20 arguments
	int words = _parseCommandLine(cmd_s.c_str(), args);
	Command* result = nullptr;
	if (words > 0) {
		result = _createBuiltInCommand(args, words, original_cmd_line);
		result = (result!=nullptr ? result : _createExternalCommand(original_cmd_line, cmd_s, args, words));
	}
	_freeArgs(args, words);
	return result;
}

void _redirectIO(Command* cmd, const char *cmd_line) {
	// check whether > or >> is in cmd_line and apply cmd->setRedirectionFile accordingly.
	std::string commandStr(cmd_line);
    size_t redirectionPos = commandStr.find_first_of('>');
    if (redirectionPos != std::string::npos) {
        bool append = (redirectionPos + 1 < commandStr.size() && commandStr[redirectionPos + 1] == '>');
        size_t fileStart = redirectionPos + (append ? 2 : 1);
		std::string secHalf = commandStr.substr(fileStart, commandStr.size()-fileStart);
        std::string file = _trim(secHalf);

        cmd->setRedirectionFile(file, append);
    }
}

std::string SmallShell::applyAlias(const std::string& cmd_line) {
    std::string cmd_s = cmd_line;
    std::map<std::string, AliasVal>& alias = getAliases();
    for (std::map<std::string, AliasVal>::iterator it = alias.begin(); it != alias.end(); ++it) {
		const std::string& key = it->first;
		const std::pair<std::string, char>& value = it->second;
		size_t pos = cmd_s.find(key);
		while (pos != std::string::npos) {
			cmd_s.replace(pos, key.length(), value.first);  // Replace alias with its value
			pos = cmd_s.find(key, pos + value.first.length());
		}
	}
    return cmd_s;
}

void SmallShell::executeCommand(const char *original_cmd_line) {
	std::string appliedAliasCmd = applyAlias(original_cmd_line);
	std::string cmd_s = _trim(appliedAliasCmd);

    Command* cmd = createCommand(original_cmd_line, cmd_s);
	m_jobsList.removeFinishedJobs(); // removing finished jobs before executing cmd
	if (cmd != nullptr) {
		_redirectIO(cmd, cmd_s.c_str());
        cmd->execute();
        delete cmd; // Prevent memory leaks
    }
}